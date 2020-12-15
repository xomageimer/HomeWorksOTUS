#include "OutputManager.h"

void OutputManager::subscribe(const std::string &subscribe_name, std::shared_ptr<struct ILogger> listener) {
    listeners.emplace(std::piecewise_construct_t(), std::forward_as_tuple(subscribe_name), std::forward_as_tuple(listener));
}

void OutputManager::unsubscribe(const std::string &subscribe_name) {
    listeners.erase(subscribe_name);
}

void OutputManager::notify(const std::string & command) {
    for (auto & subscriber : listeners){
        subscriber.second->update(command);
    }
}

void OutputManager::drop() {
    for (auto & subscriber : listeners){
        subscriber.second->output();
    }
}

void ILogger::update(std::string com) {
    if (!com.empty())
        buffer.emplace_back(com);
}

void ConsoleLogger::output() {
    if (buffer.empty()) return;
    bool is_first = true;
    out << "bulk: ";
    for (auto & i : buffer){
        if (is_first) {
            out << i;
            is_first = false;
        }
        else
            out << ", " << i;
    }
    out << std::endl;
    buffer.clear();
}

FileLogger::FileLogger(const std::filesystem::path &cur_path): fs(cur_path) {
}

void FileLogger::CreateNewFile() {
    if (ofile != nullptr){
        ofile->close();
        delete ofile;
    }
    ofile = new std::ofstream(fs/("bulk" + (std::to_string(time_manager.GetUnixTime())) + ".log"));
}

void FileLogger::output() {
    if (buffer.empty()) return;
    CreateNewFile();
    bool is_first = true;
    *ofile << "bulk: ";
    for (auto & i : buffer){
        if (is_first) {
            *ofile << i;
            is_first = false;
        }
        else
            *ofile << ", " << i;
    }
    *ofile <<  std::endl;
    buffer.clear();
}
