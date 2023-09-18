cmake_minimum_required(VERSION 3.0)
project(unnamed_c_jam_project LANGUAGES C)

set(CMAKE_C_STANDARD 11)

# TODO: Add SDL submodule!!
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(unnamed_c_jam_project src/main.c)
target_link_libraries(unnamed_c_jam_project SDL2main SDL2)
