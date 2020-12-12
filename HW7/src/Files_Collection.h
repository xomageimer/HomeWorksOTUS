#ifndef OTUSHOMEWORKS_FILES_COLLECTION_H
#define OTUSHOMEWORKS_FILES_COLLECTION_H

#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <boost/filesystem/operations.hpp>
#include <uuid/detail/md5.hpp>
#include <boost/crc.hpp>

using file_hashsum = std::map<boost::filesystem::path, std::vector<size_t>>;
using non_unique_files = std::map<boost::filesystem::path,  std::vector<boost::filesystem::path>>;

struct Files_Collection {
public:
    enum class HASH{
        MD5,
        CRC32
    };

    void set_hasher(const HASH &);
    void set_path(const boost::filesystem::path & p);
    inline void set_block_size(size_t size){
        block_size = size;
    }
    inline size_t get_files_count() const{
        return checksum.size();
    }

    void Search_Duplicate();

    void Output(std::ostream & out, bool) const;

private:
    bool Accum_Hashsum(file_hashsum::iterator main, file_hashsum::iterator second,
                       std::ifstream & is1, std::ifstream & is2);
    size_t Hasher(char *);

    file_hashsum checksum;
    non_unique_files files;

    size_t block_size;
    HASH cur_hasher;
};


#endif //OTUSHOMEWORKS_FILES_COLLECTION_H
