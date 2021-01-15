#ifndef AVL_TREE_HPP
#define AVL_TREE_HPP

#include "avl_tree.h"
#include "avl_node.hpp"
#include "two_way_list.hpp"
#include "two_way_list_node.hpp"
#include <stdlib.h>

#include <iostream>

// void printCourseNode2(avlNode<courseNode> *node);
// void printCourseNode2(avlNode<classNode> *node);

template <class T>
static void deleteNode(avlNode<T> *node)
{
    if (!node)
        return;
    if (node->getValue())
    {
        delete node->getValue();
    }
    delete node;
}

template <class T>
avlTree<T>::avlTree() : root(NULL), largest(NULL), first(NULL)
{
}

template <class T>
avlTree<T>::~avlTree()
{
    if (!root)
        return;
    postOrder(root, deleteNode);
}

inline void deleteNodeWOFreeing(avlNode<twList<int>> *node)
{
    if (!node)
        return;
    delete node;
}

template <>
inline avlTree<twList<int>>::~avlTree()
{
    if (!root)
        return;
    postOrder(root, deleteNodeWOFreeing);
}

template <class T>
void print(avlNode<T> *root)
{
    std::cout << *(root->getValue()) << " ";
}

template <class T>
void printBF(avlNode<T> *root)
{
    std::cout << getBF(root) << " ";
}

template <class T>
void printHeight(avlNode<T> *root)
{
    if (root)
        std::cout << root->getHeight() << " ";
}

template <class T>
void printRank(avlNode<T> *root)
{
    if (root)
        std::cout << root->getRank() << " ";
}

template <class T>
void printTreeStatus(avlTree<T> *tree)
{
    if (tree->getRoot())
    {
        std::cout << "root is: " << tree->getRoot()->getValue() << std::endl;
        if (tree->getRoot()->getRight())
            std::cout << "right child is:  " << tree->getRoot()->getRight()->getValue() << std::endl;
        if (tree->getRoot()->getLeft())
            std::cout << "left child is: " << tree->getRoot()->getLeft()->getValue() << std::endl;
    }
}

template <class T>
void printCourseNode(avlNode<T> *node)
{
    std::cout << "node: ";
    std::cout << node->getValue() << std::endl;
    std::cout << "Left child: ";
    printLeftChild(node);
    std::cout << "Right child: ";
    printRightChild(node);
}

template <class T>
avlTreeResult_t avlTree<T>::insert(T *const value)
{
    avlNode<T> *new_node = new avlNode<T>(value);

    if (!new_node)
    {
        return AVL_TREE_OUT_OF_MEMORY; // Out of memory
    }

    if (!root)
    { // Special case
        root = new_node;
        root->setHeightAndRank();
        largest = new_node;
        first = new_node;
        return AVL_TREE_SUCCESS;
    }
    avlNode<T> *node = find(root, (new_node->getValue()));
    if (node)
    {
        deleteNode(new_node);
        return AVL_TREE_FAILURE;
    }
    else if (insertAvlNode(root, new_node) != AVL_TREE_SUCCESS)
    {
        deleteNode(new_node);
        return AVL_TREE_FAILURE;
    }
    treeBalance(new_node);
    updateLargest(root);
    updateFirst(root);
    return AVL_TREE_SUCCESS;
}

template <class T>
avlTreeResult_t avlTree<T>::remove(T *const value)
{
    if (!root)
        return AVL_TREE_INVALID_INPUT;

    avlNode<T> *node_to_remove = find(this->root, value);

    if (!node_to_remove)
        return AVL_TREE_FAILURE;

    if (!node_to_remove->getParent())
    {
        this->root = createNewSubTree(node_to_remove);
        if (!this->root)
        {
            deleteNode(node_to_remove);
            return AVL_TREE_SUCCESS;
        }
        avlNode<T> *newroot = findNewRoot(this->root);
        rootUpdate(newroot);
    }
    else
        removeNodeWithParent(node_to_remove, 1);

    deleteNode(node_to_remove);
    updateLargest(root);
    updateFirst(root);
    // std::cout << "updated after removing first: " << this->first->getValue() << std::endl;
    return AVL_TREE_SUCCESS;
}

template <class T>
avlTreeResult_t avlTree<T>::removeWOFreeing(T *const value)
{
    if (!root)
        return AVL_TREE_INVALID_INPUT;

    avlNode<T> *node_to_remove = find(this->root, value);

    if (!node_to_remove)
        return AVL_TREE_FAILURE;

    if (!node_to_remove->getParent())
    {
        this->root = createNewSubTree(node_to_remove);
        if (!this->root)
        {
            deleteNodeWOFreeing(node_to_remove);
            return AVL_TREE_SUCCESS;
        }
        avlNode<T> *newroot = findNewRoot(this->root);
        rootUpdate(newroot);
    }
    else
        removeNodeWithParent(node_to_remove, 1);

    deleteNodeWOFreeing(node_to_remove);
    updateLargest(root);
    updateFirst(root);
    // std::cout << "updated after removing first: " << this->first->getValue() << std::endl;
    return AVL_TREE_SUCCESS;
}

template <class T>
void avlTree<T>::removeNodeWithParent(avlNode<T> *node_to_remove, int first_call)
{
    // std::cout << "removing node with parent: " << node_to_remove->getValue() << std::endl;
    avlNode<T> *parent = node_to_remove->getParent();
    // std::cout << "parent: " << node_to_remove->getParent()->getValue() << std::endl;
    bool is_right = false;
    if (node_to_remove->isLeftChild())
    {
        // std::cout << "left child" << std::endl;
        parent->setLeft(createNewSubTree(node_to_remove));
        // printCourseNode(firstInOrder(root));
        // std::cout << " supposed to be the new first after creating new sub tree" << std::endl;
    }
    else
    {
        // std::cout << "right child" << std::endl;
        parent->setRight(createNewSubTree(node_to_remove));
        is_right = true;
    }
    if (is_right)
    {
        recursiveSetHeightAndRank(parent->getRight());
        if (first_call)
            treeBalance(parent->getRight());
    }
    else
    {
        recursiveSetHeightAndRank(parent->getLeft());
        if (first_call)
            treeBalance(parent->getLeft());
    }
}

template <class T>
void printNode(avlNode<T> *node)
{
    std::cout << "=========================" << std::endl;
    std::cout << "left child of: " << node->getValue() << " is: " << (node->getLeft() ? node->getLeft()->getValue() : -1) << std::endl;
    std::cout << "Right child of: " << node->getValue() << " is: " << (node->getRight() ? node->getRight()->getValue() : -1) << std::endl;
    std::cout << "parent of: " << node->getValue() << " is: " << (node->getParent() ? node->getParent()->getValue() : -1) << std::endl;
    if (node->getParent())
    {
        if (node->isLeftChild())
            std::cout << node->getValue() << " is left child" << std::endl;
        else
            std::cout << node->getValue() << " is right child" << std::endl;
    }
    std::cout << "=========================" << std::endl;
}

template <class T>
void printLeftChild(avlNode<T> *node)
{
    if (node->getLeft())
        std::cout << node->getLeft()->getValue() << std::endl;
    else
        std::cout << "No Left child." << std::endl;
}

template <class T>
void printRightChild(avlNode<T> *node)
{
    if (node->getRight())
        std::cout << node->getRight()->getValue() << std::endl;
    else
        std::cout << "No Right child." << std::endl;
}

template <class T>
avlNode<T> *avlTree<T>::createNewSubTree(avlNode<T> *node)
{
    if ((node->getRight()) && (node->getLeft()))
    {
        // std::cout << "has both childs " << std::endl;
        avlNode<T> *next_node_in_order = firstInOrder(node->getRight());
        // printCourseNode(node);
        // printCourseNode(next_node_in_order);
        swap(next_node_in_order, node);
        // printCourseNode(node);
        // printCourseNode(next_node_in_order);
        next_node_in_order->setParent(NULL);
        removeNodeWithParent(node);
        // std::cout << "next node in order left child: " << next_node_in_order->getLeft()->getValue() << std::endl;
        return next_node_in_order;
    }
    if ((!node->getRight()) && (!node->getLeft()))
    {
        // printCourseNode(node);
        // std::cout << "this is a leaf" << std::endl;
        return NULL;
    }
    if (node->getRight())
    {
        node->getRight()->setParent(NULL);
        avlNode<T> *right_sub_root = node->getRight();
        return right_sub_root;
    }
    node->getLeft()->setParent(NULL);
    avlNode<T> *left_sub_root = node->getLeft();
    return left_sub_root;
}

template <class T>
avlNode<T> *avlTree<T>::firstInOrder(avlNode<T> *sub_root)
{
    while (sub_root->getLeft())
    {
        sub_root = sub_root->getLeft();
    }
    return sub_root;
}

template <class T>
void avlTree<T>::swap(avlNode<T> *src, avlNode<T> *dst)
{
    // std::cout << "trying to swap: src = " << src->getValue() << std::endl;
    // std::cout << "is left child:  " << src->getParent()->getLeft()->getValue() << std::endl;
    // std::cout << "is right child:  " << src->getParent()->getRight()->getValue() << std::endl;

    if (src->getRight() == dst) //if dst is right child of src perform parent-child right swap
    {
        // std::cout << "1" << std::endl;
        src->swapWithChild(dst, 1);
    }
    else if (src->getLeft() == dst) //if dst is left child of src perform parent-child left swap
    {
        // std::cout << "2" << std::endl;
        src->swapWithChild(dst, 0);
    }
    else if (dst->getRight() == src) //if src is right child of dst perform parent-child right swap
    {
        // std::cout << "3" << std::endl;
        dst->swapWithChild(src, 1);
    }
    else if (dst->getLeft() == src) //if src is left child of dst perform parent-child left swap
    {
        // std::cout << "4" << std::endl;
        dst->swapWithChild(src, 0);
    }
    else //not connected in the tree
    {
        avlNode<T> temp_src = *src;
        avlNode<T> temp_dst = *dst;
        src->copyFrom(&temp_dst);
        dst->copyFrom(&temp_src);
    }
}

template <class T>
void avlTree<T>::rootUpdate(avlNode<T> *newroot)
{
    this->root = newroot;
}

template <class T>
avlNode<T> *avlTree<T>::getLargest()
{
    assert(this->root);
    return this->largest;
}

template <class T>
avlNode<T> *avlTree<T>::getFirst()
{
    assert(this->root);

    return this->first;
}

template <class T>
void avlTree<T>::updateLargest(avlNode<T> *root)
{
    if (!root)
        return;
    largest = root;
    while (largest->getRight())
    {
        largest = largest->getRight();
    }
}

template <class T>
void avlTree<T>::updateFirst(avlNode<T> *root)
{
    if (!root)
        return;
    first = root;
    while (first->getLeft())
    {
        first = first->getLeft();
    }
}

template <class T>
avlNode<T> *avlTree<T>::findNewRoot(avlNode<T> *newroot)
{
    if (!newroot)
        return NULL;
    while (newroot->getParent())
    {
        newroot = newroot->getParent();
    }
    return newroot;
}

template <class T>
avlTreeResult_t avlTree<T>::insertAvlNode(avlNode<T> *root, avlNode<T> *new_node)
{
    // Binary Search Tree insertion algorithm
    if (*(new_node->getValue()) < *(root->getValue()))
    {
        if (root->getLeft())
        { // If there is a left child, keep searching
            avlTreeResult_t result = insertAvlNode(root->getLeft(), new_node);
            root->setHeightAndRank();
            return result;
        }
        else
        { // Found the right spot
            // std::cout << "SPOT FOR: " << new_node->getValue() << std::endl;
            root->setLeft(new_node);
            // new_node->setParent(root);
            new_node->setHeightAndRank();
            // std::cout << "new node height: " << new_node->getHeight() << std::endl;
            root->setHeightAndRank();
            return AVL_TREE_SUCCESS;
        }
    }
    else
    {
        if (root->getRight())
        { // If there is a right child, keep searching
            avlTreeResult_t result = insertAvlNode(root->getRight(), new_node);
            root->setHeightAndRank();
            // std::cout << "Root is: " << root->getValue() << " and it's height is: " << root->getHeight() << std::endl;
            return result;
        }
        else
        { // Found the right spot
            root->setRight(new_node);
            // new_node->setParent(root);
            new_node->setHeightAndRank();
            root->setHeightAndRank();
            return AVL_TREE_SUCCESS;
        }
    }
}

template <class T>
void avlTree<T>::treeBalance(avlNode<T> *root)
{
    if (!root)
        return;

    int balance = getBF(root);
    if (balance > 1)
    { // left tree unbalanced
        if (getBF(root->getLeft()) < 0)
            rotateLeft(root->getLeft()); // double rotation required
        rotateRight(root);
    }
    else if (balance < -1)
    {
        // right tree unbalanced
        if (getBF(root->getRight()) > 0)
            rotateRight(root->getRight());
        rotateLeft(root);
    }
    if (root->getParent())
    {
        treeBalance(root->getParent());
    }
}

template <class T>
void avlTree<T>::rotateLeft(avlNode<T> *sub_root)
{
    // std::cout << "Rotating: " << sub_root->getValue() << " left" << std::endl;
    avlNode<T> *newroot = sub_root->getRight();
    sub_root->setRight(newroot->getLeft());

    if (sub_root->getParent() == NULL)
    {
        this->root = newroot;
        newroot->setParent(NULL);
    }
    else
    {
        if (sub_root->isLeftChild())
        {
            sub_root->getParent()->setLeft(newroot);
        }
        else
        {
            sub_root->getParent()->setRight(newroot);
        }
    }
    newroot->setLeft(sub_root);
    this->recursiveSetHeightAndRank(sub_root);
    // std::cout << sub_root->getValue() << "'s new parent is: " << newroot->getValue() << std::endl;
}

template <class T>
void avlTree<T>::recursiveSetHeightAndRank(avlNode<T> *node)
{
    while (node)
    {
        node->setHeightAndRank();
        node = node->getParent();
    }
}

template <class T>
void avlTree<T>::rotateRight(avlNode<T> *sub_root)
{
    // std::cout << "Rotating: " << sub_root->getValue() << " right" << std::endl;
    avlNode<T> *newroot = sub_root->getLeft();
    sub_root->setLeft(newroot->getRight());

    if (sub_root->getParent() == NULL)
    {
        this->root = newroot;
        newroot->setParent(NULL);
    }
    else
    {
        // std::cout << "sub_root's parent is: " << sub_root->getParent()->getValue() << std::endl;
        if (sub_root->isLeftChild())
        {
            // std::cout << "sub_root is left child" << std::endl;
            sub_root->getParent()->setLeft(newroot);
        }
        else
        {
            // std::cout << "sub_root is right child" << std::endl;
            sub_root->getParent()->setRight(newroot);
        }
    }
    newroot->setRight(sub_root);
    this->recursiveSetHeightAndRank(sub_root);

    // std::cout << sub_root->getValue() << "'s new parent is: " << newroot->getValue() << std::endl;
}

template <class T>
avlNode<T> *find(avlNode<T> *root, T *const value)
{
    if (!root)
        return NULL;
    if (!value)
        return NULL;

    if (*(root->getValue()) == *value)
        return root;

    else if (*value < *(root->getValue()))
        return find(root->getLeft(), value); //search left sub tree
    else
        return find(root->getRight(), value); //search right sub tree
}

template <class T>
avlNode<T> *find(avlNode<T> *root, const T &value)
{
    if (!root)
        return NULL;

    if (!value)
        return NULL;

    if (*(root->getValue()) == value)
        return root;

    else if (value < *(root->getValue()))
        return find(root->getLeft(), value); //search left sub tree

    else
        return find(root->getRight(), value); //search right sub tree
}

template <class T>
int getBF(avlNode<T> *root)
{
    if (!root)
    {
        return 0;
    }
    int balance;
    int left = root->getLeft() ? root->getLeft()->getHeight() : -1;
    int right = root->getRight() ? root->getRight()->getHeight() : -1;

    // std::cout << "left height: " << left << "     right height: " << right << std::endl;
    balance = left - right;
    return balance;
}

template <class T>
void avlTree<T>::inOrder(avlNode<T> *root, void (*function)(avlNode<T> *)) const
{
    if (!root)
        return;
    inOrder(root->getLeft(), function);
    function(root);
    inOrder(root->getRight(), function);
}

template <class T>
void avlTree<T>::postOrder(avlNode<T> *root, void (*function)(avlNode<T> *)) const
{
    if (!root)
        return;
    postOrder(root->getLeft(), function);
    postOrder(root->getRight(), function);
    function(root);
}

template <class T>
void avlTree<T>::preOrder(avlNode<T> *root, void (*function)(avlNode<T> *)) const
{
    if (!root)
        return;
    function(root);
    preOrder(root->getLeft(), function);
    preOrder(root->getRight(), function);
}

template <class T>
void updateNextNode(avlNode<T> **node)
{
    if ((*node)->getLeft())
        (*node) = (*node)->getLeft();
    else
        (*node) = (*node)->getParent();
}

template <class T>
void updateNextNodeInOrder(avlNode<T> **node)
{
    if ((*node)->getRight())
        (*node) = (*node)->getRight();
    else
        (*node) = (*node)->getParent();
}

template <class T>
int avlTree<T>::nonRecursiveInOrder(int m, void (*function)(avlNode<T> *)) const
{
    if (this->root == NULL)
        return -1;
    int i = 0;
    avlNode<T> *last_node = NULL;
    avlNode<T> *node = first;
    while (i < m)
    {
        if (!node)
            break;
        if (node->getParent())
        {
            if (node->isRightChild())
            {
                if (!last_node)
                {
                    function(node);
                    i++;
                    updateNextNodeInOrder(&node);
                    continue;
                }
                else if (last_node == node->getParent())
                {
                    last_node = node;
                    if (node->getLeft())
                    {
                        node = node->getLeft();
                        continue;
                    }
                    function(node);
                    i++;
                    if (node == this->largest)
                        return (m - i);
                    updateNextNodeInOrder(&node);
                }
                else if (last_node == node->getRight())
                {
                    last_node = node;
                    node = node->getParent();
                    continue;
                }
                else if (last_node == node->getLeft())
                {
                    function(node);
                    i++;
                    if (node == this->largest)
                        return (m - i);
                    last_node = node;
                    updateNextNodeInOrder(&node);
                }
            }
            else
            {
                if (!last_node)
                {
                    function(node);
                    i++;
                    last_node = node;
                    updateNextNodeInOrder(&node);
                    continue;
                }
                else if (last_node == node->getParent())
                {
                    last_node = node;
                    if (node->getLeft())
                    {
                        node = node->getLeft();
                        continue;
                    }
                    function(node);
                    i++;
                    updateNextNodeInOrder(&node);
                }
                else if (last_node == node->getRight())
                {
                    last_node = node;
                    node = node->getParent();
                }
                else if (last_node == node->getLeft())
                {
                    function(node);
                    i++;
                    last_node = node;
                    updateNextNodeInOrder(&node);
                }
            }
        }
        else
        {
            function(node); //I need clarifiction
            i++;
            if (node == this->largest)
                return (m - i);
            last_node = node;
            updateNextNodeInOrder(&node);
        }
    }
    return (m - i);
}

template <class T>
int avlTree<T>::nonRecursiveInOrder(int m, void (*function)(avlNode<T> *, int *, int *, int *, int), int *courses, int *classes) const
{
    if (this->root == NULL)
        return -1;
    int i = 0;
    avlNode<T> *last_node = NULL;
    avlNode<T> *node = first;
    while (i < m)
    {
        if (!node)
            break;
        if (node->getParent())
        {
            if (node->isRightChild())
            {
                if (!last_node)
                {
                    function(node, courses, classes, &i, m);
                    updateNextNodeInOrder(&node);
                    continue;
                }
                else if (last_node == node->getParent())
                {
                    last_node = node;
                    if (node->getLeft())
                    {
                        node = node->getLeft();
                        continue;
                    }
                    function(node, courses, classes, &i, m);
                    if (node == this->largest)
                        return (m - i);
                    updateNextNodeInOrder(&node);
                    continue;
                }
                else if (last_node == node->getRight())
                {
                    last_node = node;
                    node = node->getParent();
                    continue;
                }
                else if (last_node == node->getLeft())
                {
                    function(node, courses, classes, &i, m);
                    if (node == this->largest)
                        return (m - i);
                    last_node = node;
                    updateNextNodeInOrder(&node);
                    continue;
                }
            }
            else
            {
                if (!last_node)
                {
                    function(node, courses, classes, &i, m);
                    last_node = node;
                    updateNextNodeInOrder(&node);
                    continue;
                }
                else if (last_node == node->getParent())
                {
                    last_node = node;
                    if (node->getLeft())
                    {
                        node = node->getLeft();
                        continue;
                    }
                    function(node, courses, classes, &i, m);
                    updateNextNodeInOrder(&node);
                    continue;
                }
                else if (last_node == node->getRight())
                {
                    last_node = node;
                    node = node->getParent();
                }
                else if (last_node == node->getLeft())
                {
                    function(node, courses, classes, &i, m);
                    last_node = node;
                    updateNextNodeInOrder(&node);
                    continue;
                }
            }
        }
        else
        {
            function(node, courses, classes, &i, m);
            if (node == this->largest)
                return (m - i);
            last_node = node;
            updateNextNodeInOrder(&node);
            continue;
        }
    }
    return (m - i);
}

template <class T>
int avlTree<T>::reverseInOrder(int m, void (*function)(avlNode<T> *, int *, int *, int), int *courses, int *classes) const
{
    if (this->root == NULL)
        return m;
    int i = 0;
    avlNode<T> *last_node = NULL;
    avlNode<T> *node = largest;
    while (i < m)
    {
        // std::cout << node->getValue() << std::endl;
        if (!node)
            break;
        if (node->getParent())
        {
            if (node->isLeftChild())
            {
                if (!last_node)
                {
                    function(node, courses, classes, i);
                    i++;
                    updateNextNode(&node);
                    continue;
                }
                else if (last_node == node->getParent())
                {
                    last_node = node;
                    if (node->getRight())
                    {
                        node = node->getRight();
                        continue;
                    }
                    function(node, courses, classes, i);
                    i++;
                    if (node == this->first)
                        return (m - i);
                    updateNextNode(&node);
                }
                else if (last_node == node->getLeft())
                {
                    last_node = node;
                    node = node->getParent();
                    continue;
                }
                else if (last_node == node->getRight())
                {
                    function(node, courses, classes, i);
                    i++;
                    if (node == this->first)
                        return (m - i);
                    last_node = node;
                    updateNextNode(&node);
                }
            }
            else
            {
                if (!last_node)
                {
                    function(node, courses, classes, i);
                    i++;
                    last_node = node;
                    updateNextNode(&node);
                }
                else if (last_node == node->getParent())
                {
                    last_node = node;
                    if (node->getRight())
                    {
                        node = node->getRight();
                        continue;
                    }
                    function(node, courses, classes, i);
                    i++;
                    updateNextNode(&node);
                }
                else if (last_node == node->getLeft())
                {
                    last_node = node;
                    node = node->getParent();
                }
                else if (last_node == node->getRight())
                {
                    function(node, courses, classes, i);
                    i++;
                    last_node = node;
                    updateNextNode(&node);
                }
            }
        }
        else
        {
            function(node, courses, classes, i);
            i++;
            if (node == this->first)
                return (m - i);
            last_node = node;
            updateNextNode(&node);
        }
    }
    return (m - i);
}

template <class T>
avlNode<T>* avlTree<T>::select (int k){
    if (k > root.getIndex()){
        return nullptr;
    }
    int wanted_index = k;
    avlNode<T> current_node = this->getRoot();
    int current_left_rank = this->getLeft() ? current_node.getLeft().getRank() : 0;
    while(current_left_rank != wanted_index){
        if(current_left_rank > k-1){
            current_node = current_node.getLeft();    
        }
        else{
            wanted_index -= (current_left_rank + 1)
            current_node = current_node.getRight();
        }
        current_left_rank = this->getLeft() ? current_node.getLeft().getRank() : 0;
    }
}
#endif