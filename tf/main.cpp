#include <iostream>
#include "rbtree.hpp"
#include "avltree.hpp"
#include "splaytree.hpp"
#include "bst.hpp"
#include "range.hpp"
#include "aatree.hpp"

#include <iterator>


int main()
{

    std::cout << "test" << std::endl;
    tf::xrandom r(0, 20);


    tf::aatree<int> tree;
    const int N = 500;
    for (int i = 0; i < N; i++)
    {
        int added = r();
        std::cout << "-----------------------------------------" << std::endl;
        std::cout << "add\t" << added << std::endl;
        tree.insertOneNode(added);
        std::cout << "size: " << tree.size() << std::endl;
        if (!tree.isAATree(tree.root()))
        {
            std::cout << "not aatree1" << std::endl;
            return 0;
        }
    }
    for (int i = 0; i < N; ++i)
    {
         std::cout << "-----------------------------------------" << std::endl;
         std::cout << "del cnt = " << i << std::endl;
         std::cout << "size: " << tree.size() << std::endl;
  //      tree.treeShap();
        std::cout << "delete root()" << std::endl;
        tree.deleteOneNode(tree.root());

        if (!tree.isAATree(tree.root()))
        {
            std::cout << "not aatree1" << std::endl;
            tree.treeShap();
            return 0;
        }
    }

    return 0;
}

