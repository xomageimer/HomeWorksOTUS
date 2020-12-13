#ifndef OTUSHOMEWORKS_FILES_COLLECTION_H
#define OTUSHOMEWORKS_FILES_COLLECTION_H

#include <vector>
#include <set>
#include <iostream>
#include <fstream>
#include <boost/filesystem/operations.hpp>
#include <uuid/detail/md5.hpp>
#include <boost/crc.hpp>

struct File;

using file_hashsum = std::set<File>;
using non_unique_files = std::map<File,  std::vector<File>>;

struct File {
public:
    explicit File(const boost::filesystem::path & p, size_t size);

    [[nodiscard]] inline std::string GetFullName() const { return full_name;}

    [[nodiscard]] inline std::string GetShortName() const { return short_name;}

    [[nodiscard]] inline size_t GetSize() const { return file_size;}

    [[nodiscard]] inline const std::vector<size_t> & GetHashsum() const {return hashs;}

    inline void SetHash(size_t s) const {  hashs.push_back(s);}

    bool operator<(const File &f) const {
        return this->full_name < f.full_name;
    }

private:
    std::string full_name;
    std::string short_name;
    std::size_t file_size {0};
    mutable std::vector<size_t> hashs {};
};

struct Files_Collection {
public:
    enum class HASH{
        MD5,
        CRC32
    };

    void set_hasher(const HASH &);

    void set_path(const boost::filesystem::path & p, size_t size);

    inline void set_block_size(size_t size){ block_size = size;}

    inline size_t get_files_count() const{ return checksum.size(); }

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
