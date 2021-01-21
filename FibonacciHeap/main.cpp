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
#include <random>
#define NDEBUG
#include "./FibonacciHeap.h"

int main() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dis(0, 999999);

  fib::FibonacciHeap<int> f;
  for (int i = 0; i < 1000000; i++) {
    if (dis(gen) < 300000 && f.size() > 0) {
      std::cout << f.extractMin() << "\n";
    } else {
      f.insert(dis(gen));
    }
  }
}
