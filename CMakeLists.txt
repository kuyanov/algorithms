cmake_minimum_required(VERSION 3.17)
project(algorithms)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS_ASAN "-g -fsanitize=address,undefined")

add_executable(Algorithms main.cpp)