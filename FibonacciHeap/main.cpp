/**
 * @file main.cpp
 * @author jhjang
 * @brief
 * @version 0.1
 * @date 2021-01-20
 *
 * @copyright Copyright (c) 2021
 *
 */
#include <iostream>

#include "./FibonacciHeap.h"

int main() {
  fib::FibonacciHeap<int> f;
  for (int i = 100; i > 0; i--) {
    f.insert(i);
  }

  for (int i = 0; i <= 100; i++) {
    std::cout << f.extractMin() << "\n";
  }
}
