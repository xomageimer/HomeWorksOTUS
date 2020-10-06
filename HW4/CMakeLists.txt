cmake_minimum_required (VERSION 3.1)

project(Editor)

if($ENV{TRAVIS_BUILD_NUMBER})
    project(Editor VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project(Editor VERSION 0.0.1)
endif()

set(CMAKE_CXX_STANDARD 17)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(contrib/glfw)
include_directories("glfw_repo/include")
include_directories("dependence/glad/include")

add_executable(Editor src/main.cpp src/Shader.cpp src/Primitives.cpp src/Widgets.cpp ${SOURCE_FILES} dependence/glad/src/glad.c)

target_include_directories(Editor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/dependence/glm-0.9.9.8)

find_package(OpenGL REQUIRED)

target_link_libraries(Editor  OpenGL::GL -lopengl32)
target_link_libraries(Editor  glfw)

install(TARGETS Editor RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT xomageimer@gmail.com)

include(CPack)