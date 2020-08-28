cmake_minimum_required(VERSION 3.1)

project(print_ip)

if($ENV{TRAVIS_BUILD_NUMBER})
    project(print_ip VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project(print_ip VERSION 0.0.1)
endif()

configure_file(version.h.in ${PROJECT_SOURCE_DIR}/version.h)

add_executable(print_ip src/print_meta_ip.cpp src/pch.cpp)

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

set(CPACK_PACKAGE_CONTACT kuskov.stanislav@gmail.com)

include(CPack)