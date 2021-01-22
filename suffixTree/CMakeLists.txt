project(suffixTree)

cmake_minimum_required(VERSION 3.16)

set(CMAKE_C_COMPILER g++)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
add_compile_options(-std=c++17)

ADD_EXECUTABLE(main.out main.cc SuffixTree.cc)