/**
 * @file FibonacciHeap.cpp
 * @author jhjang
 * @brief
 * @version 0.1
 * @date 2021-01-19
 *
 * @copyright Copyright (c) 2021
 *
 */
#include "./FibonacciHeap.h"

#include <vector>

namespace fib {
template <typename ValType>
void Node<ValType>::insertSiblings(Node<ValType> *siblings) {
  this->left->right = siblings;
  siblings->left->right = this;
  std::swap(this->left, siblings->left);
}

template <typename ValType>
void Node<ValType>::removeSelfFromSiblings() {
  this->left->right = this->right;
  this->right->left = this->left;
  this->left = this->right = this;
}

template <typename ValType>
void Node<ValType>::insertChild(Node<ValType> *child) {
  child->parent = this;
  if (this->child == nullptr) {
    this->child = child;
  } else {
    this->child->insertSiblings(child);
  }
  this->degree += 1;
}

template <typename ValType>
void FibonacciHeap<ValType>::consolidate() {
  std::vector<Node<ValType> *> arr, root_list;
  Node<ValType> *w = this->min_;

  do {
    root_list.push_back(w);
    w = w->right;
  } while (w != this->min_);

  for (auto x : root_list) {
    int d = x->degree;
    if (d >= arr.size()) arr.resize(d + 1, nullptr);
    while (arr[d] != nullptr) {
      Node<ValType> *y = arr[d];
      if (x->val > y->val) std::swap(x, y);
      this->heapLink(y, x);
      arr[d] = nullptr;
      d += 1;
      if (d >= arr.size()) arr.resize(d + 1, nullptr);
    }
    arr[d] = x;
    w = w->right;
  }

  this->min_ = nullptr;
  for (Node<ValType> *w : arr) {
    if (w != nullptr) {
      w->left = w->right = w;
      if (this->min_ == nullptr) {
        this->min_ = w;
      } else {
        this->min_->insertSiblings(w);
        if (w->val < this->min_->val) this->min_ = w;
      }
    }
  }
}

template <typename ValType>
void FibonacciHeap<ValType>::heapLink(Node<ValType> *y, Node<ValType> *x) {
  y->removeSelfFromSiblings();
  x->insertChild(y);
  y->mark = true;
}

template <typename ValType>
FibonacciHeap<ValType>::~FibonacciHeap() {}

template <typename ValType>
void FibonacciHeap<ValType>::insert(ValType val) {
  Node<ValType> *n = new Node<ValType>(val);
  if (this->min_ == nullptr) {
    this->min_ = n;
  } else {
    this->min_->insertSiblings(n);
    if (n->val < this->min_->val) this->min_ = n;
  }
  this->num_nodes_ += 1;
}

template <typename ValType>
ValType FibonacciHeap<ValType>::findMin() const {
  return this->min_->val;
}

template <typename ValType>
ValType FibonacciHeap<ValType>::extractMin() {
  assert(this->min_ != nullptr);
  assert(this->num_nodes_ > 0);
  Node<ValType> *z = this->min_;
  ValType min_val = z->val;
  if (z != nullptr) {
    if (z->child != nullptr) {
      Node<ValType> *it = z->child;
      do {
        it->parent = nullptr;
        it = it->right;
      } while (it != z->child);
      z->insertSiblings(z->child);
    }

    if (z == z->right) {
      this->min_ = nullptr;
    } else {
      this->min_ = z->right;
      z->removeSelfFromSiblings();
      this->consolidate();
    }
    delete z;
    this->num_nodes_ -= 1;
  }
  return min_val;
}
}  // namespace fib
