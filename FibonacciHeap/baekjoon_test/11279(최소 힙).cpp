#include <iostream>

#define NDEBUG

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

#include <vector>

#ifndef DEBUG_H_
#define DEBUG_H_

#include <cassert>
#ifndef NDEBUG

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"

#define debug_print(fmt, ...)          \
  {                                    \
    fprintf(stderr, ANSI_COLOR_CYAN);  \
    fprintf(stderr, fmt, __VA_ARGS__); \
    fprintf(stderr, ANSI_COLOR_RESET); \
    fprintf(stderr, "\n");             \
  }

constexpr bool _DEBUG = true;

#else

constexpr bool _DEBUG = false;

#define debug_print(fmt, ...)

#endif  // NDEBUG
#endif  // DEBUG_H_

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

#endif  // FIBONACCIHEAP_H_

int main() {
  fib::FibonacciHeap<int> f;
  std::cin.tie(NULL);
  std::cout.tie(NULL);
  std::ios::sync_with_stdio(false);

  int n, val;
  std::cin >> n;
  for (size_t i = 0; i < n; i++) {
    std::cin >> val;
    if (val > 0) {
      f.insert(val);
    } else {
      if (f.size() > 0)
        std::cout << f.extractMin() << "\n";
      else
        std::cout << 0 << "\n";
    }
  }
}
