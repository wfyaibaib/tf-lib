#include <iostream>
#include "rbtree.hpp"
#include "avltree.hpp"
#include "splaytree.hpp"
#include "bst.hpp"
#include "range.hpp"
#include <iterator>
#include <algorithm>

int main()
{
    std::copy(xrange<float>(0.5, -10, -1), xrange<float>(),
              std::ostream_iterator<float>(std::cout, " "));
    tf::rbtree<int> tree;
    const int N = 10;
    for (int i = 0; i < N; i++)
    {
        tree.insertOneNode(i);
        std::cout << "-----------------------------------------" << std::endl;
        std::cout << "add\t" << i << std::endl;
        tree.treeShap();
//        bstDisplayNodeRecusive(tree.root(), tree.head);
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

