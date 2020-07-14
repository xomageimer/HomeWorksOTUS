#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include <algorithm>
#include "Tester.h"
#include "ip_filter.h"

using namespace std;

void print_ip2(std::vector<std::vector<int>>& ip_pool, ostream& out){
    for(std::vector<std::vector<int> >::const_iterator ip = ip_pool.cbegin(); ip != ip_pool.cend(); ++ip)
    {
        for(std::vector<int>::const_iterator ip_part = ip->cbegin(); ip_part != ip->cend(); ++ip_part)
        {
            if (ip_part != ip->cbegin())
            {
                out << ".";
            }
            out << *ip_part;
        }
        out << '\n';
    }
}

struct fexture solve{
    std::string Run(std::string input) override {
        stringstream ss (input);

        //cerr << ss.str() << endl;
        ostringstream out;
        std::vector<std::vector<int>> ip_pool;

        for(std::string line; std::getline(/*std::cin*/ ss, line);)
        {
            std::vector<std::string> v = split(line, '\t');
            ip_pool.push_back(toInt(split(v.at(0), '.')));
        }

        sort_ip(ip_pool);

        print_ip2(ip_pool, out);


        auto n_ip = filter_by(ip_pool, 1);
        print_ip2(n_ip, out);


        auto n_ip2 = filter_by(ip_pool, 46, 70);
        print_ip2(n_ip2, out);


        auto n_ip3 = filter_ant_of(ip_pool, 46);
        print_ip2(n_ip3, out);


        return out.str();
    }
};

TEST(MyFunCase, TestIsTrue)
{
    Issue iss = new_Issue(fexture);
    Tester test(move(iss));
    EXPECT_TRUE(testing::internal::IsTrue(test.RunTest()));
}