CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    project(allocator VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project(allocatorDEMO)
endif()

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/googletest"
        "googletest"
        )

set (GTEST_INCLUDE_DIR "${gtest_SOURCE_DIR}/include")
include_directories("${gtest_SOURCE_DIR}/include")

add_executable(allocator src/main.cpp)
add_library(tests src/test.cpp)

#find_package(gtest REQUIRED)

target_link_libraries(tests gtest gtest_main)

set_target_properties(allocator tests PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra"
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )

install(TARGETS allocator RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT xomageimer@Gmail.com)

set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

include(CPack)

