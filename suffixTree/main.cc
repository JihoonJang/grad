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

#include "./SuffixTree.h"

int main() {
  std::cin.tie(NULL);
  std::cout.tie(NULL);
  std::ios::sync_with_stdio(false);

  int size;
  char s[200001];

  std::cin >> size >> s;

  SuffixTree st(s, size);
  std::cout << st.getSizeOfLargestRepeatedSubstr();
}
