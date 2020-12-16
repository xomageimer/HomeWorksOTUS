#ifndef OTUSHOMEWORKS_CONTROLLER_H
#define OTUSHOMEWORKS_CONTROLLER_H

#include "OutputManager.h"

struct Event{
    enum class Type{
        READ = 0, WRITE
    };
    Type type;
};

struct IController {
protected:
    std::string command;
public:
    virtual void HandleEvent([[maybe_unused]]const Event&, std::string str = "") = 0;
};

struct BulkController : IController{
public:
    struct OutputManager manager;

    void HandleEvent(const Event&, std::string str = "") override;
    void ActionWrite();
    void ActionRead();
};

#endif //OTUSHOMEWORKS_CONTROLLER_H
