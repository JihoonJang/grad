#ifndef SUFFIXTREE_H_
#define SUFFIXTREE_H_

#define NDEBUG

#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <utility>
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

class SufTreeString {
 public:
  char* str;
  int begin, end;
  SufTreeString(char* str, int begin, int end)
      : str(str), begin(begin), end(end) {
    assert(begin <= end);
  }

  SufTreeString() {}

  char operator[](int i);

  SufTreeString operator-(SufTreeString other);

  int size() const;

  char* getStr();
};

class SufTreeNode {
 private:
  int _locus_string_size = 0;

 public:
  std::unordered_map<char, SufTreeNode*> children;
  SufTreeNode *suffix_link = NULL, *parent_link = NULL;
  SufTreeString arc_string;
  explicit SufTreeNode(SufTreeString arc_string) : arc_string(arc_string) {}
  SufTreeNode() {}
  void insertChild(SufTreeNode* child);

  SufTreeNode* split(SufTreeNode* child, int k);
  SufTreeString getLocusString();
  int getLocusBegin() const;
  void deleteAllNodes();
};

SufTreeNode* getNewNode(SufTreeString arc_string) {
  static SufTreeNode pool[2000200];
  static int cur = -1;

  cur += 1;
  pool[cur].arc_string = arc_string;
  return pool + cur;
}

struct st {
  int res;
  uint16_t bitmap;
  st(int res, uint16_t bitmap) : res(res), bitmap(bitmap) {}
};

class SuffixTree {
 private:
  SufTreeString _main_str;
  SufTreeNode* _root;

  SufTreeNode* _rescan(SufTreeNode* n, SufTreeString b);

  std::pair<SufTreeNode*, SufTreeString> _scan(SufTreeNode* n, SufTreeString r);

  bool _dfs_search(SufTreeNode* cur, SufTreeString s);

 public:
  SuffixTree(char* str, int size);
  ~SuffixTree();

  bool search(char* str, int size);
  int getSizeOfLargestRepeatedSubstr(SufTreeNode* cur = NULL);
  st getAnswer16908(std::vector<int> const& sizes, SufTreeNode* cur = nullptr);
};

constexpr char LAST_CHAR = '$';

char SufTreeString::operator[](int i) {
  assert(begin <= begin + i - 1 && begin + i - 1 < end);
  return str[begin + i - 2];
}

SufTreeString SufTreeString::operator-(SufTreeString other) {
  assert(other.size() <= this->size());

  if constexpr (_DEBUG) {
    for (int i = 1; i <= other.size(); i++) assert(other[i] == (*this)[i]);
  }

  return SufTreeString(str, begin + other.size(), end);
}

int SufTreeString::size() const { return end - begin; }

char* SufTreeString::getStr() {
  char* s = new char[size() + 1];
  for (int i = 0; i < size(); i++) {
    s[i] = (*this)[i + 1];
  }
  s[size()] = '\0';
  return s;
}

void SufTreeNode::insertChild(SufTreeNode* child) {
  this->children[child->arc_string[1]] = child;
  child->parent_link = this;
  child->_locus_string_size =
      this->_locus_string_size + child->arc_string.size();
}

SufTreeNode* SufTreeNode::split(SufTreeNode* child, int k) {
  assert(this->children[child->arc_string[1]] == child);
  k = child->arc_string.begin + k - 1;
  assert(child->arc_string.begin < k && child->arc_string.end > k);
  SufTreeNode* new_node = getNewNode(
      SufTreeString(child->arc_string.str, child->arc_string.begin, k));
  child->arc_string.begin = k;
  new_node->children[child->arc_string[1]] = child;
  new_node->parent_link = this;
  child->parent_link = new_node;
  this->children[new_node->arc_string[1]] = new_node;
  new_node->_locus_string_size =
      this->_locus_string_size + new_node->arc_string.size();
  return new_node;
}

SufTreeString SufTreeNode::getLocusString() {
  return SufTreeString(this->arc_string.str,
                       this->arc_string.end - this->_locus_string_size,
                       this->arc_string.end);
}

int SufTreeNode::getLocusBegin() const {
  return this->arc_string.end - this->_locus_string_size;
}

void SufTreeNode::deleteAllNodes() {
  for (auto child : this->children) {
    child.second->deleteAllNodes();
  }
  delete this;
}

SufTreeNode* SuffixTree::_rescan(SufTreeNode* n, SufTreeString b) {
  int l = 1;
  while (l <= b.size()) {
    SufTreeNode* n_next = n->children[b[l]];
    SufTreeString w = n_next->arc_string;
    if (l + w.size() > b.size() + 1) {
      int k = b.size() - l + 1;
      return n->split(n_next, k + 1);
    }
    l += w.size();
    n = n_next;
  }
  return n;
}

std::pair<SufTreeNode*, SufTreeString> SuffixTree::_scan(SufTreeNode* n,
                                                         SufTreeString r) {
  int l = 1;
  while (n->children.find(r[l]) != n->children.end()) {
    int k = 1;
    SufTreeNode* n_next = n->children[r[l]];
    SufTreeString w = n_next->arc_string;
    while (k <= w.size() && w[k] == r[l]) {
      k += 1;
      l += 1;
    }
    if (k > w.size())
      n = n_next;
    else
      return std::make_pair(n->split(n_next, k),
                            SufTreeString(r.str, r.begin + l - 1, r.end));
  }
  return std::make_pair(n, SufTreeString(r.str, r.begin + l - 1, r.end));
}

bool SuffixTree::_dfs_search(SufTreeNode* cur, SufTreeString s) {
  if (cur->children.find(s[1]) == cur->children.end()) return false;
  auto next = cur->children[s[1]];
  auto arc = next->arc_string;
  debug_print("dfs search\narc: %s\nstr: %s\n", arc.getStr(), s.getStr());
  int i;
  for (i = 1; i <= s.size() && i <= arc.size(); i++) {
    if (s[i] != arc[i]) return false;
  }
  if (s.size() > arc.size()) {
    SufTreeString prefix(s.str, s.begin, s.begin + i - 1);
    return _dfs_search(next, s - prefix);
  } else {
    return true;
  }
}

SuffixTree::SuffixTree(char* str, int size) {
  str[size] = LAST_CHAR;
  size++;

  _root = getNewNode(SufTreeString(str, 1, 1));
  _root->parent_link = _root;

  SufTreeNode* head_locus = _root;
  SufTreeString x, a, b, suf;

  for (int i = 1; i <= size; i++) {
    suf = SufTreeString(str, i, size + 1);
    debug_print("head%d: %s", i - 1, head_locus->getLocusString().getStr());
    if (head_locus->getLocusString().size() == 0) {
      x = a = b = SufTreeString(str, 1, 1);
    } else {
      x = SufTreeString(str, head_locus->getLocusBegin(),
                        head_locus->getLocusBegin() + 1);
      if (head_locus->parent_link != _root) {
        debug_print("head parent exist!\nparent locus: %s",
                    head_locus->parent_link->getLocusString().getStr());
        a = SufTreeString(
            str, x.end,
            x.end + head_locus->parent_link->getLocusString().size() - 1);
      } else {
        a = SufTreeString(str, x.end, x.end);
      }
      b = SufTreeString(str, a.end, head_locus->getLocusString().end);
    }

    debug_print("in step %d, \nhead: %s\nx: %s\na: %s\nb: %s\n", i,
                head_locus->getLocusString().getStr(), x.getStr(), a.getStr(),
                b.getStr());

    SufTreeNode* c;
    if (a.size() > 0)
      c = head_locus->parent_link->suffix_link;
    else
      c = _root;

    SufTreeNode* ab = _rescan(c, b);

    head_locus->suffix_link = ab;

    auto ht = _scan(ab, suf - ab->getLocusString());
    ht.first->insertChild(getNewNode(ht.second));
    head_locus = ht.first;
  }
}

SuffixTree::~SuffixTree() {  // this->_root->deleteAllNodes();
}

bool SuffixTree::search(char* str, int size) {
  return _dfs_search(_root, SufTreeString(str, 1, size + 1));
}

int SuffixTree::getSizeOfLargestRepeatedSubstr(SufTreeNode* cur) {
  if (cur == NULL) cur = _root;

  int res = cur->children.size() > 1 ? cur->getLocusString().size() : 0;

  for (auto& child : cur->children) {
    int child_res = getSizeOfLargestRepeatedSubstr(child.second);
    res = std::max(res, child_res);
  }

  return res;
}

st SuffixTree::getAnswer16908(std::vector<int> const& sizes, SufTreeNode* cur) {
  if (cur == nullptr) cur = this->_root;
  // std::cout << "locus : " << cur->arc_string.begin << " " <<
  // cur->arc_string.end
  //           << "\n";
  st res(0, 0);

  if (cur->children.size() == 0) {
    // std::cout << "no child\n";
    int begin = cur->arc_string.begin;
    for (size_t i = 0; i < sizes.size(); i++) {
      if (sizes[i] >= begin) return st(0, 1 << i);
    }
  } else {
    uint16_t bitmap = 0;

    for (auto& child : cur->children) {
      st child_res = this->getAnswer16908(sizes, child.second);
      if (child_res.bitmap == (1 << sizes.size()) - 1 &&
          res.res < child_res.res)
        res = child_res;
      bitmap |= child_res.bitmap;
    }
    res.bitmap = bitmap;
    // std::cout << "a " << bitmap << " " << ((1 << sizes.size()) - 1) << "\n";
    if (bitmap == (1 << sizes.size()) - 1) {
      res.res += cur->arc_string.size();
    }
  }
  // std::cout << res.res << " " << res.bitmap << "\n";
  return res;
}

#endif  // SUFFIXTREE_H_

#include <cstring>

char s[1000020];

int main() {
  int cnt;
  std::vector<int> sizes;

  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  std::ios::sync_with_stdio(false);

  std::cin >> cnt;

  int startIdx = 0;
  for (size_t i = 0; i < cnt; i++) {
    std::cin >> (s + startIdx);
    startIdx += strlen(s + startIdx) + 1;
    s[startIdx - 1] = 'A' + static_cast<char>(i);
    sizes.push_back(startIdx);
  }

  s[startIdx] = '\0';

  SuffixTree stree(s, startIdx);

  std::cout << stree.getAnswer16908(sizes).res;
}
