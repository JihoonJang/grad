SET(CMAKE_C_COMPILER g++)

add_compile_options(-std=c++17)

ADD_EXECUTABLE(main main.cc SuffixTree.cc)