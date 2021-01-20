/**
 * @file FibonacciHeap.h
 * @author jhjang
 * @brief
 * @version 0.1
 * @date 2021-01-19
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef FIBONACCIHEAP_H_
#define FIBONACCIHEAP_H_

#include "./debug.h"

namespace fib {
template <typename ValType>
class Node {
 public:
  int degree;
  Node<ValType> *left, *right, *parent, *child;
  ValType val;
  bool mark;
  explicit Node(ValType val)
      : left(this),
        right(this),
        parent(nullptr),
        child(nullptr),
        val(val),
        degree(0),
        mark(false) {}
  void insertSiblings(Node<ValType> *sibling);
  void removeSelfFromSiblings();
  void insertChild(Node<ValType> *child);
};

template <typename ValType>
class FibonacciHeap {
 private:
  Node<ValType> *min_;
  int num_nodes_;
  void consolidate();
  void heapLink(Node<ValType> *y, Node<ValType> *x);

 public:
  FibonacciHeap() : min_(nullptr), num_nodes_(0) {}
  ~FibonacciHeap();

  void insert(ValType val);
  ValType findMin() const;
  ValType extractMin();
};
}  // namespace fib

#include "FibonacciHeap.cpp"

#endif  // FIBONACCIHEAP_H_
