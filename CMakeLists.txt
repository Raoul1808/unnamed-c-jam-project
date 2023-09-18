cmake_minimum_required(VERSION 3.0)
project(unnamed_c_jam_project LANGUAGES C)

set(CMAKE_C_STANDARD 11)

add_subdirectory(external/SDL)

add_executable(unnamed_c_jam_project src/main.c)
target_link_libraries(unnamed_c_jam_project SDL2main SDL2)
