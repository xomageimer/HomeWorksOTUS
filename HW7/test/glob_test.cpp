#include <gtest/gtest.h>

#include "Glob_Parser.h"

TEST(GlobParser, Test_EQ_1){
    Glob_Parser gp;
    std::string str = "[a-Z]*.?onf";
    std::string expected = "(?i)([a-Z]).*\\..onf";

    gp.set_str(str);
    auto answer = gp();

    ASSERT_STREQ(answer.str().c_str(), expected.c_str());
}

TEST(GlobParser, Test_EQ_2){
    Glob_Parser gp;
    std::string str;
    std::string expected = ".*";

    gp.set_str(str);
    auto answer = gp();

    ASSERT_STREQ(answer.str().c_str(), expected.c_str());
}

TEST(GlobParser, Test_EQ_3){
    Glob_Parser gp;
    std::string str = "[!azb][1-7]??.txt";
    std::string expected = "(?i)([^a|z|b])([1-7])..\\.txt";

    gp.set_str(str);
    auto answer = gp();

    ASSERT_STREQ(answer.str().c_str(), expected.c_str());
}

TEST(GlobParser, Test_EQ_4){
    Glob_Parser gp;
    std::string str = "a*.?";
    std::string expected = "(?i)a.*\\..";

    gp.set_str(str);
    auto answer = gp();

    ASSERT_STREQ(answer.str().c_str(), expected.c_str());
}

TEST(GlobParser, Test_EQ_5){
    Glob_Parser gp;
    std::string str = "[a-z2]?.txt";
    std::string expected = "(?i)([a-z|2]).\\.txt";

    gp.set_str(str);
    auto answer = gp();

    ASSERT_STREQ(answer.str().c_str(), expected.c_str());
}

TEST(GlobMatch, Test_1){
    std::vector<std::string> v {"important_file.txt",
                                "notimportant_file.conf",
                                "student_file.txt"};
    Glob_Parser gp ("*_*.txt");

    ASSERT_TRUE(boost::regex_match(v[0], gp()));
    ASSERT_FALSE(boost::regex_match(v[1], gp()));
    ASSERT_TRUE(boost::regex_match(v[2], gp()));
}

TEST(GlobMatch, Test_2){
    std::vector<std::string> v {"important_2014file.txt",
                                "notimportant_2014file.txt",
                                "student_2013file.txt"};
    Glob_Parser gp ("*_2014*.txt");

    ASSERT_TRUE(boost::regex_match(v[0], gp()));
    ASSERT_FALSE(boost::regex_match(v[2], gp()));
    ASSERT_TRUE(boost::regex_match(v[1], gp()));
}

TEST(GlobMatch, Test_3){
    std::vector<std::string> v {"aserver.txt",
                                "2client.conf",
                                "zproxy.txt",
                                "5handshake.conf"};
    Glob_Parser gp ("[a-z|2]*.*");

    ASSERT_TRUE(boost::regex_match(v[0], gp()));
    ASSERT_FALSE(boost::regex_match(v[3], gp()));
    ASSERT_TRUE(boost::regex_match(v[1], gp()));
    ASSERT_TRUE(boost::regex_match(v[2], gp()));
}

TEST(GlobMatch, Test_4){
    std::vector<std::string> v {"aserver.txt",
                                "2client.conf",
                                "zproxy.txt",
                                "5handshake.conf"};
    Glob_Parser gp ("[!a-z|2]*.*");

    ASSERT_FALSE(boost::regex_match(v[0], gp()));
    ASSERT_TRUE(boost::regex_match(v[3], gp()));
    ASSERT_FALSE(boost::regex_match(v[1], gp()));
    ASSERT_FALSE(boost::regex_match(v[2], gp()));
}
