#ifndef RBTREE_KCOLOR
#define RBTREE_KCOLOR

#include <cstddef>

typedef int color_t;

struct Node
{
    Node *parent;
    Node *left;
    Node *right;
    color_t color;
    int key;
};

Node *getNewNode(color_t color, int key);

class RBTree_kColor
{
private:
    // for implementation
    Node *root;
    Node *Nil;
    const int BLACK = 0;
    const int RED;
    const int TOTAL;

    void _left_rotate(Node *node);
    void _right_rotate(Node *node);
    void _insert_fixup(Node *node);
    void _delete_fixup(Node *node);
    void _transplant(Node *removed, Node *transplanted);
    Node *_getFindNode(int key);
    Node *_treeMinimum(Node *root);

    // for testing
    struct _info
    {
        int leftRotateCount = 0;
        int rightRotateCount = 0;
        int deleteRotateCount = 0;
        int insertRotateCount = 0;
    } info;
    long long exec_time = 0;

    bool _dfs_search(Node *node);
    int _dfs_getHeight(Node *node);
    int _dfs_getBlackHeight(Node *node);

public:
    // for implementation
    RBTree_kColor(int n);
    bool find(int key);
    bool insert(int key);
    bool erase(int key);

    // for testing
    int getHeight();
    int getBlackHeight();
    int getSize(Node *r = NULL);
    int getBlackSize(Node *r = NULL);
    long long getExecTime();
    double getBlackRatio();
    int getTotalRotateCount();
    int getInsertRotateCount();
    int getDeleteRotateCount();
    bool isRBTree();
    void print(Node *r = NULL);
};

#endif