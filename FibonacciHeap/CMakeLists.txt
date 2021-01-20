project(FibonacciHeap)

cmake_minimum_required(VERSION 3.16)

SET(CMAKE_C_COMPILER g++)

add_compile_options(-std=c++17)

ADD_EXECUTABLE(main.out main.cpp)