#include <iostream>
#include "rbtree.hpp"
//#include "avltree.hpp"
#include "splaytree.hpp"
#include "bst.hpp"
#include "range.hpp"
#include "aatree.hpp"
//#include "binomial_heap.hpp"
#include "fibonacci_heap.hpp"

#include <iterator>
#include <algorithm>

struct A
{
    A& operator=(const int& i)
    {
        std::cout << "A" << std::endl;
        return *this;
    }
};
struct B : public A
{
    B& operator=(const int& i)
    {
        std::cout << "B" << std::endl;
        return *this;
    }
};

int main()
{

//    B b;
//    b = 1;
//    std::cout << "test" << std::endl;
//    tf::xrange<int> r(0, 20, 1);
//    tf::xrange<int> r1(r);
//    tf::xrange<int> e;


    //tf::binomial_heap<int> tree;
    tf::fibonacci_heap<int> tree;
    tf::fibonacci_heap<int> tree1;
//	auto l = tree.head;
	std::vector<tf::fibonacci_heap<int>::link_t> vec;
    for (int i = 0; i < 5; ++i)
    {
		auto link = tree.insertOneNode(i);
		tree1.insertOneNode(i);
		vec.push_back(link);
		tree.display();	
    }
    std::cout << "======================" << std::endl;
	tree.consolidate();
    tree.display();	
	tree.decreaseKey(vec[2], -1);
	tree.display();
	tree.decreaseKey(vec[1], -2);
	tree.display();

	tree.mergeWithAnother(tree1);
    std::cout << "======================" << std::endl;
	tree.display();
	tree1.display();
	tree.consolidate();
	tree.display();
    

//	while (tree.cnt)
//	{
//		tree.extractMinimum();
//		tree.display();
//	}
    
    std::cin.get();
	//tree.extractMinimum();
	////tree.mergeWithAnother(tree1);
  //  std::cout << "========+++++++++++++++++++++++++++++==========" << std::endl;
  //  tree.treeShap();
  //  std::cout << "========+++++++++++++++++++++++++++++==========" << std::endl;
  //  tree1.treeShap();
  //  std::cin.get();
//    auto inserter = tf::bst_multi_inserter_iterator(tree);
//    std::cout << typeid(*inserter).name() << std::endl;
//    std::copy(r, e, inserter);
//    std::copy(r1, e, inserter);
   // std::copy(r, e, tf::bst_unique_inserter_iterator(tree));


//    const int N = 200;
//    for (int i = 0; i < N; i++)
//    {
//        int added = r();
//        std::cout << "-----------------------------------------" << std::endl;
//        std::cout << "add\t" << added << std::endl;
//        tree.insertOneNode(added, true);
//        std::cout << "size: " << tree.size() << std::endl;

//    }


//    for (int i = 0; i < N; ++i)
//    {
//         std::cout << "-----------------------------------------" << std::endl;
//         std::cout << "del cnt = " << i << std::endl;
//         std::cout << "size: " << tree.size() << std::endl;
//  //      tree.treeShap();
//        std::cout << "delete root()" << std::endl;
//        tree.deleteOneNode(tree.root());

//        if (!tree.isAATree(tree.root()))
//        {
//            std::cout << "not aatree1" << std::endl;
//            tree.treeShap();
//            return 0;
//        }
//    }

    return 0;
}

