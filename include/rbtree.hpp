#ifndef RBTREE_HPP
#define RBTREE_HPP
#include "bst.hpp"
#include "string.hpp"
#include <string>
#include <iostream>
#include <functional>
#include "hasvalue.hpp"
namespace tf
{
/**************************************************************************/

template <class Node>
struct rbnode_base : public tlink<Node>
{
    bool c;// 0:red,1:black
    rbnode_base():c(false){}// red node
};
template <class RB_Node_Base>
bool color(RB_Node_Base *p) {return p->c;}

template <class Value>
struct rbnode : public rbnode_base<rbnode<Value> >,
                public has_value<Value>
{
    rbnode(const Value& value = Value()) : has_value<Value>(value) {}
    std::string valueToString()
    {
        return "(" + to_string(this->v) + ", " + (this->c ? "b" : "r") +")";
    }
};

/**************************************************************************/

template <class Value, class Cmp = std::less<Value> >
class rbtree : public bst_base<rbnode<Value>, Cmp >
{
public:
//  typedef rbnode<Value> rbnode; g++4.6 not support
    typedef rbnode<Value>   node_t;
    typedef rbnode<Value>* link_t;
// g++4.6 not support std::to_string<>

    rbtree()
    {
        this->head->p = this->head->l = this->head->r = this->head;
        this->head->c = true;// this->head is black
    }

    link_t insertOneNode(const Value& value, bool unique = false)
    {
        // find new node's parent
        link_t p = findInsertPosition(value);
        if (p == this->head)
        {// insert as root
            // new node will be leaf node
            link_t newnode = getNewNodeAsLeaf(value);
            this->head->p = newnode;
            newnode->p = this->head;
            this->increaseCnt();
            insertAdjust(newnode);
            return newnode;
        }
        else
        {
            if (unique)
            {
                link_t prevNodeOfInsert = p;
                if (cmp(value, p->v))// insert as left child
                {
                    prevNodeOfInsert = prev(p);
                }
                if (prevNodeOfInsert != this->head &&
                        !cmp(value, prevNodeOfInsert->v) && // value >=
                        !cmp(prevNodeOfInsert->v, value)      //
                        )
                    return 0;
            }

            link_t newnode = getNewNodeAsLeaf(value);
            newnode->p = p;
            if (cmp(value, p->v))
                p->l = newnode;
            else
                p->r = newnode;
            this->increaseCnt();
            insertAdjust(newnode);
            return newnode;

        }
    }
    void insertAdjust(link_t newnode)
    {
        if (newnode->p == this->head)
        {
            newnode->c = true;// black
            return;
        }
        else
        {
            newnode->c = false;//red
            if (parent(newnode)->c == true) return;// parent is black
            else// parent is red, so newnode has black grandparent pp,
            {
                link_t p = parent(newnode);
                link_t pp = parent(p);
                link_t u = uncle(newnode);
                if (u->c == false)// uncle is red
                {
                    p->c = u->c = true;
                    pp->c = false;
                    insertAdjust(pp);
                }
                else// uncle is black
                {
                    if (p == left(pp))// l ?
                    {
                        if (newnode == right(p))
                        {// l r -> l l
                            leftRotation(p);
                            rightRotation(pp);
                            pp->c = false;//red
                            newnode->c = true;
                        }
                        else
                        {// l l
                            rightRotation(pp);
                            pp->c = false;
                            p->c = true;
                        }
                    }
                    else // r ?
                    {
                        if (newnode == left(p))
                        {// r l
                            rightRotation(p);
                            leftRotation(pp);
                            pp->c = false;
                            newnode->c = true;
                        }
                        else
                        {// r r
                            leftRotation(pp);
                            pp->c = false;
                            p->c = true;
                        }
                    }
                }
            }

        }
    }

    void deleteOneNode(link_t del)
    {
        link_t d = del;
        if (del->l != this->head && del->r != this->head)//
        {
            d = next(del);
            del->v = d->v;// copy data from next
        }
        // if left(del) && right(del) then left(d)==this->head , right(d) == red/this->head
        // if left(del)

        link_t n = right(d) == this->head ? left(d) : right(d);//
        link_t p = parent(d);
        if (d == left(p)) p->l = n;
        else if (d == right(p)) p->r = n;
        else p->p = n;
        if (n != this->head) n->p = p;


        bool dc = d->c;
        this->decreaseCnt();
        delete d;

        if (dc == true) deleteAdjust(n, p);


    }
    void deleteAdjust(link_t n, link_t np)// n subtree decrease one black
    {
        if (this->empty()) return;// delete only one node : root
        else
        {
            if (n->c == false)// replaced node is red
            {
                n->c = true;
                return;
            }
            else// replaced node is bottom black this->head
            {// uncle subtree has a black node
                while (1)
                {
                    if (n == this->root())
                    {
                        //std::cout << "case del root" << std::endl;
                        if (n->c == false)
                            n->c = true;
                        return;
                    }
                    link_t p =  n == this->head ? np : parent(n);
                    link_t s = n == left(p) ? right(p) : left(p);
                    link_t sl = left(s);
                    link_t sr = right(s);
                    int state = 8*p->c + 4*s->c + 2*sl->c + sr->c;
                    switch(state)
                    {
                    case 11://1011:wiki-case 2:clrs-case 1
                        if (n == left(p))
                        {
                        //std::cout << "n is left child,case 1011" << std::endl;
                              leftRotation(p);
                              s->c = true;
                              sl->c = false;
                        }
                        else
                        {
                        //std::cout << "n is right child,case 1011" << std::endl;
                            rightRotation(p);
                            s->c = true;
                            sr->c = false;
                        }
                        return;
                    case 15://1111 wiki-case 3, clrs-case 2(1)
                        //std::cout << "case 1111" << std::endl;
                        s->c = false;
                        n = p;
                        continue;
                    case 7://0111 wiki-case 4, clrs-case 2(2)
                        //std::cout << "case 0111" << std::endl;
                        p->c = true;
                        s->c = false;
                        return;
                    default:
                        // wiki-case 5, clrs-case 4
                        if (left(p) == n)
                        {
                            switch (state)
                            {
                            case 5:// *101
                            case 13:
                        //std::cout << "n is left child,case *101" << std::endl;
                                rightRotation(s);
                                leftRotation(p);
                                sl->c = p->c;
                                p->c = true;
                                return;
                            case 4:// *1*0
                            case 6:
                            case 12://
                            case 14:
                        //std::cout << "n is left child,case *1*0" << std::endl;
                                leftRotation(p);
                                s->c = p->c;
                                p->c = true;
                                sr->c = true;
                                return;
                            }
                        }
                        else
                        {
                            switch (state)
                            {
                            case 6://0110
                            case 14://1110
                        //std::cout << "n is right child,case *110" << std::endl;
                                leftRotation(s);
                                rightRotation(p);
                                sr->c = p->c;
                                p->c = true;
                                return;
                            case 4:// *10*
                            case 5:
                            case 12:
                            case 13:
                        //std::cout << "n is right child,case *10*" << std::endl;
                                rightRotation(p);
                                s->c = p->c;
                                p->c = true;
                                sl->c = true;
                                return;
                            }
                        }
                    }
                }
            }

        }
    }

};

}
#endif // RBTREE_HPP
