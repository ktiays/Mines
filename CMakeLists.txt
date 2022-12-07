cmake_minimum_required(VERSION 3.24)
project(Mines)

set(CMAKE_CXX_STANDARD 20)

add_executable(Mines main.cpp minefield.hpp vector_2d.hpp minefield.cpp)
