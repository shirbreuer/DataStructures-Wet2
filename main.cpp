#include "avl_tree.hpp"
#include "avl_node.hpp"
#include "stdio.h"

void print(avlNode<int> *root)
{
    std::cout << *((root)->getValue()) << " ";
}

class int_test
{
private:
    int num;

public:
    int_test(int key = 0) : num(key);
    ~int_test() = default;
};

int main()
{
    // avlTree<int> tree;
    // int a=0, b=1, c=2, d=3, e=4;
    // int* const p=&a;
    // tree.insert(p);
    // tree.insert(&b);
    // tree.insert(&c);
    // tree.insert(&d);
    // tree.insert(&e);
    // tree.inOrder(tree.getRoot(), print);

    // hashTable<int> a;
    // a.add(15, hash);
    hashTable<int> *b = new hashTable<int>(0.5f, 0.75f);
    for (int i = 0; i < 1500; i++)
    {
        // int c = b->getCapacity();
        // std::cout << i << "           ";
        b->add(8 + 16 * i);
        // if (c != b->getCapacity())
        // std::cout << std:: endl << "New capacity: " << b->getCapacity() << std:: endl;
    }
    // for (int i = 0; i < 1500; i++)
    //     {
    //         // std::cout << i << "           ";
    //         b->remove(8+16*i, hash);
    //     }
    // std::cout << hash(1, 12) << std::endl;
    // std::cout << a.getCapacity() << std::endl;
    // std::cout << a.getSize() << std::endl;
    std::cout << b->getCapacity() << std::endl;
    std::cout << b->getSize() << std::endl;
    delete b;
    return 0;
}