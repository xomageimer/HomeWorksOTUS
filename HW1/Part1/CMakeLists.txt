cmake_minimum_required(VERSION 3.2)

IF(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    message("TRAVIS ENV VARIABLE DETECTED!")
    project(helloworld VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
    find_package(Boost COMPONENTS unit_test_framework REQUIRED)
ELSE()
    project(helloworld VERSION 0.0.1)
    set(BOOST_ROOT C:/Users/xomag/Desktop/boost_1_71_0/boost_1_71_0)
    find_package(Boost REQUIRED)
ENDIF()

configure_file(version.h.in ${PROJECT_SOURCE_DIR}/version.h)

add_executable(helloworld_cli main.cpp)
add_library(helloworld lib.cpp)
add_executable(test_version test_version.cpp)

set_target_properties(helloworld_cli helloworld test_version PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra"
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )

set_target_properties(test_version PROPERTIES
        COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
        INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
        )

target_include_directories(helloworld
        PRIVATE "${CMAKE_BINARY_DIR}"
        )

target_link_libraries(helloworld_cli
        helloworld)

target_link_libraries(test_version
        ${Boost_LIBRARIES}
        helloworld
        )

install(TARGETS helloworld_cli RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT xomageimer@Gmail.com)

set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

include(CPack)

enable_testing()

add_test(helloworld_tests test_version)