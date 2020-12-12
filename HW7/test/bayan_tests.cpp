#include <memory>
#include <fstream>

#include <gtest/gtest.h>
#include <boost/filesystem/operations.hpp>

#include "Command_interpreter.h"
#include "Directory_Compositer.h"

namespace po = boost::program_options;

struct Bayan : public testing::Test {
protected:
    void Add_Files(const std::vector<std::string> & vec, const std::string & path ){
        size_t text_num = 0;
        for (auto & i : vec) {
            if (i == "skip")
                if (text_num > texts.size()){
                    text_num = 0;
                } else
                    text_num++;
            else
            {
                std::ofstream file(path + "/" + i);
                file << texts[text_num];
                assert(file);
                file.close();
            }
        }
    }

    void Add_Texts(const std::string & str){
        texts.push_back(str);
    }

    void SetUp()
    {
        ci = std::make_unique<Command_interpreter>();
        (*ci)->add_options()
                ("help,h", "help info")
                ("hasher", po::value<std::string>()->default_value("crc32"), "Hash function (md5, crc32)")
                ("include,i", po::value<std::vector<std::string>>()->multitoken()->
                        zero_tokens()->composing(), "Include specified directory")
                ("exclude,e", po::value<std::vector<std::string>>()->multitoken()->
                        zero_tokens()->composing(), "Exclude specified directory")
                ("fsize", po::value<size_t>()->default_value(1), "Minimum size for file to include")
                ("sblock,s", po::value<size_t>()->default_value(5), "Size of block to Hash")
                ("scan", po::value<size_t>()->default_value(0),
                 "Level of scan (0 - only this level, 1 - all children levels)")
                ("mask,m", po::value<std::string>(), "Mask of file. For example: [a-z]*.?onfig")
                ("count, c",  po::value<size_t>()->default_value(10), "The number at which the search for unique files will start.")
                ;

        texts = {{"Александр\n"
                  "Даниил\n"
                  "Максим\n"
                  "Дмитрий\n"
                  "Михаил\n"
                  "Иван\n"
                  "Матвей\n"
                  "Роман\n"
                  "Кирилл\n"
                  "Егор\n"
                  " \n"
                  "Илья\n"
                  "Тимофей\n"
                  "Ярослав\n"
                  "Никита\n"
                  "Алексей\n"
                  "Павел\n"
                  "Константин\n"
                  "Марк\n"
                  "Сергей\n"
                  "Артем\n"
                  " \n"
                  "Владимир\n"
                  "Лев\n"
                  "Арсений\n"
                  "Владислав\n"
                  "Андрей\n"
                  "Евгений\n"
                  "Федор\n"
                  "Степан\n"
                  "Николай\n"
                  "Макар\n"
                  " \n"
                  "Глеб\n"
                  "Вячеслав\n"
                  "Денис\n"
                  "Богдан\n"
                  "Мирон\n"
                  "Виктор\n"
                  "Олег\n"
                  "Георгий\n"
                  "Тимур\n"
                  "Платон\n"
                  " \n"
                  "Юрий\n"
                  "Игорь\n"
                  "Захар\n"
                  "Семен\n"
                  "Давид\n"
                  "Савелий\n"
                  "Роберт\n"
                  "Дамир\n"
                  "Артемий\n"
                  "Святослав"},
                 {"1. Мосты разводят не для кораблей. То, что по ночам выходит на улицы Васильевского острова — без глаз, без лица и без тела — не может перебраться на другую сторону.\n"
                  "\n"
                  "1.1. Если вы остались на Васильевском острове на ночь — не смотрите в окна. Оно решит, что вы зовёте его в гости. Что случается после того, когда оно стучится в двери — никто не знает, потому что свидетелей не остаётся.\n"
                  "\n"
                  "1.2. Даже днём вы можете ощутить его присутствие, если неожиданно поняли, что запутались в линиях и улицах, и три раза выходите к одному и тому же дому. Тогда поворачивайте обратно и идите прямо. Не смотрите назад. Ему быстро наскучит, если вы перестанете чувствовать тревогу.\n"
                  "\n"
                  "2. Если вы думаете, что самая опасная часть Петербурга — это старый фонд с домами-колодцами, то вы ошибаетесь. Местные духи уже привыкли к людям. А те, с окраин, кого недавно потревожили строители, ещё совсем дикие.\n"
                  "\n"
                  "2.1. Им негде скрыться: нет деревьев, овраги засыпаны, озёра осушены. Они обживаются в подвалах и пережидают дни в подземных парковках. А по ночам принимают человеческий облик и подолгу стоят на одном месте в безлюдных дворах, глядя на свет фонарей.\n"
                 }
        };
    }
    std::unique_ptr<Command_interpreter> ci;
    std::vector<std::string> texts;

};

TEST_F(Bayan, test_1) {
    std::string main_directory = "Bayan_tests/test_1";
    boost::system::error_code error;
    boost::filesystem::create_directories(main_directory, error);
    if (!error) {
        std::stringstream answer;
        std::stringstream expected {
            "\"boys.txt\"\n"
            "\"names.txt\"\n"
            "\n"
            "\"horror_stories.txt\"\n"
            "\"stories.txt\"\n"
            "\n"
        };
        std::string commnad_i = "-i" + main_directory;
        std::vector<const char *> argv{
            "--hasher=md5",
            "-s5",
            commnad_i.c_str(),
            "--scan=1",
            "-m*.txt"
        };
        std::vector<std::string> files{
                "names.txt",
                "boys.txt",
                "skip",
                "horror_stories.txt",
                "stories.txt",
                "skip",
                "simple.txt"
        };
        Add_Texts("hello world!\n");
        Add_Files(files, main_directory);
        Directory_Compositer d(answer, false);


        ci->parse_and_execute(argv.size(), argv.data(), d);


        ASSERT_STREQ(answer.str().c_str(), expected.str().c_str());
    }
}

TEST_F(Bayan, test_2) {
    std::string main_directory = "Bayan_tests/test_2";
    boost::system::error_code error;
    boost::filesystem::create_directories(main_directory, error);
    if (!error) {
        std::stringstream answer;
        std::stringstream expected {
                "\"boys.txt\"\n"
                "\"names.txt\"\n"
                "\n"
                "\"horror_stories.txt\"\n"
                "\"stories.txt\"\n"
                "\n"
        };
        std::string commnad_i = "-i" + main_directory;
        std::vector<const char *> argv{
                "--hasher=crc32",
                "-s5",
                commnad_i.c_str(),
                "--scan=1",
                "-m*.txt"
        };
        std::vector<std::string> files{
                "names.txt",
                "boys.txt",
                "skip",
                "horror_stories.txt",
                "stories.txt",
                "skip",
                "simple.txt"
        };
        Add_Texts("hello world!\n");
        Add_Files(files, main_directory);
        Directory_Compositer d(answer, false);


        ci->parse_and_execute(argv.size(), argv.data(), d);


        ASSERT_STREQ(answer.str().c_str(), expected.str().c_str());
    }
}

TEST_F(Bayan, test_3) {
    std::string main_directory = "Bayan_tests/test_3";
    boost::system::error_code error;
    boost::filesystem::create_directories(main_directory, error);
    if (!error) {
        std::stringstream answer;
        std::stringstream expected {
                "\"boys.txt\"\n"
                "\"names.txt\"\n"
                "\n"
                "\"simple.txt\"\n"
                "\"simple.txt\"\n"
                "\n"
                "\"horror_stories.txt\"\n"
                "\"stories.txt\"\n"
                "\n"
        };
        std::string commnad_i = "-i" + main_directory;
        std::vector<const char *> argv{
                "--hasher=crc32",
                "-s45",
                commnad_i.c_str(),
                "--scan=1",
                "-m*.txt"
        };

        boost::filesystem::create_directories(main_directory + "/child");
        std::vector<std::string> files{
                "child/names.txt",
                "boys.txt",
                "skip",
                "horror_stories.txt",
                "stories.txt",
                "skip",
                "simple.txt",
                "child/simple.txt"
        };
        Add_Texts("hello world!\n");
        Add_Files(files, main_directory);
        Directory_Compositer d(answer, false);


        ci->parse_and_execute(argv.size(), argv.data(), d);


        ASSERT_STREQ(answer.str().c_str(), expected.str().c_str());
    }
}

TEST_F(Bayan, test_4) {
    std::string main_directory = "Bayan_tests/test_4";
    boost::system::error_code error;
    boost::filesystem::create_directories(main_directory, error);
    if (!error) {
        std::stringstream answer;
        std::stringstream expected {
                "\"horror_stories.txt\"\n"
                "\"stories.txt\"\n"
                "\n"
        };
        std::string commnad_i = "-i" + main_directory;
        std::vector<const char *> argv{
                "--hasher=crc32",
                "-s5",
                "--fsize=1000",
                commnad_i.c_str(),
                "--scan=1",
                "-m*.txt"
        };

        boost::filesystem::create_directories(main_directory + "/child");
        std::vector<std::string> files{
                "child/names.txt",
                "boys.txt",
                "skip",
                "horror_stories.txt",
                "stories.txt",
                "skip",
                "simple.txt",
                "child/simple.txt"
        };
        Add_Texts("hello world!\n");
        Add_Files(files, main_directory);
        Directory_Compositer d(answer, false);


        ci->parse_and_execute(argv.size(), argv.data(), d);


        ASSERT_STREQ(answer.str().c_str(), expected.str().c_str());
    }
}