cmake_minimum_required(VERSION 3.28.3)
project(GBEm VERSION 0.1.0 LANGUAGES C CXX)

add_executable(GBEm src/main.c)

target_include_directories(GBEm PRIVATE src/)



