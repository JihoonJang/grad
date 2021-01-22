/**
 * @file main.cc
 * @author jhjang
 * @brief
 * @version 0.1
 * @date 2021-01-18
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <random>

#include "./SuffixTree.h"

int main() {
  std::cin.tie(NULL);
  std::cout.tie(NULL);
  std::ios::sync_with_stdio(false);

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<char> dis('a', 'z');

  int size;
  char s[200001];

  size = 200000;
  for (int i = 0; i < size; i++) {
    s[i] = dis(gen);
  }

  SuffixTree st(s, size);
  std::cout << st.getSizeOfLargestRepeatedSubstr();
}
