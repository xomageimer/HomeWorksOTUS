#include "Command_interpreter.h"

namespace po = boost::program_options;

po::options_description * Command_interpreter::operator->() {
    return &m_opt_desc;
}

const boost::program_options::options_description *Command_interpreter::operator->() const {
    return &m_opt_desc;
}

void Command_interpreter::parse(int argc, const char ** argv) const {
    try {
        po::store(po::parse_command_line(argc, argv, m_opt_desc), m_vm);
        po::notify(m_vm);
    } catch (std::exception & exc){
        std::cerr << exc.what() << std::endl;
    }
}

void Command_interpreter::execute(Directory_Compositer & directoryCompositer) const {
    if (m_vm.count("help"))
        std::cout << m_opt_desc << '\n';
    else {
        if (m_vm.count("hasher")) {
            if (m_vm["hasher"].as<std::string>() == "crc32") {
                directoryCompositer.Set_Hasher(Files_Collection::HASH::CRC32);
            } else {
                directoryCompositer.Set_Hasher(Files_Collection::HASH::MD5);
            }
        }
        if (m_vm.count("include"))
        {
            for (auto & path : m_vm["include"].as<std::vector<std::string>>()){
                directoryCompositer.Set_Directory(path);
            }
        } else {
            directoryCompositer.Set_Directory((boost::filesystem::current_path()).string());
        }
        if (m_vm.count("exclude")){
            for (auto & path : m_vm["exclude"].as<std::vector<std::string>>()){
                directoryCompositer.Unset_Directory(path);
            }
        }
        if (m_vm.count("fsize")){
            directoryCompositer.Set_Minimum_File_Size(m_vm["fsize"].as<size_t>());
        }
        if (m_vm.count("sblock")){
            directoryCompositer.Set_Block_Size(m_vm["sblock"].as<size_t>());
        }
        if (m_vm.count("scan")){
            directoryCompositer.Set_Is_Go_Around(static_cast<bool>(m_vm["scan"].as<size_t>()));
        }
        if (m_vm.count("mask")){
            directoryCompositer.Set_Glob(Glob_Parser(m_vm["mask"].as<std::string>()));
        }
    }
    directoryCompositer.run();
}

void Command_interpreter::parse_and_execute(int argc, const char **argv, Directory_Compositer & dc) const {
    parse(argc, argv);
    execute(dc);
}
