#ifndef OTUSHOMEWORKS_BULK_H
#define OTUSHOMEWORKS_BULK_H

#include "Controller.h"
#include "BulkReader.h"
#include "OutputManager.h"

// builder
struct Bulk {
    Bulk() = default;

    void SetBulkModel(std::istream& input);

    void SetDefaultBulkMode();

    void SetController(std::shared_ptr<BulkController> controller);

    void SubscribeLogger(const std::string & sub_name, std::shared_ptr<ILogger> logger);

    void GetStr(const char * msg, std::size_t msg_size);

    void build(char *);
    bool run();

    // for tests
    [[deprecated]] void run(int);

private:
    struct Data{
        std::optional<BulkReader> model;
        std::shared_ptr<BulkController> controller;
    } data;
    std::istream * inp;
};
#endif //OTUSHOMEWORKS_BULK_H
