cmake_minimum_required(VERSION 3.0)
project(unnamed_c_jam_project LANGUAGES C)

set(CMAKE_C_STANDARD 11)

add_subdirectory(external/SDL)
add_subdirectory(glad)

include_directories(external/stb)

add_executable(unnamed_c_jam_project src/main.c src/image.c src/read_data.c)
target_link_libraries(unnamed_c_jam_project SDL2main SDL2 ${CMAKE_DL_LIBS} glad)

if (UNIX)
    target_link_libraries(unnamed_c_jam_project m)
endif(UNIX)

file(COPY res DESTINATION ${CMAKE_BINARY_DIR})
