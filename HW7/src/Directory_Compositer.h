#ifndef OTUSHOMEWORKS_DIRECTORY_COMPOSITER_H
#define OTUSHOMEWORKS_DIRECTORY_COMPOSITER_H

#include <iostream>
#include <boost/filesystem/operations.hpp>
#include <boost/container/stable_vector.hpp>
#include <boost/tokenizer.hpp>
#include <boost/regex.hpp>
#include <cassert>
#include <fstream>

#include "Files_Collection.h"
#include "Glob_Parser.h"


struct Directory_Compositer {
public:
    Directory_Compositer(std::ostream & o = std::cout, bool full_name = true) : out(o), is_full_name(full_name) {}
    void Set_Directory(const std::string & path_);
    void Unset_Directory(const std::string & path_);

    void Set_Hasher(const Files_Collection::HASH & hash);

    inline void Set_Is_Go_Around(bool b){
        is_go_around = b;
    }

    inline void Set_Minimum_File_Size(size_t size){
        minimum_size = size;
    }
    inline void Set_Block_Size(size_t size){
        fc.set_block_size(size);
    }

    inline void Set_Glob(const Glob_Parser & gb){
        globParser = gb;
    }

    inline void Set_Files_Count(size_t size){
        files_count = size;
    }
    
    void run();

private:
    std::ostream & out;
    bool is_full_name;

    boost::container::stable_vector<boost::filesystem::path> include_dirs;
    std::vector<boost::filesystem::path> exclude_dirs;

    Glob_Parser globParser;
    bool is_go_around;
    size_t minimum_size;
    size_t files_count;
    Files_Collection fc;
};


#endif //OTUSHOMEWORKS_DIRECTORY_COMPOSITER_H
