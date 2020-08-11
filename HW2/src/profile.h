//
// Created by xomag on 11.08.2020.
//

#ifndef OTUSHOMEWORKS_PROFILE_H
#define OTUSHOMEWORKS_PROFILE_H
#include <chrono>
#include "pch.h"

struct Log_Time{
private:
    std::string message;
    std::chrono::_V2::steady_clock::time_point start;

public:
    explicit Log_Time(std::string mes = "") : message(mes + " : "){
        start = std::chrono::steady_clock::now();
    }


    ~Log_Time() {
        auto end = std::chrono::steady_clock::now();
        std::cerr << message <<  double(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()) / 1000 << " seconds" << std::endl;
    }
};

#define unique_impl2(line) timer_num_ ## line
#define unique_impl(line) unique_impl2(line)
#define unique_id unique_impl(__LINE__)

#define Code_Timer(message) \
    Log_Time unique_id{#message}

#endif //OTUSHOMEWORKS_PROFILE_H
