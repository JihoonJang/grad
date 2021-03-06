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

#include "./debug.h"

namespace fib {
template <typename ValType>
void Node<ValType>::insertChild(Node<ValType> *child) {
  assert(this->getChildDoublyLinkedListSize() == this->degree);
  child->parent = this;
  if (this->child == NULL) {
    this->child = child;
    this->degree = 1;
  } else {
    this->child->insertSiblings(child);
    this->degree += 1;
  }
  assert(this->getChildDoublyLinkedListSize() == this->degree);
}

template <typename ValType>
void Node<ValType>::eraseChild(Node<ValType> *child) {
  assert(this->getChildDoublyLinkedListSize() == this->degree);
  assert(child->parent == this);
  assert(this->degree > 0);
  if (this->degree == 1) {
    assert(this->child == child);
    this->child = nullptr;
  } else if (this->child == child) {
    this->child = child->right;
  }
  this->degree -= 1;
  child->removeSelfFromSiblings();
  assert(this->getChildDoublyLinkedListSize() == this->degree);
}

template <typename ValType>
void Node<ValType>::insertSiblings(Node<ValType> *siblings) {
  assert(this->checkValidity());
  this->left->right = siblings;
  siblings->left->right = this;
  std::swap(this->left, siblings->left);
  assert(this->checkValidity());
}

template <typename ValType>
void Node<ValType>::removeSelfFromSiblings() {
  assert(this->checkValidity());

  this->left->right = this->right;
  this->right->left = this->left;

  this->parent = nullptr;
  this->left = this->right = this;

  assert(this->checkValidity());
}

template <typename ValType>
bool Node<ValType>::checkValidity() {
  assert(this->right->left == this && this->left->right == this);
  Node<ValType> *cur = this->right;
  int i;
  for (i = 0; i < 1000000; i++) {
    assert(this != cur->child);
    if (cur == this) return true;
    assert(this->parent == cur->parent);
    cur = cur->right;
  }
  return false;
}

template <typename ValType>
int Node<ValType>::getDoublyLinkedListSize() {
  int size = 0;
  Node<ValType> *cur = this;
  do {
    cur = cur->right;
    size++;
  } while (cur != this);
  return size;
}

template <typename ValType>
int Node<ValType>::getChildDoublyLinkedListSize() {
  if (this->child == nullptr)
    return 0;
  else
    return this->child->getDoublyLinkedListSize();
}

template <typename ValType>
void Node<ValType>::deleteAllNodes() {
  std::vector<Node<ValType> *> root_list;
  Node<ValType> *cur = this;
  do {
    root_list.push_back(cur);
    cur = cur->right;
  } while (cur != this);

  for (auto cur : root_list) {
    if (cur->child != nullptr) cur->child->deleteAllNodes();
    delete cur;
  }
}

template <typename ValType>
void FibonacciHeap<ValType>::consolidate() {
  std::vector<Node<ValType> *> arr(0), root_list(0);
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
      if (y->val < x->val) std::swap(x, y);
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
      w->removeSelfFromSiblings();
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
  y->mark = false;
}

template <typename ValType>
void FibonacciHeap<ValType>::cut(Node<ValType> *x, Node<ValType> *y) {
  y->eraseChild(x);
  this->min_->insertSiblings(x);
  x->mark = false;
}

template <typename ValType>
void FibonacciHeap<ValType>::cascadingCut(Node<ValType> *y) {
  Node<ValType> *z = y->parent;
  if (z != nullptr) {
    if (y->mark == false) {
      y->mark = true;
    } else {
      this->cut(y, z);
      this->cascadingCut(z);
    }
  }
}

template <typename ValType>
FibonacciHeap<ValType>::~FibonacciHeap() {
  if (this->min_ != nullptr) this->min_->deleteAllNodes();
}

template <typename ValType>
bool FibonacciHeap<ValType>::includes(ValType const &val) {
  return this->value_to_ptr_.find(val) != this->value_to_ptr_.end();
}

template <typename ValType>
int FibonacciHeap<ValType>::size() {
  return this->num_nodes_;
}

template <typename ValType>
void FibonacciHeap<ValType>::insert(ValType val) {
  Node<ValType> *n = new Node<ValType>(val);
  this->value_to_ptr_[val].insert(n);
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
    assert(this->value_to_ptr_.find(min_val) != this->value_to_ptr_.end());
    this->value_to_ptr_[min_val].erase(z);
    if (this->value_to_ptr_[min_val].size() == 0) {
      this->value_to_ptr_.erase(min_val);
    }
    delete z;
    this->num_nodes_ -= 1;
  }
  debug_print("size : %ld", this->value_to_ptr_.size());
  return min_val;
}

template <typename ValType>
void FibonacciHeap<ValType>::decreaseKey(ValType const &prev_key, ValType key,
                                         bool is_delete) {
  assert(this->value_to_ptr_.find(prev_key) != this->value_to_ptr_.end());
  assert(this->value_to_ptr_[prev_key].size() > 0);
  assert(is_delete || prev_key >= key);
  Node<ValType> *x = *(this->value_to_ptr_[prev_key].begin());

  x->val = key;
  Node<ValType> *y = x->parent;
  if (y != nullptr && (is_delete || x->val < y->val)) {
    this->cut(x, y);
    this->cascadingCut(y);
  }
  if (is_delete || x->val < this->min_->val) {
    this->min_ = x;
  }
  this->value_to_ptr_[prev_key].erase(x);
  if (this->value_to_ptr_[prev_key].size() == 0) {
    this->value_to_ptr_.erase(prev_key);
  }
  this->value_to_ptr_[key].insert(x);
}

template <typename ValType>
void FibonacciHeap<ValType>::erase(ValType const &key) {
  this->decreaseKey(key, ValType(0), true);
  ValType tmp = this->extractMin();
  assert(tmp == ValType(0));
}
}  // namespace fib
