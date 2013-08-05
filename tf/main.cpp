#include <iostream>
#include "rbtree.hpp"
#include "avltree.hpp"
#include "splaytree.hpp"
#include "bst.hpp"

int main()
{
    tf::splaytree<float> tree;
    const int N = 10;
    for (int i = 0; i < N; i++)
    {
        tree.insertOneNode(i + 0.1);
        std::cout << "-----------------------------------------" << std::endl;
        std::cout << "add\t" << i << std::endl;
//        tree.treeShap(tree.root());
        tree.treeShap();
    //    std::cout << tree.nodeToStringRecusive(tree.root());
        std::cout << std::endl;
    //    bstDisplayNodeRecusive(tree.root(), tree.head);
    }

//    for (int i = 0; i < N; i++)
//    {
//        std::cout << "-----------------------------------------" << std::endl;
//        std::cout << "delete root node: cnt = " << i << std::endl;
//        tree.deleteOneNode(tree.root());
//        tree.treeShap(tree.root());
//    }

    //tree.leftRotation(tree.root());
    //tree.leftRotation(tree.root());
    //tree.rightRotation(tree.root());
    //tree.rightRotation(tree.root());


    return 0;
}

