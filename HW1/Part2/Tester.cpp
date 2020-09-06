#include "Tester.h"


bool Tester::RunTest() {
        std::string InFile = "ip_filter.tsv";
        std::string OutFile = "ip_filter.tst";
        std::fstream testin(InFile);
        std::fstream testout(OutFile);
        if (!testin || !testout) {
            std::cerr << "cant open/find files!\n";
        }
       //std::cerr << RunTest(testin, testout);
        return RunTest(testin, testout) == "OK";
}

std::string Tester::RunTest(std::istream &in, std::istream &out) {
    try {
        std::string input;
        std::string expected;
        std::string tmpword1;
        std::string tmpword2;
        for(;getline(in, tmpword1), getline(out, tmpword2);){
            input += tmpword1 + '\n';
            expected += tmpword2 + '\n';
        }
        while (isspace(input.back()))
            input.erase(--input.end());
        while (isspace(expected.back()))
            expected.erase(--expected.end());
        {
            expected += '\n';
            //std::cerr << task->Run(input) << std::endl;
            if (task->Run(input) == expected)
                return "OK";
            else
                return {"answer != expected"};
        }
    } catch (std::exception &e){
        std::cerr << e.what() << std::endl;
        return {"Programm drops with Error!" };
    }
}
