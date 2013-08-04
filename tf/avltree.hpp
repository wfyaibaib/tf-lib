#ifndef AVLTREE_HPP
#define AVLTREE_HPP

#include <node.hpp>
#include <string>
#include "string.hpp"
#include <functional>
#include <iostream>

namespace tf
{

/**************************************************************************/
template <class Node>
struct avlnode_base : public tlink<Node>
{
    size_t h;// height
    avlnode_base():h(1){}
};
template <class Value>
struct avlnode : public avlnode_base<avlnode<Value> >
{
    typedef Value value_type;
    Value v;
    avlnode(const Value& value = Value()) : v(value) {}
};
template <class AVL_Node_Base>
int height(AVL_Node_Base* p) {return p->h;}
/**************************************************************************/
template <class Value, class Cmp = std::less<Value> >
class avltree
{
public:
    typedef avlnode<Value> node_t;
    typedef avlnode<Value>* link_t;

    int factor(link_t pnode)
    {
        return height(left(pnode)) - height(right(pnode));
    }

    std::string display_recusive(link_t root)
    {
        if (root == head) return "[None]";
        else return
            (
            "[" +
            display_recusive(root->l) +
            " (" +
            to_string(root->v) + ", " + to_string(root->h) +
            ") " +
            display_recusive(root->r) +
            "]"
            );
    }

    link_t leftmost(link_t pnode) { return leftMost(pnode, head); }
    link_t rightmost(link_t pnode) { return rightMost(pnode, head); }
    void leftRotation(link_t n) { bstLeftRotation(n, head); }
    void rightRotation(link_t n) { bstRightRotation(n, head); }
    bool isRoot(link_t pnode) { return parent(pnode) == head; }
    bool empty() { return cnt == 0; }
    link_t root() { return parent(head); }
    link_t getNewNodeAsLeaf(const Value& value) { return bstGetNewNodeAsLeaf(value, head); }

    std::string display() { return display_recusive(root()); }


    link_t head;
    Cmp cmp;
    size_t cnt;

    avltree()
    {
        cmp = Cmp();

        head = new node_t();
        head->p = head->l = head->r = head;
        head->h = 0;

        cnt = 0;
    }
    ~avltree()
    {
        if (!empty()) bstDeleteNodeRecusive(head->p, head);
        delete head;
    }


    link_t findInsertPosition(const Value& value) const
    {
        return bstFindInsertPosition(head->p, head, value, cmp);
    }
    /**
     * @brief insertOneNode :
     * @param value : the value to be insert
     * @param unique
     * @return
     */
    link_t insertOneNode(const Value& value, bool unique = false)
    {
//        std::cout << "insertOneNode: " << value << std::endl;
        // find new node's parent
        link_t p = findInsertPosition(value);
        if (p == head)
        {// insert as root
            // new node will be leaf node
            link_t newnode = getNewNodeAsLeaf(value);
            head->p = newnode;
            newnode->p = head;
            cnt++;
            insertAdjust(newnode);
            return newnode;
        }
        else
        {
            if (unique && !cmp(value, p->v) && !cmp(p->v, value)) return 0;
            else// multi set
            {
                link_t newnode = getNewNodeAsLeaf(value);
                newnode->p = p;
                if (cmp(value, p->v))
                    p->l = newnode;
                else
                    p->r = newnode;
                cnt++;
                insertAdjust(newnode);
                return newnode;
            }
        }
    }
    /**
     * @brief insertAdjust
     * @param newnode
     */
    bool adjustHeight(link_t adjustNode, link_t changedChild)
    {
        if (height(adjustNode) == height(changedChild))
        {
            ++(adjustNode->h);
            return true;
        }
        else return false;
    }


    void insertAdjust(link_t newnode)
    {
        newnode->h = 1;//

        link_t changedChild = newnode;
        link_t adjustNode = parent(changedChild);

        while (adjustNode != head)
        {
            if (adjustHeight(adjustNode, changedChild))// adjustNode's height changed
            {
                //                    std::cout << "\tadjustNode: " << adjustNode->v << std::endl;
                //                    std::cout << "\tchangedChild: " << changedChild->v << std::endl;

                //                    std::cout << "\tfactor :" << factor(adjustNode) << std::endl;
                if (factor(adjustNode) == -2)// right
                {

                    if (height(left(changedChild)) == height(changedChild) - 1)// right left
                    {
                        //                            std::cout << "rl" << std::endl;
                        link_t changedChildLeft = left(changedChild);
                        bstRightRotation(changedChild, head);
                        bstLeftRotation(adjustNode, head);

                        ++(changedChildLeft->h);
                        --(changedChild->h);
                        adjustNode->h = adjustNode->h - 2;

                        changedChild = changedChildLeft;
                        adjustNode = parent(changedChild);
                    }
                    else// right right
                    {
                        //                            std::cout << "rr" << std::endl;
                        bstLeftRotation(adjustNode, head);
                        adjustNode->h = adjustNode->h - 2;

                        adjustNode = parent(changedChild);

                    }
                }
                else if (factor(adjustNode) == 2)// left
                {

                    if (height(right(changedChild)) == height(changedChild) - 1)// left right
                    {
                        //                            std::cout << "lr" << std::endl;
                        link_t changedChildRight = right(changedChild);
                        bstLeftRotation(changedChild, head);
                        bstRightRotation(adjustNode, head);

                        ++(changedChildRight->h);
                        --(changedChild->h);
                        adjustNode->h = adjustNode->h - 2;

                        changedChild = changedChildRight;
                        adjustNode = parent(changedChild);
                    }
                    else// left left
                    {
                        //                            std::cout << "ll" << std::endl;
                        bstRightRotation(adjustNode, head);
                        adjustNode->h = adjustNode->h - 2;

                        adjustNode = parent(changedChild);
                    }
                }
                else// factor = -1, 0, 1
                {
                    changedChild = adjustNode;
                    adjustNode = parent(adjustNode);
                }
            }
            else return;// adjustNode's height not change
        }
    }
    void treeShap(link_t subtree ,size_t table_cnt = 0)
    {
        if (subtree == head) std::cout << std::string(table_cnt, '\t') << "[None]" << std::endl;
        else
        {

            treeShap(right(subtree), table_cnt + 1);
            std::cout << std::string(table_cnt, '\t') + "(" + to_string(subtree->v) + ", " + to_string(subtree->h) + ")" << std::endl;
            treeShap(left(subtree), table_cnt + 1);
        }
    }
/*
    void deleteOneNode(link_t del)
    {
        link_t d = del;
        if (del->l != head && del->r != head)//
        {
            d = next(del);
            del->v = d->v;// copy data from next
        }
        // if left(del) && right(del) then left(d)==head , right(d) == red/head
        // if left(del)

        link_t n = right(d) == head ? left(d) : right(d);//
        link_t p = parent(d);
        if (d == left(p)) p->l = n;
        else if (d == right(p)) p->r = n;
        else p->p = n;
        if (n != head) n->p = p;


        bool dc = d->c;
        cnt--;
        delete d;

        if (dc == true) deleteAdjust(n, p);


    }
    void deleteAdjust(link_t n, link_t np)// n subtree decrease one black
    {
        if (cnt == 0) return;// delete only one node : root
        else
        {
            if (n->c == false)// replaced node is red
            {
                n->c = true;
                return;
            }
            else// replaced node is bottom black head
            {// uncle subtree has a black node
                while (1)
                {
                    if (n == root())
                    {
                        //std::cout << "case del root" << std::endl;
                        if (n->c == false)
                            n->c = true;
                        return;
                    }
                    link_t p =  n == head ? np : parent(n);
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
    */
    link_t next(link_t pnode)
    {
        if (right(pnode) != head) return leftmost(right(pnode));
        else
        {
            link_t c = pnode;
            link_t p = parent(c);
            while (c == right(p))
            {
                c = p;
                p = parent(p);
            }
            return p;// return head if pnode is max one
        }
    }
    link_t prev(link_t pnode)
    {
        if (left(pnode) != head) return rightmost(left(pnode));
        else
        {
            link_t c = pnode;
            link_t p = parent(c);
            while (c == left(p))
            {
                c = p;
                p = parent(p);
            }
            return p;// return head if pnode is min one
        }

    }
    link_t minimum()
    {
        if (!empty()) return leftmost(root());
        else return head;
    }
    link_t maximum()
    {
        if (!empty()) return rightmost(root());
        else return head;
    }
};
}
#endif // AVLTREE_HPP
