#ifndef BINOMIAL_HEAP_H
#define BINOMIAL_HEAP_H

#include "node.hpp"
#include "bst.hpp"
#include "hasvalue.hpp"
#include <algorithm>
#include <iostream>
namespace tf{
/**************************************************************************/
template <class Node>
struct binomial_heap_node_base : public tlink<Node>
{
    size_t h;// height, from 0
    binomial_heap_node_base():h(0){}
};
template <class Value>
struct binomial_heap_node : public binomial_heap_node_base<binomial_heap_node<Value> > ,
                 public has_value<Value>
{
    binomial_heap_node(const Value& value = Value()) : has_value<Value>(value) {}
    std::string valueToString() // implement
    {
        return "(" + to_string(this->v) + ", " + to_string(this->h) + ")";
    }
};
//template <class AVL_Node_Base>
//int height(AVL_Node_Base* p) {return p->h;}
/**************************************************************************/
template <class Value, class Cmp = std::less<Value> >
class binomial_heap : public bst_base<binomial_heap_node<Value>, Cmp>
{
    /*
    bst_base<binomial_heap_node<Value>, Cmp>:
        countable:
             size_t cnt;
        link_t this->head;            // binomial_heap_node<Value>*
        Cmp cmp;
    */

public:
    typedef binomial_heap_node<Value> node_t;
    typedef binomial_heap_node<Value>* link_t;

    binomial_heap()
    {
        this->head->p = this->head->l = this->head->r = 0;
        this->head->h = 0;
    }
    ~binomial_heap() {}
    link_t insertOneNode(const Value & value)
    {
        //std::cout << "heap insert one node " << std::endl;
        link_t newNode = bstGetNewNodeAsLeaf(value, link_t(0));
        newNode->h = 0;

        link_t old_root = this->root();// may be 0

        this->head->p = newNode;
        newNode->p = this->head;

        newNode->r = old_root;
        if (old_root != 0)
		{
            old_root->p = newNode;
		}

        this->increaseCnt();
        //this->treeShap();
        mergeSelf();
        return newNode;
    }
    void deleteOneNodeInFirstLevel(link_t pnode)
	{
		//std::cout << "deleteOneNodeInFirstLevel :" << pnode->valueToString() << std::endl;
        //treeShap();
        if (empty()) return;

        link_t min = pnode;
        link_t min_r = right(min);
        link_t min_p = parent(min);
        if (min == this->root())
		{
            this->head->p = min_r; //new root
            if (min_r != 0)
			{
                min_r->p = this->head;
			}
		}
		else
		{
            min_p->r = min_r;
            if (min_r  != 0)
			{
                 min_r->p = min_p;
			}
		}

        // reverse min list
        if (height(min) == 0)// left(min) == 0
		{
            --this->cnt;
            return ;
		}
		else// >=1 node
		{
            link_t first = left(min);
            link_t newRoot = first;
            // p r reverse from [first, 0)
            if (right(first) == 0)// only one node
			{// reversed 
                newRoot = first;
			}
			else
			{
                link_t p1 = first;
                link_t p2 = right(p1);
				while(p2 != 0)
				{
                    link_t p3 = right(p2);
                    p1->p = p2;
                    p2->r = p1;

                    p1 = p2;
                    p2 = p3;
				}
                first->r = 0;
                newRoot = p1;
//std::cout << "after reverse: list: " << std::endl;
//bstDisplayNodeRecusive(newRoot, link_t(0));
//std::cout << "-----------------------" << std::endl;
//treeShap();

			}
            mergeWithChain(newRoot, 0);
		}
        --this->cnt;
	}

    void extractMinimum()
	{
        if (empty()) return;
        link_t min = minimum();
        deleteOneNodeInFirstLevel(min);
	}

    link_t mergeSame(link_t x, link_t y)
    {
		
        if (!this->cmp(x->v, y->v))// x < y
        {
            link_t tmp = x;
            x = y;
            y = tmp;
        }
		//std::cout << "mergeSame: x:" <<  x->valueToString() << "  y:" << y->valueToString() << std::endl;

        if (height(x) == 0)
        {
            x->l = y;
            y->p = x;
            ++x->h;
			y->r = 0;// old x->l is head
            return x;
        }
        else
        {
            link_t xl = left(x);
            x->l = y;
            y->p = x;
            y->r = xl;
            xl->p = y;
            
            ++x->h;
            return x;
        }
    }
    void mergeSelf()
    {
        //std::cout << "merge self" << std::endl;
		//treeShap();
        link_t b = this->root();
        while (1)
        {// [head][0, head]
			//std::cout << b->valueToString() << std::endl;
            //this->treeShap();
            if (b == 0 || right(b) == 0)
            {
                //std::cout << "[head][0, head]" << std::endl;
                return;
            }
            else
            {
                link_t br = right(b);
                if (height(b) != height(br))// [0, 1...]
                {
                    //std::cout << "[0, 1...]" << std::endl;
                    b = br;
                    continue;
                }
                else// [0, 0, head][0, 0, 1..]
                {
                    //std::cout << "[0, 0, head][0, 0, 1..]" << std::endl;
                    if (right(br) == 0 || height(right(br)) != height(br))
                    {// merge [
                        //std::cout << "[0, 0, head][0, 0, 1..]" << std::endl;
                        link_t bp = parent(b);
                        link_t brr= right(br);

                        link_t merged = this->mergeSame(b, br);
                        // bp (merged) brr
                        if (bp == this->head)
                        {
                            this->head->p = merged;
                            merged->p = this->head;
                        }
                        else
                        {
                            merged->p = bp;
                            bp->r = merged;
                        }

                        merged->r = brr;
                        if (brr != 0)
                        {
                            brr->p = merged;
                        }
                        b = merged;
                        // [1, head][ 1, 1, 1..][1, 1, 2..]
                        continue;
                    }
                    else//[1, 1, 1..]
                    {
                        b = br;
                        continue;
                    }

                }
            }
        }
    }

    link_t minimum()
    {
        if (this->empty()) return this->head;
        else
        {
            link_t min = this->root();
            for (link_t p = right(root()); p != 0; p = right(p) )
            {
                if (cmp(p->v, min->v))
                {
                    min = p;
                }
            }
            return min;
        }
    }
     void treeShap()
    {
        nodeShapRecusive(root(), 0);
    }
    void mergeWithChain(link_t b, size_t b_cnt)
	{
        if (b == 0) return;// empty list
        if (this->empty())// self is empty
		{
            this->head->p = b;
            b->p = this->head;
            // cnt?
            this->cnt += b_cnt;
            return;
		}
		else// both not empty
		{
            this->cnt += b_cnt;

            link_t this_right_most = bstRightMost(this->root(), link_t(0));
            link_t another_right_most = bstRightMost(b, link_t(0));
            //another_right_most->r = this->head;
            //merge
            link_t p1 = this->root();
            link_t p2 = b;

            while(p1 != 0 && p2 != 0)
            {
                //std::cout << "-----------------------------------------" << std::endl;
                //this->treeShap();

                if (height(p1) <= height(p2))
                {
                    p1 = right(p1);
                }
                else
                {
                    link_t b = p2;
                    while (p2 != 0 && height(p2) < height(p1))//bug!!!
                    {
                        p2 = right(p2);
                    }
                    // p1 may be root 
                    link_t p1p = parent(p1);// p1p may be head
                    // p2 may be 0
					link_t p2p = p2 ? parent(p2): another_right_most;
                   
                    // p1p (b p2p) p1
//					std::cout << "p1p: " << p1p->valueToString() << std::endl;
//					std::cout << "b: " << b->valueToString() << std::endl;
//					std::cout << "p2p: " << p2p->valueToString() << std::endl;
//					std::cout << "p1: " << p1->valueToString() << std::endl;
//					std::cout << "p2: " << p2->valueToString() << std::endl;

                    if (p1p == this->head) { p1p->p = b; }
					else { p1p->r = b; }
                    b->p = p1p;
                    p2p->r = p1;
                    p1->p = p2p;
                }
            }
            if (p2 != 0)
            {//this_right_most (p2 another_right_most) 0
                this_right_most->r = p2;
                p2->p = this_right_most;
                another_right_most->r = 0;
            }
		}
        mergeSelf();
	}
    void mergeWithAnother(binomial_heap<Value>& another)
    {
        if(another.empty()) return;
        mergeWithChain(another.root(), another.size());

        another.head->p = another.head->l = another.head->r = 0;
        another.cnt = 0;
    }
    link_t getRealParent(link_t son)
	{
		//std::cout << "getReal son " << son->valueToString() << std::endl; 
        if (son == this->head)
		{
            return this->head;
		}
		else
		{
            link_t s = son;
            link_t p = parent(son);
            //std::cout << p->valueToString() << std::endl;
			//std::cout << s->valueToString() << std::endl;
            while (p != this->head && s == right(p))
			{
                //std::cout << "while " << std::endl;
				//std::cout << p->valueToString() << std::endl;
				//std::cout << s->valueToString() << std::endl;
                s = p;
                p = parent(s);
			}
            return p;
		}
	}
    void decreaseKey(link_t pnode, const Value& newValue)
    {
        if (pnode == this->head || !cmp(newValue, pnode->v)) return;
		else
		{
            link_t p = getRealParent(pnode);
            pnode->v = newValue;
            while (p != this->head && cmp(pnode->v, p->v))
			{// swap
                Value tmp = pnode->v;
                pnode->v = p->v;
                p->v = tmp;

                pnode = p;
                p = getRealParent(pnode);
		    }
		}
	}
    void deleteOneNode(link_t pnode)
	{
        //std::cout << pnode->valueToString() << std::endl;
        if (pnode == this->head) return;
		else
		{
            link_t p = getRealParent(pnode);
            //std::cout << p->valueToString() << std::endl;
            while (p != this->head)
			{
				//std::cout << p->valueToString() << std::endl;
                pnode->v = p->v;
                
                pnode = p;
				p = getRealParent(pnode);
			}
            deleteOneNodeInFirstLevel(pnode);
		}
	}
};
}
#endif // BINOMIAL_HEAP_H
