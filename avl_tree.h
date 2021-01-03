#ifndef AVL_TREE_H
#define AVL_TREE_H

#include "avl_node.h"

typedef enum avlTreeResult_t
{
    AVL_TREE_OUT_OF_MEMORY = -2,
    AVL_TREE_INVALID_INPUT = -3,
    AVL_TREE_SUCCESS = 0,
    AVL_TREE_FAILURE = -1
} avlTreeResult_t;

template <class T>
class avlTree
{
private:
    avlNode<T> *root;
    avlNode<T> *largest;
    avlNode<T> *first; //is it equal to smallest?
    avlTreeResult_t insertAvlNode(avlNode<T> *root, avlNode<T> *new_node);
    avlTreeResult_t removeAvlNode(avlNode<T> *root, avlNode<T> *new_node);

public:
    avlTree();
    ~avlTree();
    avlTreeResult_t insert(T* const value);
    avlTreeResult_t remove(T* const value);
    avlTreeResult_t removeWOFreeing(T *const value);
    void treeBalance(avlNode<T> *root);
    void rotateLeft(avlNode<T> *root);
    void rotateRight(avlNode<T> *root);
    void cleanTree();
    avlNode<T> *getRoot() const { return this->root; }
    void inOrder(avlNode<T> *root, void (*function)(avlNode<T> *)) const; // Left, this, Right
    int nonRecursiveInOrder(int m, void (*function)(avlNode<T> *)) const; // Left, this, Right
    int nonRecursiveInOrder(int m, void (*function)(avlNode<T> *, int *, int *, int *, int), int *courses, int *classes) const;
    void preOrder(avlNode<T> *root, void (*function)(avlNode<T> *)) const;  // this, Left, Right
    void postOrder(avlNode<T> *root, void (*function)(avlNode<T> *)) const; // Left, Right, this
    int reverseInOrder(int m, void (*function)(avlNode<T> *)) const;
    int reverseInOrder(int m, void (*function)(avlNode<T> *, int *, int *, int), int *courses, int *classes) const;
    avlNode<T> *findNewRoot(avlNode<T> *newroot);
    void rootUpdate(avlNode<T> *newroot);
    void updateLargest(avlNode<T> *root);
    void updateFirst(avlNode<T> *root);
    avlNode<T> *getLargest();
    avlNode<T> *getFirst();
    void recursiveSetHeight(avlNode<T> *node);
    void removeLeaf(avlNode<T> *node);
    void removeNodeOneChild(avlNode<T> *node, bool is_right_child);
    void eraseAndBalance(avlNode<T> *node);
    avlNode<T> *createNewSubTree(avlNode<T> *node);
    void swap(avlNode<T> *src, avlNode<T> *dst);
    avlNode<T> *firstInOrder(avlNode<T> *sub_root);
    void removeNodeWithParent(avlNode<T> *node_to_remove);
};

template <class T>
avlNode<T> *find(avlNode<T> *root, T* const value);

template <class T>
avlNode<T> *find(avlNode<T> *root, const T& value);

template <class T>
int getBF(avlNode<T> *root);

#endif