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
    this->left = new_left;
}

template <class T>
void avlNode<T>::setRight(avlNode<T> *new_right)
{
    if (new_right)
        new_right->setParent(this);
    this->right = new_right;
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

        node->setRight(this);
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

        node->setLeft(this);
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

#endif
