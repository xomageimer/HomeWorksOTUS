cmake_minimum_required(VERSION 3.5)
project(bayan)

if($ENV{TRAVIS_BUILD_NUMBER})
    project(${PROJECT_NAME}  VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project(${PROJECT_NAME}  VERSION 0.0.1)
endif()

if (WIN32)
    set (Boost_COMPILER mgw81)
    set (Boost_USE_STATIC_LIBS ON)
endif()

find_package(Boost COMPONENTS system regex filesystem program_options REQUIRED)

include_directories(${Boost_INCLUDE_DIR})

add_executable(bayan

        src/main.cpp
        src/Glob_Parser.cpp
        src/Command_interpreter.cpp
        src/Directory_Compositer.cpp
        src/Files_Collection.cpp)

add_executable(tests
        test/glob_test.cpp


        src/Glob_Parser.cpp
        src/Command_interpreter.cpp
        src/Directory_Compositer.cpp
        src/Files_Collection.cpp)

target_link_libraries(bayan
        ${Boost_LIBRARIES}
        )

target_link_libraries(tests
        ${Boost_LIBRARIES}
        )

set_target_properties(bayan tests PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        COMPILE_OPTIONS "-Wall;-Wextra;-Wpedantic;"
        )

add_subdirectory(
        "contrib/gtest"
        "googletest"
)

target_link_libraries(tests gtest gtest_main gmock)

target_include_directories(tests PRIVATE src)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT xomageimer@gmail.com)

set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

include(CPack)
include(CTest)

enable_testing()
add_test(test tests)