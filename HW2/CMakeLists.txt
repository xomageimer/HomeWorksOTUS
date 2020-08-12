CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    project(allocator VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project(allocatorDEMO)
endif()

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/googletest"
        "build-googletest"
        )

add_executable(allocator src/main.cpp)

add_executable(tests src/test.cpp)

set (GTEST_INCLUDE_DIR "${gtest_SOURCE_DIR}/include")

message(${GTEST_INCLUDE_DIR})

include_directories("${gtest_SOURCE_DIR}/include")

target_link_libraries(tests PRIVATE
        ${GTEST_LIBRARY_DEBUG} ${GTEST_MAIN_LIBRARY_DEBUG} )


enable_testing(
)

add_test(
        NAME tests
        COMMAND tests
)
