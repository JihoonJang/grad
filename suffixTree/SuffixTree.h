/**
 * @file SuffixTree.h
 * @author jhjang
 * @brief Suffix tree header
 * @version 0.1
 * @date 2021-01-14
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef SUFFIXTREE_H_
#define SUFFIXTREE_H_

#define NDEBUG

#include <algorithm>
#include <cassert>
#include <iostream>
#include <unordered_map>
#include <utility>

#include "./debug.h"

class SufTreeString {
 public:
  char *str;
  int begin, end;
  SufTreeString(char *str, int begin, int end)
      : str(str), begin(begin), end(end) {
    assert(begin <= end);
  }

  SufTreeString() {}

  char operator[](int i);

  SufTreeString operator-(SufTreeString other);

  int size() const;

  char *getStr();
};

class SufTreeNode {
 private:
  int _locus_string_size = 0;

 public:
  std::unordered_map<char, SufTreeNode *> children;
  SufTreeNode *suffix_link = NULL, *parent_link = NULL;
  SufTreeString arc_string;
  explicit SufTreeNode(SufTreeString arc_string) : arc_string(arc_string) {}
  void insertChild(SufTreeNode *child);

  SufTreeNode *split(SufTreeNode *child, int k);
  SufTreeString getLocusString();
  int getLocusBegin() const;
  void deleteAllNodes();
};

class SuffixTree {
 private:
  SufTreeString _main_str;
  SufTreeNode *_root;

  SufTreeNode *_rescan(SufTreeNode *n, SufTreeString b);

  std::pair<SufTreeNode *, SufTreeString> _scan(SufTreeNode *n,
                                                SufTreeString r);

  bool _dfs_search(SufTreeNode *cur, SufTreeString s);

 public:
  SuffixTree(char *str, int size);
  ~SuffixTree();

  bool search(char *str, int size);
  int getSizeOfLargestRepeatedSubstr(SufTreeNode *cur = NULL);
};

#endif  // SUFFIXTREE_H_
