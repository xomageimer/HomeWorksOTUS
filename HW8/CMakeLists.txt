cmake_minimum_required(VERSION 3.1)

set(PROJECT_NAME_ bulk_mt)
if($ENV{TRAVIS_BUILD_NUMBER})
    project(${PROJECT_NAME_}  VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project(${PROJECT_NAME_}  VERSION 0.0.1)
endif()

add_executable(${PROJECT_NAME_}

        src/main.cpp
        src/Bulk.cpp

        src/BulkController/Controller.cpp
        src/BulkModel/BulkReader.cpp
        src/BulkView/OutputManager.cpp
        src/TimeManager/TimeManager.cpp
        src/main.cpp)

target_include_directories(${PROJECT_NAME_} PRIVATE src/BulkController src/BulkModel src/BulkView src/TimeManager)

add_executable(tests src/Bulk.cpp test/tests.cpp src/BulkController/Controller.cpp src/BulkModel/BulkReader.cpp src/BulkView/OutputManager.cpp src/TimeManager/TimeManager.cpp)

set_target_properties(${PROJECT_NAME_} tests PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        COMPILE_OPTIONS "-Wall;-Wextra;-Wpedantic;"
        )

target_include_directories(tests PRIVATE src src/BulkController src/BulkModel src/BulkView src/TimeManager)

add_subdirectory(
        "contrib/gtest"
        "googletest"
)

target_link_libraries(tests PRIVATE gtest gtest_main gmock)

install(TARGETS ${PROJECT_NAME_} RUNTIME DESTINATION bin)

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