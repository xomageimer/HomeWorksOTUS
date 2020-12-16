#include "OutputManager.h"

using namespace std::chrono_literals;

void OutputManager::subscribe(const std::string &subscribe_name, std::shared_ptr<struct ILogger> listener) {
    listeners.emplace(std::piecewise_construct_t(), std::forward_as_tuple(subscribe_name), std::forward_as_tuple(listener));
    thread_pool.emplace_back(std::thread{[capture = listener.get(), this] { capture->output(proxy, thread_pool.size() - 1); }});
}

void OutputManager::unsubscribe(const std::string &subscribe_name) {
    listeners.erase(subscribe_name);
}

void OutputManager::notify(const std::string & command) {
    if (!command.empty())
        buffer.back().emplace_back(command);
}

void OutputManager::drop() {
    if (buffer.empty())
        buffer.push({});
    else if (!buffer.front().empty()) {
        cv.notify_all();
        buffer.push({});
    }
}

OutputManager::OutputManager() : proxy(buffer, cv, cv_m) {
    buffer.push({});
}

OutputManager::~OutputManager() {
    make_quit();
    cv.notify_all();
    for (auto & t : thread_pool){
        t.join();
    }
}

void ConsoleLogger::output(ProxyBuffer & buf, [[maybe_unused]] size_t id) {
    while (!buf.quit) {
        std::unique_lock<std::mutex> lk(buf.cv_mutex);

        buf.cv.wait(lk, [&buf]{
            return (!buf.GetBuffer().empty() && !buf.console_completed) || buf.quit;
        });

        if (!buf.GetBuffer().empty() && !buf.console_completed) {
            Block buffer = buf.GetBuffer();
            buf.console_completed = true;
            lk.unlock();

            bool is_first = true;
            out << "bulk: ";
            for (auto &i : buffer) {
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

FileLogger::FileLogger(const std::filesystem::path & cur_path): fs(cur_path) {
}

void FileLogger::CreateNewFile(size_t id) {
    if (ofile != nullptr){
        ofile->close();
        delete ofile;
    }
    ofile = new std::ofstream(fs/("bulk" + (std::to_string(time_manager.GetUnixTime())) + "_" + std::to_string(id) + ".log"));
}

void FileLogger::output(ProxyBuffer & buf, size_t id) {
    while (!buf.quit) {
        std::unique_lock<std::mutex> lk(buf.cv_mutex);

        buf.cv.wait_for(lk, 100ms, [&buf]{
            return (!buf.GetBuffer().empty() && buf.console_completed) || buf.quit ;
        });

        if (!buf.GetBuffer().empty() && buf.console_completed) {
            Block buffer = std::move(buf.GetBuffer());
            buf.DeleteBlock();
            lk.unlock();

            CreateNewFile(id);
            bool is_first = true;
            *ofile << "bulk: ";
            for (auto &i : buffer) {
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

Block & ProxyBuffer::GetBuffer() {
    return buffer.front();
}

void ProxyBuffer::DeleteBlock() {
    buffer.pop();
    console_completed = false;
}

bool ProxyBuffer::Empty() const {
    return buffer.empty();
}
