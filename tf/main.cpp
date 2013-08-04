#include <iostream>
#include "rbtree.hpp"
#include "avltree.hpp"

int main()
{
    tf::avltree<int> tree;
    const int N = 10;
    for (int i = 0; i < N; i++)
    {
        tree.insertOneNode(i);
        std::cout << "-----------------------------------------" << std::endl;
        std::cout << "add\t" << i << std::endl;
        tree.treeShap(tree.root());
    }

    for (int i = 0; i < N; i++)
    {
        std::cout << "-----------------------------------------" << std::endl;
        std::cout << "delete root node: cnt = " << i << std::endl;
        tree.deleteOneNode(tree.root());
        tree.treeShap(tree.root());
    }

    //tree.leftRotation(tree.root());
    //tree.leftRotation(tree.root());
    //tree.rightRotation(tree.root());
    //tree.rightRotation(tree.root());


    return 0;
}

