#include "Files_Collection.h"

#include <sstream>
#include <boost/algorithm/hex.hpp>

void Files_Collection::set_hasher(const Files_Collection::HASH & h) {
    cur_hasher = h;
}

void Files_Collection::set_path(const boost::filesystem::path &p, size_t size) {
    checksum.emplace(p, size);
}

void Files_Collection::Search_Duplicate() {
    if (checksum.size() <= 1){
        return;
    }

    std::ifstream file_1;
    std::ifstream file_2;

    file_1.rdbuf()->pubsetbuf(nullptr, 0);
    file_2.rdbuf()->pubsetbuf(nullptr, 0);

    auto duple = checksum.begin();
    while (duple != checksum.end()){
        for (auto check = next(duple); check != checksum.end();){
            if (Accum_Hashsum(duple, check, file_1, file_2)){
                files[*duple].push_back(*check);
                check = checksum.erase(check);
            } else {
                check++;
            }
        }
        duple++;
    }
}

bool Files_Collection::Accum_Hashsum(file_hashsum::iterator main,
                                     file_hashsum::iterator second,
                                     std::ifstream &is1,
                                     std::ifstream &is2){
    if (main->GetSize() != second->GetSize())
        return false;


    is1.open(main->GetFullName());
    is2.open(second->GetFullName());

    std::string buffer_1(block_size, '\0');
    std::string buffer_2(block_size, '\0');

    if (is1.is_open() && is2.is_open()) {
        while (!is1.eof() || !is2.eof()) {
            if(!is1.eof())
                is1.seekg(main->GetHashsum().size() * block_size, std::ios::beg);
            if (!is2.eof())
                is2.seekg(second->GetHashsum().size() * block_size, std::ios::beg);

            if (main->GetHashsum().size() < second->GetHashsum().size()) {
                is1.read(&buffer_1[0], block_size);
                main->SetHash(Hasher(&buffer_1[0]));
            } else if (main->GetHashsum().size() > second->GetHashsum().size()) {
                is2.read(&buffer_2[0], block_size);
                second->SetHash(Hasher(&buffer_2[0]));
            } else {
                is1.read(&buffer_1[0], block_size);
                is2.read(&buffer_2[0], block_size);
                main->SetHash(Hasher(&buffer_1[0]));
                second->SetHash(Hasher(&buffer_2[0]));
            }
            buffer_1.assign(block_size, '\0');
            buffer_2.assign(block_size, '\0');

            if (!std::equal(main->GetHashsum().begin(),
                            main->GetHashsum().begin() + ((main->GetHashsum().size() > second->GetHashsum().size())
                                                    ? second->GetHashsum().size()
                                                    : main->GetHashsum().size()), second->GetHashsum().begin())) {
                is1.close();
                is2.close();
                return false;
            }
        }
        is1.close();
        is2.close();
        if (main->GetHashsum() == second->GetHashsum()) {
            return true;
        } else
            return false;
    }
    else return false;
}

std::string toString(const boost::uuids::detail::md5::digest_type &digest)
{
    const auto charDigest = reinterpret_cast<const char *>(&digest);
    std::string result;
    boost::algorithm::hex(charDigest, charDigest + sizeof(boost::uuids::detail::md5::digest_type), std::back_inserter(result));
    return result;
}

size_t Files_Collection::Hasher(char * mas) {
    switch (cur_hasher) {
        case HASH::MD5 : {
            boost::uuids::detail::md5 hash;
            hash.process_bytes(mas, block_size);
            boost::uuids::detail::md5::digest_type result;
            hash.get_digest(result);
            std::stringstream res (toString(result));
            size_t digit;
            res >> digit;
            return digit;
        }
        case HASH::CRC32 : {
            boost::crc_32_type result;
            result.process_bytes(mas, block_size);
            return result.checksum();
        }
        default:
            return 0;
    }
}

void Files_Collection::Output(std::ostream & out, bool is_full) const {
    for (auto & i : files){
        out << ((is_full) ? i.first.GetFullName() : i.first.GetShortName()) << std::endl;
        for (auto & j : i.second){
            out << ((is_full) ? j.GetFullName() : j.GetShortName()) << std::endl;
        }
        out << std::endl;
    }
}

File::File(const boost::filesystem::path &p, size_t size) {
    full_name = p.string();
    short_name = p.filename().string();
    file_size = size;
}
