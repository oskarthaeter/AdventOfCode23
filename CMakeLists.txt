cmake_minimum_required(VERSION 3.27)
project(AdventOfCode2023)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
include_directories(./common)

# ADD SUBDIRECTORY
add_subdirectory(day01)
add_subdirectory(day02)
add_subdirectory(day03)
add_subdirectory(day04)
add_subdirectory(day05)

# Common utilities (if any)
add_library(common STATIC common/utils.cpp)
