#include "avl_tree.hpp"
#include "avl_node.hpp"
#include "stdio.h"

void print(avlNode<int> *root)
{
    std::cout << *((root)->getValue()) << " ";
}

int main(){
    avlTree<int> tree;
    int a=0, b=1, c=2, d=3, e=4;
    int* const p=&a;
    tree.insert(p);
    tree.insert(&b);
    tree.insert(&c);
    tree.insert(&d);
    tree.insert(&e);
    tree.inOrder(tree.getRoot(), print);
    return 0;
}