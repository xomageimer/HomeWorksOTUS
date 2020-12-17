#include "async.h"

namespace async {

    handle_t connect(std::size_t bulk) {
        return EventHandler::Instance().make_connect(bulk);
    }

    void receive(handle_t handle, const char *data, std::size_t size) {
        EventHandler::Instance().make_receive(handle, data, size);
    }

    void disconnect(handle_t handle) {
        EventHandler::Instance().make_disconnect(handle);
    }

    EventHandler::EventHandler() : handle_thread(std::thread{[this] { worker(); }}) {}

    EventHandler::~EventHandler() {
        quit = true;
        handle_thread.join();
    }

    handle_t EventHandler::make_connect(size_t s) {
        std::lock_guard lg(m);
        auto m_bulk = new Bulk();
        m_bulk->SetDefaultBulkMode();
        m_bulk->SetController(std::make_shared<BulkController>());
        m_bulk->SubscribeLogger("Console_Outputer", std::make_shared<ConsoleLogger>(std::cout));
        m_bulk->SubscribeLogger("File_Outputer", std::make_shared<FileLogger>(std::filesystem::current_path(), 2));
        auto it = handlers.emplace(std::piecewise_construct, std::forward_as_tuple(m_bulk), std::forward_as_tuple(request_queue{}));
        if (!it.second) {
            delete m_bulk;
            throw std::bad_alloc();
        }
        m_bulk->build(std::to_string(s).data());
        return m_bulk;
    }

    void EventHandler::make_disconnect(handle_t h) {
        std::lock_guard lg(m);
        auto to_del = handlers.find(reinterpret_cast<Bulk *>(h));
        if (to_del != handlers.end()){
            auto tmp = to_del->first;
            handlers.erase(to_del);
            delete tmp;
        }
    }

    void EventHandler::make_receive(handle_t h, const char *msg, std::size_t msg_size) {
        std::lock_guard lg(m);
        auto it = handlers.find(reinterpret_cast<Bulk *>(h));
        if (it != handlers.end()){
            it->second.push(std::make_pair(msg, msg_size));
        }
    }

    void EventHandler::worker() {
        while (!quit && handlers.empty()){
            std::unique_lock lk(m);
            for (auto & [handler, queue] : handlers){
                if (!queue.empty()){
                    auto msg = std::move(queue.front());
                    queue.pop();
                    lk.unlock();
                    handler->GetStr(msg.first, msg.second);
                    handler->run();
                    lk.lock();
                }
            }
        }
    }
}
