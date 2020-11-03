#ifndef OTUSHOMEWORKS_TIMEMANAGER_H
#define OTUSHOMEWORKS_TIMEMANAGER_H

#include <ctime>
#include <iostream>

#define DEBUG_LOGGER

struct TimeManager {
public:
    static inline std::time_t last_time;
    [[nodiscard]] inline std::time_t GetUnixTime() const {
        auto time = std::time(nullptr);
    #ifdef DEBUG_LOGGER
            last_time = time;
    #endif
        return time;
    }

};


#endif //OTUSHOMEWORKS_TIMEMANAGER_H
