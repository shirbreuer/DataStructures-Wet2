#ifndef AVL_NODE_H
#define AVL_NODE_H

#include <iostream>

class classNode;
class courseNode;

template <class T>
class avlNode
{
private:
    T *data;
    avlNode *left;
    avlNode *right;
    avlNode *parent;
    int height;
    int rank;

public:
    avlNode() = default;
    avlNode(T *const value);
    avlNode(const avlNode<T> *node);
    ~avlNode() = default;
    avlNode *getLeft();
    avlNode *getRight();
    avlNode *getParent();
    T *getValue();
    const T *getValue() const;
    void setValue(T *const value);
    void setLeft(avlNode<T> *new_left);
    void setRight(avlNode<T> *new_right);
    void setParent(avlNode<T> *new_parent);
    void setRank(int new_rank) { this->rank = new_rank; }
    void setHeightAndRank();
    void print() const;
    int getHeight();
    int getRank();
    bool isLeftChild();
    bool isRightChild();

    void copyFrom(avlNode<T> *node);
    void swapWithChild(avlNode<T> *node, bool is_right);
};

#endif