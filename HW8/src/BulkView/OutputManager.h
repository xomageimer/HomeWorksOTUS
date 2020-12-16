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

struct OutputManager {
private:
    Block cur_block;

    std::unordered_map<std::string, std::shared_ptr<struct ILogger>> listeners;
public:
    ~OutputManager();
    void subscribe(const std::string & subscribe_name, std::shared_ptr<struct ILogger> listener);
    void unsubscribe(const std::string & subscribe_name);
    void notify(const std::string &);
    void drop();
};

struct ILogger {
protected:
    std::queue<Block> buffer;

    mutable std::atomic_bool quit = false;
    std::condition_variable cv;
    std::mutex cv_m;

    std::vector<std::thread> thread_pool;
public:
    virtual ~ILogger();

    virtual void update(const Block & bl);
    virtual void output(size_t id) = 0;
    virtual void make_quit() const;
};

struct ConsoleLogger : public ILogger {
private:
    std::ostream& out;
public:
    explicit ConsoleLogger(std::ostream& os);
    void output([[maybe_unused]] size_t id) override;
};

struct FileLogger : public ILogger {
private:
    std::ofstream * ofile = nullptr;
    const std::filesystem::path fs;
    TimeManager time_manager {};
public:
    explicit FileLogger(std::filesystem::path  cur_path, size_t thread_size = 1);
    void CreateNewFile(size_t id);
    void output(size_t id) override;
};


#endif //OTUSHOMEWORKS_OUTPUTMANAGER_H
