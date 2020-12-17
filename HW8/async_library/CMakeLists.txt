cmake_minimum_required(VERSION 3.9)
project(async)

set(CMAKE_CXX_STANDARD 17)

add_library(async SHARED async.cpp async.h

        ../src/Bulk.cpp ../src/BulkController/Controller.cpp ../src/BulkModel/BulkReader.cpp ../src/BulkView/OutputManager.cpp ../src/TimeManager/TimeManager.cpp
        )

include_directories(async PRIVATE ../src/BulkController ../src/BulkModel ../src/BulkView ../src/TimeManager)

add_executable(async_main main.cpp)

target_link_libraries(async_main async)