cmake_minimum_required(VERSION 3.5)
project(bayan)

if (WIN32)
    set (Boost_COMPILER mgw81)
    set (Boost_USE_STATIC_LIBS ON)
endif()

find_package(Boost COMPONENTS system regex filesystem program_options REQUIRED)

include_directories(${Boost_INCLUDE_DIR})

set(CMAKE_CXX_STANDARD 17)

add_executable(bayan

        src/main.cpp
        src/Glob_Parser.cpp
        src/Command_interpreter.cpp
        src/Directory_Compositer.cpp
        src/Files_Collection.cpp
        )

target_link_libraries(bayan
        ${Boost_LIBRARIES}
        )
