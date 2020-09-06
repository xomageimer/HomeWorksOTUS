#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <utility>
#include <memory>

#ifndef ALGORYTHMS_TESTER_H
#define ALGORYTHMS_TESTER_H

struct Tasker{
    virtual std::string Run(std::string) = 0;
};

struct Tester{
    explicit Tester(std::unique_ptr<Tasker> cell) : task(move(cell)){
    }
    bool RunTest();
    std::string RunTest(std::istream & in, std::istream& out);
private:
    std::unique_ptr<Tasker> task;
};

#define solve : Tasker
#define Issue std::unique_ptr<Tasker>
#define new_Issue(n) std::make_unique<n>()


#endif //ALGORYTHMS_TESTER_H
