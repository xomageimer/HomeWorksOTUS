cmake_minimum_required(VERSION 3.1)

project(print_ip)

if($ENV{TRAVIS_BUILD_NUMBER})
    project(print_ip VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project(print_ip VERSION 0.0.1)
endif()

add_subdirectory(
        "contrib/gtest"
        "googletest"
)

add_executable(print_ip src/main.cpp src/print_meta_ip.cpp src/pch.cpp)

add_executable(test test/print_test.cpp src/print_meta_ip.cpp src/pch.cpp)

target_link_libraries(
        test
        PRIVATE
        gtest gtest_main gmock
)

include_directories(src)

set_target_properties(print_ip PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        COMPILE_OPTIONS "-Wall;-Wextra;-Wpedantic"
        )

install(TARGETS print_ip RUNTIME DESTINATION bin)


set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT xomageimer@gmail.com)

include(CPack)

enable_testing()
add_test(NAME print_tests COMMAND test)