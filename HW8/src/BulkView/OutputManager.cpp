#include "OutputManager.h"

using namespace std::chrono_literals;

void OutputManager::subscribe(const std::string &subscribe_name, std::shared_ptr<struct ILogger> listener) {
    listeners.emplace(std::piecewise_construct_t(), std::forward_as_tuple(subscribe_name), std::forward_as_tuple(listener));
}

void OutputManager::unsubscribe(const std::string &subscribe_name) {
    auto it = listeners.find(subscribe_name);
    if (it != listeners.end()){
        it->second->make_quit();
        listeners.erase(it);
    }
}

void OutputManager::notify(const std::string & command) {
    if (!command.empty())
        cur_block.push_back(command);
}

void OutputManager::drop() {
    if (cur_block.empty())
        return;
    else {
        for (auto &obj : listeners) {
            obj.second->update(cur_block);
        }
        cur_block.clear();
    }
}

OutputManager::~OutputManager() {
    for (auto & l : listeners){
        l.second->make_quit();
    }
}

void ConsoleLogger::output([[maybe_unused]] size_t id) {
    while (!quit) {
        std::unique_lock<std::mutex> lk(cv_m);

        cv.wait(lk, [&]{
            return !buffer.empty() || quit;
        });

        if (!buffer.empty()) {
            Block buf = std::move(buffer.front());
            buffer.pop();
            lk.unlock();

            bool is_first = true;
            out << "bulk: ";
            for (auto &i : buf) {
                if (is_first) {
                    out << i;
                    is_first = false;
                } else
                    out << ", " << i;
            }
            out << std::endl;
        }
    }
}

ConsoleLogger::ConsoleLogger(std::ostream &os) : out(os) {
    size_t s = thread_pool.size();
    thread_pool.emplace_back(std::thread{[s, this] { output(s); }});
}

FileLogger::FileLogger(std::filesystem::path cur_path, size_t thread_size): fs(std::move(cur_path)) {
    for (size_t i = 0; i < thread_size; i++) {
        size_t s = thread_pool.size();
        thread_pool.emplace_back(std::thread{[s, this] { output(s); }});
    }
}

void FileLogger::CreateNewFile(size_t id) {
    if (ofile != nullptr){
        ofile->close();
        delete ofile;
    }
    ofile = new std::ofstream(fs/("bulk" + (std::to_string(time_manager.GetUnixTime())) + "_" + std::to_string(id) + ".log"));
}

void FileLogger::output(size_t id) {
    while (!quit) {
        std::unique_lock<std::mutex> lk(cv_m);

        cv.wait(lk, [&]{
            return !buffer.empty() || quit;
        });

        if (!buffer.empty()) {
            Block buf = std::move(buffer.front());
            buffer.pop();
            lk.unlock();

//            std::this_thread::sleep_for(5s);

            CreateNewFile(id);
            bool is_first = true;
            *ofile << "bulk: ";
            for (auto &i : buf) {
                if (is_first) {
                    *ofile << i;
                    is_first = false;
                } else
                    *ofile << ", " << i;
            }
            *ofile << std::endl;
        }
    }
}

void ILogger::make_quit() const {
    quit = true;
}

void ILogger::update(const Block &bl) {
    std::lock_guard<std::mutex> lg(cv_m);
    buffer.emplace(bl);
    cv.notify_one();
}

ILogger::~ILogger() {
    for (auto & t : thread_pool){
        make_quit();
        cv.notify_all();
        t.join();
    }
}
