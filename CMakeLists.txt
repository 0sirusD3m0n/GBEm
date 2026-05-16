cmake_minimum_required(VERSION 3.28.3)
project(gbem VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)

file(GLOB_RECURSE SOURCES "src/*c")

add_executable(gbem ${SOURCES})

target_include_directories(gbem PRIVATE src/)



