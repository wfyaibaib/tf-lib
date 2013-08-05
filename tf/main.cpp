#include <iostream>
#include "rbtree.hpp"
#include "avltree.hpp"
#include "splaytree.hpp"
#include "bst.hpp"
#include "range.hpp"
#include <iterator>


int main()
{

    tf::splaytree<int> tree;
    const int N = 10;
    for (int i = 0; i < N; i++)
    {
        tree.insertOneNode(i);
        std::cout << "-----------------------------------------" << std::endl;
        std::cout << "add\t" << i << std::endl;
        tree.treeShap();
   //     bstDisplayNodeRecusive(tree.root(), tree.head);
    }
    for (tf::splaytree<int>::iterator it = tree.begin(); it != tree.end(); ++it)
    {
        std::cout << *it << std::endl;
    }
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

