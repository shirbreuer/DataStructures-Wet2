#ifndef AVL_NODE_HPP
#define AVL_NODE_HPP

#include "avl_node.h"

template <class T>
avlNode<T>::avlNode(T* const value) : data(value), left(NULL), right(NULL), parent(NULL)
{
}

template <class T>
avlNode<T>::avlNode(const avlNode<T> *node)
{
    this->data = node->getValue();
    this->left = node->getLeft();
    this->right = node->getRight();
    this->parent = node->getParent();
    this->rank=getRight().getRank()+getLeft().getRank()+1;
}

template <class T>
avlNode<T> *avlNode<T>::getLeft()
{
    return this->left;
}

template <class T>
avlNode<T> *avlNode<T>::getRight()
{
    return this->right;
}

template <class T>
avlNode<T> *avlNode<T>::getParent()
{
    return this->parent;
}

template <class T>
T* avlNode<T>::getValue()
{
    return this->data;
}

template <class T>
void avlNode<T>::setValue(T* const value)
{
    this->data = value;
}

template <class T>
const T* avlNode<T>::getValue() const
{
    return this->data;
}

template <class T>
void avlNode<T>::setLeft(avlNode<T> *new_left)
{
    if (new_left)
        new_left->setParent(this);
    int oldLeftRank = left->getRank();
    int newLeftRank = new_left->getRank();
    int newRank = getRank() - oldLeftRank + newLeftRank;
    this->left = new_left;
    this->rank = newRank;
}

template <class T>
void avlNode<T>::setRight(avlNode<T> *new_right)
{
    if (new_right)
        new_right->setParent(this);
    int oldRightRank = right->getRank();
    int newRightRank = new_right->getRank();
    int newRank = getRank()-oldRightRank+newRightRank;
    this->right = new_right;
    this->rank = newRank;
}

template <class T>
void avlNode<T>::setParent(avlNode<T> *new_parent)
{
    this->parent = new_parent;
}

template <class T>
bool avlNode<T>::isLeftChild()
{
    if (this->getParent()->getLeft())
    {
        if (this->getParent()->getLeft() == this)
            return true;
        if (*(this->getParent()->getLeft()->getValue()) == *(this->getValue()))
            return true;
    }
    return false;
}

template <class T>
bool avlNode<T>::isRightChild()
{
    if (this->getParent()->getRight())
    {
        if (this->getParent()->getRight() == this)
            return true;
        if (*(this->getParent()->getRight()->getValue()) == *(this->getValue()))
            return true;
    }
    return false;
}

template <class T>
void avlNode<T>::copyFrom(avlNode<T> *node)
{
    this->setRight(node->getRight());
    this->setLeft(node->getLeft());
    this->rank = node->getRank();
    if (node->getParent())
    {
        if (node->isLeftChild())
        {
            node->getParent()->setLeft(this);
        }
        else
        {
            node->getParent()->setRight(this);
        }
    }
    else
    {
        this->setParent(NULL);
    }
}

template <class T>
void avlNode<T>::swapWithChild(avlNode<T> *node, bool is_right)
{
    // std::cout << "swapping with: " << (is_right? "right " : "left ") << " child.!!!!" << std::endl;
    // std::cout << "!@#!@#!@#!@#";
    // node is either right or left child of this
    avlNode<T> *temp_right = node->getRight();
    avlNode<T> *temp_left = node->getLeft();
    avlNode<T> *temp_this_left = this->getLeft();
    int myRank=rank, rightRank=getRight()->getRank(), leftRank=getLeft()->getRank();
    if (is_right)
    {
        // std::cout << "Is RIGHT!" << std::endl;
        node->setLeft(this->getLeft());
        if (this->getParent())
        {
            if (this->isLeftChild())
            {
                this->getParent()->setLeft(node);
            }
            else
            {
                this->getParent()->setRight(node);
            }
        }
        else
            node->setParent(NULL);
        rank = rightRank;
        node->setRight(this);
        node->rank = myRank;
        this->setRight(temp_right);
        this->setLeft(temp_left);
    }
    else
    {
        // std::cout << "Is LEFT!" << std::endl;
        node->setRight(this->getRight());
        if (this->getParent())
        {
            if (this->isLeftChild())
            {
                this->getParent()->setLeft(node);
            }
            else
            {
                this->getParent()->setRight(node);
            }
        }
        else
            node->setParent(NULL);
        rank = leftRank;
        node->setLeft(this);
        node->rank = myRank;
        this->setRight(temp_right);
        this->setLeft(temp_left);
    }
    if (temp_this_left != node->getLeft())
        exit(2);
}

template <class T>
void avlNode<T>::setHeight()
{
    int left = this->getLeft() ? this->getLeft()->getHeight() : -1;
    int right = this->getRight() ? this->getRight()->getHeight() : -1;
    this->height = 1 + ((left > right) ? left : right);
}

template <class T>
void avlNode<T>::print() const
{
    std::cout << data << std::endl;
}


template <class T>
int avlNode<T>::getHeight()
{
    return this->height;
}

template <class T>
int avlNode<T>::getRank()
{
    return this->rank;
}

#endif
