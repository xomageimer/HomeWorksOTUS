#include <gtest/gtest.h>

#include <sstream>
#include <fstream>


#include "Bulk.h"

using namespace std;

TEST (bulk, ConsoleLogger){
    stringstream input {
        "cmd1\n"
        "cmd2\n"
        "cmd3\n"
        "cmd4\n"
        "cmd5\n"
    };
    int NUMBERS = 3;
    Bulk blk;
    std::stringstream answer;

    blk.SetController(std::make_shared<BulkController>());
    blk.SetBulkModel(input);
    blk.SubscribeLogger("Console_Outputer", std::make_shared<ConsoleLogger>(answer));
    blk.run(NUMBERS);


    std::stringstream expected {
            "bulk: cmd1, cmd2, cmd3\n"
            "bulk: cmd4, cmd5\n"
    };
    EXPECT_EQ(answer.str(), expected.str());
}

TEST (bulk, FileLogger){
    stringstream input {
            "cmd1\n"
            "cmd2\n"
            "cmd3\n"
            "cmd4\n"
            "cmd5\n"
    };
    int NUMBERS = 3;
    Bulk blk;

    std::fstream file_with_answer;
    std::stringstream answer;

    blk.SetController(std::make_shared<BulkController>());
    blk.SetBulkModel(input);
    blk.SubscribeLogger("File_Outputer", std::make_shared<FileLogger>(std::filesystem::current_path()));
    blk.run(NUMBERS);
    auto safe_time = TimeManager::last_time;

    file_with_answer.open(std::filesystem::current_path()/("bulk" + (std::to_string(safe_time)) + ".log"));
    answer << file_with_answer.rdbuf();

    std::stringstream expected {
            "bulk: cmd4, cmd5\n"
    };
    EXPECT_EQ(answer.str(), expected.str());
}

TEST (bulk, DynamicInput_1){
    stringstream input {
            "cmd1\n"
            "cmd2\n"
            "{\n"
            "cmd3\n"
            "cmd4\n"
            "cmd5\n"
            "\n"
            "cmd6\n"
            "{\n"
    };
    int NUMBERS = 3;
    Bulk blk;
    std::stringstream answer;

    blk.SetController(std::make_shared<BulkController>());
    blk.SetBulkModel(input);
    blk.SubscribeLogger("Console_Outputer", std::make_shared<ConsoleLogger>(answer));
    blk.run(NUMBERS);


    std::stringstream expected {
            "bulk: cmd1, cmd2\n"
            "bulk: cmd3, cmd4, cmd5, cmd6\n"
    };
    EXPECT_EQ(answer.str(), expected.str());
}


TEST (bulk, DynamicInput_2){
    stringstream input {
            "cmd1\n"
            "cmd2\n"
            "{\n"
            "cmd3\n"
            "cmd4\n"
            "cmd5\n"
            "\n"
            "cmd6\n"
            "{\n"
            "cmd7\n"
            "cmd8\n"
            "}"
            "cmd9\n"
            "}\n"
            "cmd10\n"
            "cmd11\n"
            "cmd12\n"
    };
    int NUMBERS = 3;
    Bulk blk;
    std::stringstream answer;

    blk.SetController(std::make_shared<BulkController>());
    blk.SetBulkModel(input);
    blk.SubscribeLogger("Console_Outputer", std::make_shared<ConsoleLogger>(answer));
    blk.run(NUMBERS);


    std::stringstream expected {
            "bulk: cmd1, cmd2\n"
            "bulk: cmd3, cmd4, cmd5, cmd6, cmd7, cmd8, cmd9\n"
            "bulk: cmd10, cmd11, cmd12\n"
    };
    EXPECT_EQ(answer.str(), expected.str());
}

TEST (bulk, DynamicInput_3){
    stringstream input {
            "cmd1\n"
            "cmd2\n"
            "{\n"
            "cmd3\n"
            "cmd4\n"
            "cmd5\n"
            "\n"
            "cmd6\n"
            "{\n"
            "cmd7\n"
            "cmd8\n"
            "}"
            "cmd9\n"
            "}\n"
            "cmd10\n"
            "cmd11\n"
            "cmd12\n"
    };
    int NUMBERS = 1;
    Bulk blk;
    std::stringstream answer;

    blk.SetController(std::make_shared<BulkController>());
    blk.SetBulkModel(input);
    blk.SubscribeLogger("Console_Outputer", std::make_shared<ConsoleLogger>(answer));
    blk.run(NUMBERS);


    std::stringstream expected {
            "bulk: cmd1\n"
            "bulk: cmd2\n"
            "bulk: cmd3, cmd4, cmd5, cmd6, cmd7, cmd8, cmd9\n"
            "bulk: cmd10\n"
            "bulk: cmd11\n"
            "bulk: cmd12\n"
    };
    EXPECT_EQ(answer.str(), expected.str());
}

TEST (bulk, DynamicInput_4){
    stringstream input {
            "cmd1\n"
            "cmd2\n"
            "{\n"
            "cmd3\n"
            "cmd4\n"
            "cmd5\n"
            "\n"
            "cmd6\n"
            "{\n"
            "cmd7\n"
            "cmd8\n"
            "}"
            "cmd9\n"
            "}\n"
            "cmd10\n"
            "cmd11\n"
            "cmd12\n"
            "{\n"
            "cmd6\n"
            "}\n"
            "cmd1\n"
            "cmd2\n"
            "cmd3\n"
            "cmd4\n"
            "cmd5\n"
    };
    int NUMBERS = 5;
    Bulk blk;
    std::stringstream answer;

    blk.SetController(std::make_shared<BulkController>());
    blk.SetBulkModel(input);
    blk.SubscribeLogger("Console_Outputer", std::make_shared<ConsoleLogger>(answer));
    blk.run(NUMBERS);


    std::stringstream expected {
            "bulk: cmd1, cmd2\n"
            "bulk: cmd3, cmd4, cmd5, cmd6, cmd7, cmd8, cmd9\n"
            "bulk: cmd10, cmd11, cmd12\n"
            "bulk: cmd6\n"
            "bulk: cmd1, cmd2, cmd3, cmd4, cmd5\n"
    };
    EXPECT_EQ(answer.str(), expected.str());
}

TEST (bulk, StaticInput_1){
    stringstream input {
            "cmd1\n"
            "cmd2\n"
            "cmd3\n"
            "cmd4\n"
            "cmd5\n"
    };
    int NUMBERS = 1;
    Bulk blk;
    std::stringstream answer;

    blk.SetController(std::make_shared<BulkController>());
    blk.SetBulkModel(input);
    blk.SubscribeLogger("Console_Outputer", std::make_shared<ConsoleLogger>(answer));
    blk.run(NUMBERS);


    std::stringstream expected {
            "bulk: cmd1\n"
            "bulk: cmd2\n"
            "bulk: cmd3\n"
            "bulk: cmd4\n"
            "bulk: cmd5\n"
    };
    EXPECT_EQ(answer.str(), expected.str());
}

TEST (bulk, StaticInput_2){
    stringstream input {
            "cmd1\n"
            "cmd2\n"
            "cmd3\n"
            "cmd4\n"
            "cmd5\n"
    };
    int NUMBERS = 9;
    Bulk blk;
    std::stringstream answer;

    blk.SetController(std::make_shared<BulkController>());
    blk.SetBulkModel(input);
    blk.SubscribeLogger("Console_Outputer", std::make_shared<ConsoleLogger>(answer));
    blk.run(NUMBERS);


    std::stringstream expected {
            "bulk: cmd1, cmd2, cmd3, cmd4, cmd5\n"
    };
    EXPECT_EQ(answer.str(), expected.str());
}

TEST (bulk, StaticInput_3){
    stringstream input {
            "cmd1\n"
            "cmd2\n"
            "cmd3\n"
            "cmd4\n"
            "cmd5\n"
            "{\n"
            "{\n"
            "}\n"
            "}\n"
            "cmd6\n"
    };
    int NUMBERS = 9;
    Bulk blk;
    std::stringstream answer;

    blk.SetController(std::make_shared<BulkController>());
    blk.SetBulkModel(input);
    blk.SubscribeLogger("Console_Outputer", std::make_shared<ConsoleLogger>(answer));
    blk.run(NUMBERS);


    std::stringstream expected {
            "bulk: cmd1, cmd2, cmd3, cmd4, cmd5\n"
            "bulk: cmd6\n"
    };
    EXPECT_EQ(answer.str(), expected.str());
}