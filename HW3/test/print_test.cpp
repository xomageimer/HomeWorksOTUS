#include <gtest/gtest.h>
#include "print_meta_ip.cpp"
#include "pch.h"

using namespace std;

TEST (valid_cast, first_case){
    int a;
    double b = 5.15;

    a = static_cast<int>(b);

    ASSERT_EQ(5, a);
}
//
//TEST (integral_types, first_case){
//    ostringstream answer;
//    print_ip<char, -1>(answer);
//    print_ip<short, 0>(answer);
//    print_ip<int, 2130706433>(answer);
//    print_ip<long long, 8875824491850138409>(answer);
//
//    stringstream expected {
//            "255\n"
//            "0.0\n"
//            "127.0.0.1\n"
//            "123.45.67.89.101.112.131.41\n"
//    };
//
//    ASSERT_EQ(expected.str(), answer.str());
//}
//
//
//TEST (integral_types, second_case){
//    ostringstream answer;
//    print_ip<char, 127>(answer);
//    print_ip<short, 10000>(answer);
//    print_ip<int, 111111111>(answer);
//    print_ip<long long, 700000005>(answer);
//
//    stringstream expected {
//            "127\n"
//            "39.16\n"
//            "6.159.107.199\n"
//            "0.0.0.0.41.185.39.5\n"
//    };
//
//    ASSERT_EQ(expected.str(), answer.str());
//}
//
//TEST (non_trivial_types, first_case){
//    ostringstream answer;
//    print_ip<string>("255.#SECRET#.123.11", answer);
//    print_ip<vector<int>>(vector<int> {1,2,3,4}, answer);
//    print_ip<list<string>>( list<string> {"12", "114", "234", "11"}, answer);
//    print_ip(make_tuple(213, 0, 13, 255), answer);
//
//    stringstream expected {
//            "255.#SECRET#.123.11\n"
//            "1.2.3.4\n"
//            "12.114.234.11\n"
//            "213.0.13.255\n"
//    };
//
//    ASSERT_EQ(expected.str(), answer.str());
//}