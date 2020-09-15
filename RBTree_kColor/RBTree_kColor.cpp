#include <iostream>
#include <algorithm>
#include <chrono>
#include <cstddef>

#define NDEBUG
#include <cassert>

using namespace std;
using namespace chrono;

#include "RBTree_kColor.h"

Node *getNewNode(color_t color, int key)
{
    Node *node = new Node;
    node->parent = node->left = node->right = NULL;
    node->key = key;
    node->color = color;
    return node;
}

void RBTree_kColor::_left_rotate(Node *node)
{
    /*  
        to do: node->right를 node 위치로 회전
        6개의 포인터가 바뀌어야 함
    */
    info.leftRotateCount++;
    Node *child = node->right;
    node->right = child->left;
    if (child->left != Nil)
    {
        // Nil의 parent는 확정되지 않음 (Null로 유지)
        child->left->parent = node;
    }
    child->parent = node->parent;
    if (node->parent == Nil)
    {
        root = child;
    }
    else if (node == node->parent->left)
    {
        node->parent->left = child;
    }
    else
    {
        node->parent->right = child;
    }
    child->left = node;
    node->parent = child;
}

void RBTree_kColor::_right_rotate(Node *node)
{
    /*  
        to do: node->left를 node 위치로 회전
        6개의 포인터가 바뀌어야 함
    */
    info.rightRotateCount++;
    Node *child = node->left;
    node->left = child->right;
    if (child->right != Nil)
    {
        // Nil의 parent는 확정되지 않음 (Null로 유지)
        child->right->parent = node;
    }
    child->parent = node->parent;
    if (node->parent == Nil)
    {
        root = child;
    }
    else if (node == node->parent->right)
    {
        node->parent->right = child;
    }
    else
    {
        node->parent->left = child;
    }
    child->right = node;
    node->parent = child;
}

void RBTree_kColor::_insert_fixup(Node *node)
{
    // node->color <= node->parent->color, node->left와 node->right의 color는 node->color보다 크거나 BLACK
    while (node->parent->color && node->color == node->parent->color)
    {
        if ((node->parent->parent->color - node->color + TOTAL) % TOTAL >= 2)
        {
            node->parent->color = (node->parent->parent->color - 1 + TOTAL) % TOTAL;
        }
        else
        {
            if (node->parent == node->parent->parent->left)
            {
                Node *uncle = node->parent->parent->right;
                if (uncle->color == node->parent->color)
                {
                    if (node->parent->parent->color == RED)
                    {
                        node->parent->color = node->color + 1;
                        node = node->parent;
                    }
                    else
                    {
                        node->parent->color = (node->color + 1) % TOTAL;
                        uncle->color = (node->color + 1) % TOTAL;
                        node->parent->parent->color = (node->color + 2) % TOTAL;
                        node = node->parent->parent;
                    }
                }
                else
                {
                    if (node == node->parent->right)
                    {
                        node = node->parent;
                        info.insertRotateCount++;
                        _left_rotate(node);
                    }
                    node->parent->color = (node->color + 1) % TOTAL;
                    node->parent->parent->color = node->color;
                    info.insertRotateCount++;
                    _right_rotate(node->parent->parent);
                }
            }
            else
            {
                Node *uncle = node->parent->parent->left;
                if (uncle->color == node->parent->color)
                {
                    if (node->parent->parent->color == RED)
                    {
                        node->parent->color = node->color + 1;
                        node = node->parent;
                    }
                    else
                    {
                        node->parent->color = (node->color + 1) % TOTAL;
                        uncle->color = (node->color + 1) % TOTAL;
                        node->parent->parent->color = (node->color + 2) % TOTAL;
                        node = node->parent->parent;
                    }
                }
                else
                {
                    if (node == node->parent->left)
                    {
                        node = node->parent;
                        info.insertRotateCount++;
                        _right_rotate(node);
                    }
                    node->parent->color = (node->color + 1) % TOTAL;
                    node->parent->parent->color = node->color;
                    info.insertRotateCount++;
                    _left_rotate(node->parent->parent);
                }
            }
        }
    }
    root->color = BLACK;
    assert(isRBTree());
}

void RBTree_kColor::_delete_fixup(Node *node)
{
    while (node != root && node->color == BLACK)
    {
        if (node == node->parent->left)
        {
            Node *sibling = node->parent->right;
            while (sibling->color)
            {
                swap(sibling->color, node->parent->color);
                info.deleteRotateCount++;
                _left_rotate(node->parent);
                sibling = node->parent->right;
            }
            if (sibling->left->color == BLACK && sibling->right->color == BLACK)
            {
                sibling->color = RED;
                node = node->parent;
            }
            else
            {
                if (sibling->right->color == BLACK)
                {
                    sibling->left->color = BLACK;
                    sibling->color = RED;
                    info.deleteRotateCount++;
                    _right_rotate(sibling);
                    sibling = node->parent->right;
                }
                sibling->color = node->parent->color;
                node->parent->color = BLACK;
                sibling->right->color = BLACK;
                info.deleteRotateCount++;
                _left_rotate(node->parent);
                node = root;
            }
        }
        else
        {
            Node *sibling = node->parent->left;
            while (sibling->color)
            {
                swap(sibling->color, node->parent->color);
                info.deleteRotateCount++;
                _right_rotate(node->parent);
                sibling = node->parent->left;
            }
            if (sibling->right->color == BLACK && sibling->left->color == BLACK)
            {
                sibling->color = RED;
                node = node->parent;
            }
            else
            {
                if (sibling->left->color == BLACK)
                {
                    sibling->right->color = BLACK;
                    sibling->color = RED;
                    info.deleteRotateCount++;
                    _left_rotate(sibling);
                    sibling = node->parent->left;
                }
                sibling->color = node->parent->color;
                node->parent->color = BLACK;
                sibling->left->color = BLACK;
                info.deleteRotateCount++;
                _right_rotate(node->parent);
                node = root;
            }
        }
    }
    node->color = BLACK;
    assert(isRBTree());
}

void RBTree_kColor::_transplant(Node *removed, Node *transplanted)
{
    if (removed->parent == Nil)
        root = transplanted;
    else if (removed == removed->parent->left)
        removed->parent->left = transplanted;
    else
        removed->parent->right = transplanted;
    transplanted->parent = removed->parent;
}

bool RBTree_kColor::_dfs_search(Node *node)
{
    if (node == Nil)
    {
        return node->color == BLACK;
    }
    return !(node->color != BLACK && node->parent->color != BLACK && node->parent->color <= node->color) && _dfs_search(node->left) && _dfs_search(node->right);
}

int RBTree_kColor::_dfs_getHeight(Node *node)
{
    if (node == Nil)
        return 0;
    return 1 + max(_dfs_getHeight(node->left), _dfs_getHeight(node->right));
}

int RBTree_kColor::_dfs_getBlackHeight(Node *node)
{
    if (node == Nil)
        return 0;
    int l = _dfs_getBlackHeight(node->left);
    int r = _dfs_getBlackHeight(node->right);
    if (l == -1 || r == -1 || l != r)
    {
        return -1;
    }
    return l + (node->color == BLACK ? 1 : 0);
}

Node *RBTree_kColor::_getFindNode(int key)
{
    Node *parent = Nil;
    Node *child = root;
    while (child != Nil)
    {
        parent = child;
        if (key < child->key)
            child = child->left;
        else if (key > child->key)
            child = child->right;
        else
            return child;
    }
    return NULL;
}

Node *RBTree_kColor::_treeMinimum(Node *root)
{
    if (root->left == Nil)
        return root;
    else
        return _treeMinimum(root->left);
}

RBTree_kColor::RBTree_kColor(int n) : RED(n - 1), TOTAL(n)
{
    Nil = getNewNode(BLACK, -1);
    root = Nil;
    root->parent = Nil;
}

bool RBTree_kColor::find(int key)
{
    return (bool)(_getFindNode(key));
}

bool RBTree_kColor::insert(int key)
{
    system_clock::time_point start = system_clock::now();
    Node *parent = Nil, *child = root;
    while (child != Nil)
    {
        parent = child;
        if (key < child->key)
            child = child->left;
        else if (key > child->key)
            child = child->right;
        else
            return false;
    }
    // parent의 색이 있다면 node의 색은 parent와 같게, 색이 없다면 RED
    Node *node = getNewNode(parent->color == BLACK ? RED : (parent->color > 1 ? parent->color - 1 : 1), key);
    node->parent = parent;
    if (parent == Nil)
        root = node;
    else if (key < parent->key)
        parent->left = node;
    else
        parent->right = node;
    node->left = node->right = Nil;
    _insert_fixup(node);
    system_clock::time_point end = system_clock::now();
    exec_time += (end - start).count();
    return true;
}

bool RBTree_kColor::erase(int key)
{
    system_clock::time_point start = system_clock::now();
    Node *node = _getFindNode(key);
    if (!node)
        return false;
    Node *willBeReplaced = node;
    color_t original_color = node->color;

    Node *beginOfFixup;

    if (node->left == Nil)
    {
        beginOfFixup = node->right;
        _transplant(node, node->right);
    }
    else if (node->right == Nil)
    {
        beginOfFixup = node->left;
        _transplant(node, node->left);
    }
    else
    {
        willBeReplaced = _treeMinimum(node->right);
        original_color = willBeReplaced->color;
        beginOfFixup = willBeReplaced->right;
        if (willBeReplaced->parent == node)
            beginOfFixup->parent = willBeReplaced; // beginOfFixup == Nil인 경우 때문에
        else
        {
            _transplant(willBeReplaced, willBeReplaced->right);
            willBeReplaced->right = node->right;            // node 자리에
            willBeReplaced->right->parent = willBeReplaced; // willBeReplaced 삽입
        }
        _transplant(node, willBeReplaced);
        willBeReplaced->left = node->left;
        willBeReplaced->left->parent = willBeReplaced;
        willBeReplaced->color = node->color;
    }
    if (original_color == BLACK)
        _delete_fixup(beginOfFixup);
    system_clock::time_point end = system_clock::now();
    exec_time += (end - start).count();
    return true;
}

int RBTree_kColor::getHeight()
{
    return _dfs_getHeight(root);
}

int RBTree_kColor::getBlackHeight()
{
    return _dfs_getBlackHeight(root);
}

int RBTree_kColor::getSize(Node *r)
{
    if (r == NULL)
        r = root;
    if (r == Nil)
        return 0;
    return 1 + getSize(r->left) + getSize(r->right);
}

int RBTree_kColor::getBlackSize(Node *r)
{
    if (r == NULL)
        r = root;
    if (r == Nil)
        return 0;
    return (r->color == BLACK ? 1 : 0) + getBlackSize(r->left) + getBlackSize(r->right);
}

long long RBTree_kColor::getExecTime()
{
    return exec_time;
}

double RBTree_kColor::getBlackRatio()
{
    return (double)(getBlackSize()) / getSize();
}

int RBTree_kColor::getTotalRotateCount()
{
    return info.rightRotateCount + info.leftRotateCount;
}

int RBTree_kColor::getInsertRotateCount()
{
    return info.insertRotateCount;
}

int RBTree_kColor::getDeleteRotateCount()
{
    return info.deleteRotateCount;
}

bool RBTree_kColor::isRBTree()
{
    return root->color == BLACK && _dfs_search(root) && getBlackHeight() >= 0;
}

void RBTree_kColor::print(Node *r)
{
    if (r == NULL)
        r = root;
    if (r == Nil)
    {
        cout << "\n";
        return;
    }
    cout << "key : " << r->key << ", color : " << r->color << ", parent key: " << r->parent->key << "\n";

    print(r->left);
    print(r->right);
}