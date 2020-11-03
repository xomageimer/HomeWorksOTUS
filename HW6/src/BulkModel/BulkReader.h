#ifndef OTUSHOMEWORKS_BULKREADER_H
#define OTUSHOMEWORKS_BULKREADER_H

#include "Controller.h"
#include <memory>
#include <iostream>

struct BulkReader {
private:
    std::shared_ptr<struct IHandler> handler;
    std::istream& input;
    std::shared_ptr<struct IController> controller;
public:
    const int counter_cmd;

    explicit BulkReader(std::istream& in, std::shared_ptr<struct IController> contr, int c);

    void ParseStandard(int n);
    void ParseDynamic(int n);

    [[nodiscard]] std::string GetCmd() const;
    [[nodiscard]] bool CheckSymb(char delim = '{') const;
};

struct IHandler{
protected:
    int counter_cmd;
    std::shared_ptr<struct IController> controller;
public:
    explicit IHandler(int n) : counter_cmd(n) {}
    void SetController(std::shared_ptr<struct IController>);
    virtual void Execute(BulkReader * Application) = 0;
    virtual Event job() = 0;
};

struct StandardHandler : public IHandler {
public:
    explicit StandardHandler(int n) : IHandler(n) {}
    void Execute(BulkReader * Application) override;
    Event job() override;
};

struct DynamicHandler : public IHandler {
public:
    explicit DynamicHandler(int n) : IHandler(n) {}
    void Execute(BulkReader * Application) override;
    Event job() override;
};



#endif //OTUSHOMEWORKS_BULKREADER_H
