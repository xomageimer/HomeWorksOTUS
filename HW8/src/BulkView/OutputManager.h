#ifndef OTUSHOMEWORKS_OUTPUTMANAGER_H
#define OTUSHOMEWORKS_OUTPUTMANAGER_H

#include <memory>
#include <unordered_map>
#include <filesystem>
#include <vector>
#include <fstream>
#include <string>
#include <queue>

#include <future>
#include <thread>
#include <condition_variable>
#include <mutex>

#include "TimeManager.h"

using Block = std::vector<std::string>;

struct ProxyBuffer{
private:
    std::queue<Block> & buffer;
public:
    ProxyBuffer(std::queue<Block> & b, std::condition_variable & cv_, std::mutex & cv_m) :
        buffer(b), cv(cv_), cv_mutex(cv_m) {}

    Block & GetBuffer();
    [[nodiscard]] bool Empty() const;
    void DeleteBlock();

    std::condition_variable & cv;
    std::mutex & cv_mutex;

    std::atomic_bool console_completed = false;
    std::atomic_bool quit = false;
};

struct OutputManager {
private:
    std::queue<Block> buffer;
    std::vector<std::thread> thread_pool;

    std::thread console_t;
    std::condition_variable console_cv;

    ProxyBuffer proxy;

    std::condition_variable cv;
    std::mutex cv_m;

    std::unordered_map<std::string, std::shared_ptr<struct ILogger>> listeners;
public:
    OutputManager();
    ~OutputManager();
    inline void make_quit() { proxy.quit = true; }
    void subscribe(const std::string & subscribe_name, std::shared_ptr<struct ILogger> listener);
    void notify(const std::string &);
    void unsubscribe(const std::string & subscribe_name);
    void drop();
};

struct ILogger {
public:
    virtual void output(ProxyBuffer & buf, size_t id) = 0;
};

struct ConsoleLogger : public ILogger {
private:
    std::ostream& out;
public:
    explicit ConsoleLogger(std::ostream& os) : out(os){};
    void output(ProxyBuffer & buf, [[maybe_unused]] size_t id) override;
};

struct FileLogger : public ILogger {
private:
    std::ofstream * ofile = nullptr;
    const std::filesystem::path fs;
    TimeManager time_manager {};
public:
    explicit FileLogger(const std::filesystem::path& cur_path);
    void CreateNewFile(size_t id);
    void output(ProxyBuffer & buf, size_t id) override;
};


#endif //OTUSHOMEWORKS_OUTPUTMANAGER_H
