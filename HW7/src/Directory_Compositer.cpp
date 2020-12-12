#include "Directory_Compositer.h"

void Directory_Compositer::Set_Directory(const std::string &path_) {
    boost::tokenizer<boost::char_separator<char>>
            tokenizer{path_, boost::char_separator<char>{"\""}};

    boost::filesystem::directory_iterator begin(boost::filesystem::path()/(*tokenizer.begin()));
    include_dirs.emplace_back(boost::filesystem::absolute(boost::filesystem::path()/path_));
}

void Directory_Compositer::Unset_Directory(const std::string &path_) {
    boost::tokenizer<boost::char_separator<char>>
            tokenizer{path_, boost::char_separator<char>{"\""}};

    boost::filesystem::directory_iterator begin(boost::filesystem::path()/(*tokenizer.begin()));
    exclude_dirs.emplace_back(boost::filesystem::absolute(boost::filesystem::path()/path_));
}

void Directory_Compositer::Set_Hasher(const Files_Collection::HASH &hash) {
    fc.set_hasher(hash);
}

void Directory_Compositer::run() {
    std::vector<boost::filesystem::path> tmp;
    for (auto dir = include_dirs.begin(); dir != include_dirs.end(); dir++){
        boost::filesystem::directory_iterator begin(*dir);
        boost::filesystem::directory_iterator end;

        for (; begin != end; ++begin) {
            boost::filesystem::file_status fs = boost::filesystem::status(*begin);

            switch (fs.type()) {
                case boost::filesystem::regular_file:
                    if (boost::regex_match(begin->path().filename().string(), globParser()) && boost::filesystem::file_size(begin->path()) >= minimum_size){
                        fc.set_path(begin->path());
                        if (fc.get_files_count() >= files_count)
                            fc.Search_Duplicate();
                    }
                    break;
                case boost::filesystem::directory_file:
                    if (find(exclude_dirs.begin(), exclude_dirs.end(), *begin) == exclude_dirs.end() && is_go_around) {
                        tmp.push_back(begin->path());
                    }
                    break;
                default:
                    break;
            }
        }
        if (is_go_around && !tmp.empty()){
            auto it = include_dirs.insert(include_dirs.end(), tmp.begin(), tmp.end());
            tmp.clear();
        }
    }
    fc.Search_Duplicate();
    fc.Output();
}
