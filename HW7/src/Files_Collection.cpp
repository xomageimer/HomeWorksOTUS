#include "Files_Collection.h"

void Files_Collection::set_hasher(const Files_Collection::HASH & h) {
    cur_hasher = h;
}

void Files_Collection::set_path(const boost::filesystem::path &p) {
    checksum.emplace(std::piecewise_construct, std::forward_as_tuple(p), std::forward_as_tuple());
}

void Files_Collection::Search_Duplicate() {
    if (checksum.size() <= 1){
        return;
    }

    std::ifstream file_1;
    std::ifstream file_2;

    file_1.rdbuf()->pubsetbuf(0, 0);
    file_2.rdbuf()->pubsetbuf(0, 0);

    auto duple = checksum.begin();
    while (duple != checksum.end()){
        for (auto check = next(duple); check != checksum.end();){
            if (Accum_Hashsum(duple, check, file_1, file_2)){
                files[duple->first].push_back(check->first);
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
    is1.open(main->first.string());
    is2.open(second->first.string());

    is1.seekg(main->second.size() * block_size, std::ios::beg);
    is2.seekg(second->second.size() * block_size, std::ios::beg);

    std::string buffer_1(block_size, '\0');
    std::string buffer_2(block_size, '\0');

    while (!is1.eof() || !is2.eof()) {
        if (main->second.size() < second->second.size()) {
            is1.read(&buffer_1[0], block_size);
            main->second.push_back(Hasher(&buffer_1[0]));
        } else if (main->second.size() > second->second.size()) {
            is2.read(&buffer_2[0], block_size);
            second->second.push_back(Hasher(&buffer_2[0]));
        } else {
            is1.read(&buffer_1[0], block_size);
            is2.read(&buffer_2[0], block_size);
            main->second.push_back(Hasher(&buffer_1[0]));
            second->second.push_back(Hasher(&buffer_2[0]));
        }

        if (!std::equal(main->second.begin(),
                        main->second.begin() + ((main->second.size() > second->second.size())
                                                ? second->second.size()
                                                : main->second.size())
                , second->second.begin())) {
            auto s = ((main->second.size() > second->second.size())
                      ? second->second.size()
                      : main->second.size());
            is1.close();
            is2.close();
            return false;
        }
    }
    is1.close();
    is2.close();
    if (main->second == second->second) {
        return true;
    } else
        return false;
}

size_t Files_Collection::Hasher(char * mas) {
    switch (cur_hasher) {
        case HASH::MD5 : {
            boost::uuids::detail::md5 hash;
            hash.process_bytes(mas, block_size);
            boost::uuids::detail::md5::digest_type result;
            hash.get_digest(result);
            size_t res;
            for (auto & d : result){
                res += d;
            }
            return res;
        }
        case HASH::CRC32 : {
            boost::crc_32_type result;
            result.process_bytes(mas, block_size);
            return result.checksum();
        }
    }
}

void Files_Collection::Output() const {
    for (auto & i : files){
        std::cout << i.first << std::endl;
        for (auto & j : i.second){
            std::cout << j << std::endl;
        }
        std::cout << std::endl;
    }
}

