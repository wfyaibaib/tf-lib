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
    const int N = 30;
    for (int i = 0; i < N; i++)
    {
        int added = r();
        std::cout << "-----------------------------------------" << std::endl;
        std::cout << "add\t" << added << std::endl;
        tree.insertOneNode(added);

 //       tree.treeShap();
    }
    tree.treeShap();
//    for (tf::rbtree<int>::iterator it = tree.begin(); it != tree.end(); ++it)
//    {
//        std::cout << *it << std::endl;
//    }

  //  copy(tree.begin(), tree.end(), std::ostream_iterator<int>(std::cout, " "));

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

