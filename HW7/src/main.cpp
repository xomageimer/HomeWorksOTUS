#include <iostream>
#include <cassert>
#include <memory>

#include "Command_interpreter.h"

namespace po = boost::program_options;

// TODO починить md5, понять почему первый aas читаеся неверно в буффер и протестировать остальной функционал

int main(int argc, const char **argv) {
    try {
        std::unique_ptr<const Command_interpreter> g_ci;
        {
            std::unique_ptr<Command_interpreter> ci = std::make_unique<Command_interpreter>();
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
                    ;
            g_ci = std::move(ci);
        }
        g_ci->parse(argc, argv);
        Directory_Compositer dc;
        g_ci->execute(dc);
    } catch (std::exception & e) {
        std::cout << "SOMETHING ERROR!" << std::endl;
        std::cerr << e.what() << std::endl;
    }
    return EXIT_SUCCESS;
}