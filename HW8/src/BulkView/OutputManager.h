#ifndef OTUSHOMEWORKS_OUTPUTMANAGER_H
#define OTUSHOMEWORKS_OUTPUTMANAGER_H

#include <memory>
#include <unordered_map>
#include <filesystem>
#include <vector>
#include <fstream>
#include <string>

#include "TimeManager.h"

struct OutputManager {
private:
    std::unordered_map<std::string, std::shared_ptr<struct ILogger>> listeners;
public:
    void subscribe(const std::string & subscribe_name, std::shared_ptr<struct ILogger> listener);
    void notify(const std::string &);
    void unsubscribe(const std::string & subscribe_name);
    void drop();
};

struct ILogger {
protected:
    std::vector<std::string> buffer;
public:
    void update(std::string);
    virtual void output() = 0;
};

struct ConsoleLogger : public ILogger {
private:
    std::ostream& out;
public:
    explicit ConsoleLogger(std::ostream& os) : out(os){};
    void output() override;
};

struct FileLogger : public ILogger {
private:
    std::ofstream * ofile = nullptr;
    const std::filesystem::path fs;
    TimeManager time_manager {};
public:
    explicit FileLogger(const std::filesystem::path& cur_path);
    void CreateNewFile();
    void output() override;
};


#endif //OTUSHOMEWORKS_OUTPUTMANAGER_H
