//
// Created by xomag on 14.07.2020.
//

#ifndef OTUSHOMEWORKS_IP_FILTER_H
#define OTUSHOMEWORKS_IP_FILTER_H
#include <algorithm>
#include <sstream>
// ("",  '.') -> [""]
// ("11", '.') -> ["11"]
// ("..", '.') -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]
std::vector<std::string> split(const std::string &str, char d)
{
    std::vector<std::string> r;

    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while(stop != std::string::npos)
    {
        r.push_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
}

std::vector<int> toInt(std::vector<std::string> && vstr){
    std::stringstream ss;
    std::vector<int> vint;
    for (auto & el : vstr) {
        ss << el;
        int tmp;
        ss >> tmp;
        ss.clear();
        vint.push_back(tmp);
    }
    return vint;
}

std::vector<int> toInt(std::vector<int> && vint){
    return std::move(vint);
}

void sort_ip(std::vector<std::vector<int>>& ip_pool){
    sort(ip_pool.begin(), ip_pool.end(), std::greater<std::vector<int>>());
}

void print_ip(std::vector<std::vector<int>>& ip_pool){
    for(std::vector<std::vector<int> >::const_iterator ip = ip_pool.cbegin(); ip != ip_pool.cend(); ++ip)
    {
        for(std::vector<int>::const_iterator ip_part = ip->cbegin(); ip_part != ip->cend(); ++ip_part)
        {
            if (ip_part != ip->cbegin())
            {
                std::cout << ".";

            }
            std::cout << *ip_part;
        }
        std::cout << std::endl;
    }
}

template <typename ...args>
std::vector<std::vector<int>> filter_by(const std::vector<std::vector<int>> & vint, args... t){
    std::vector<std::vector<int>> vfilt;
    std::vector<int> arguments = {t...};
    std::copy_if(vint.begin(), vint.end(), std::back_inserter(vfilt), [&arguments](auto & v){
        return std::equal(v.begin(), v.begin() + arguments.size(), arguments.begin());
    });
    return vfilt;
}

bool one_of(const std::vector<int> & vi, int t){
    for (auto & p : vi){
        if (p == t) return true;
    }
    return false;
}

std::vector<std::vector<int>> filter_ant_of(const std::vector<std::vector<int>> & vint, int t){
    std::vector<std::vector<int>> vfilt;
    std::copy_if(vint.begin(), vint.end(), std::back_inserter(vfilt), [t](auto & v){
        return one_of(v, t);
    });
    return vfilt;
}

#endif //OTUSHOMEWORKS_IP_FILTER_H
