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

#include <unordered_map>
#include <unordered_set>

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
  void insertSiblings(Node<ValType> *siblings);
  void removeSelfFromSiblings();
  void insertChild(Node<ValType> *child);
  void eraseChild(Node<ValType> *child);

  bool checkValidity();
  int getDoublyLinkedListSize();
  int getChildDoublyLinkedListSize();
  void deleteAllNodes();
};

template <typename ValType>
class FibonacciHeap {
 private:
  Node<ValType> *min_;
  int num_nodes_;
  std::unordered_map<ValType, std::unordered_set<Node<ValType> *>>
      value_to_ptr_;

  void consolidate();
  void heapLink(Node<ValType> *y, Node<ValType> *x);
  void cut(Node<ValType> *x, Node<ValType> *y);
  void cascadingCut(Node<ValType> *y);

 public:
  FibonacciHeap() : min_(nullptr), num_nodes_(0) {}
  ~FibonacciHeap();

  bool includes(ValType const &val);
  int size();

  void insert(ValType val);
  ValType findMin() const;
  ValType extractMin();
  void decreaseKey(ValType const &prev_key, ValType key,
                   bool is_delete = false);
  void erase(ValType const &key);
};
}  // namespace fib

#include "FibonacciHeap.cpp"

#endif  // FIBONACCIHEAP_H_
