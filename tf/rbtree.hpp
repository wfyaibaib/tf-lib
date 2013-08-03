#ifndef RBTREE_HPP
#define RBTREE_HPP
#include "node.hpp"
#include "string.hpp"
#include <string>
#include <iostream>
namespace tf
{
/**************************************************************************/

template <class Node>
struct rbnode_base : public tlink<Node>
{
    bool c;// 0:red,1:black
    rbnode_base():c(false){}
};
template <class RB_Node_Base>
bool color(RB_Node_Base *p) {return p->c;}

template <class Value>
struct rbnode : public rbnode_base<rbnode<Value> >
{
    typedef Value value_type;
    Value v;
    rbnode(const Value& value = Value()) : v(value) {}
};
template <class RB_Node>
typename RB_Node::value_type value(RB_Node *p) {return p->v;}


/**************************************************************************/

template <class Value, class Cmp = std::less<Value> >
class rbtree
{
public:
//  typedef rbnode<Value> rbnode; g++4.6 not support
    typedef rbnode<Value>   node_t;
    typedef rbnode<Value>* link_t;
// g++4.6 not support std::to_string<>
    std::string display_recusive(link_t root)
    {
        if (root == head) return "[None]";
        else return
            (
            "[" +
            display_recusive(root->l) +
            " (" +
            to_string(root->v) + ", " + (root->c ? "b" : "r") +
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

    rbtree()
    {
        cmp = Cmp();

        head = new node_t();
        head->p = head->l = head->r = head;
        head->c = true;

        cnt = 0;
    }
    ~rbtree()
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
    void insertAdjust(link_t newnode)
    {
        if (newnode->p == head)
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
    void treeShap(link_t subtree ,size_t table_cnt = 0)
    {
        if (subtree == head) std::cout << std::string(table_cnt, '\t') << "[None]" << std::endl;
        else
        {

            treeShap(right(subtree), table_cnt + 1);
            std::cout << std::string(table_cnt, '\t') + "(" + to_string(subtree->v) + ", " + (subtree->c ? "b":"r") + ")" << std::endl;
            treeShap(left(subtree), table_cnt + 1);
        }
    }

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
#endif // RBTREE_HPP
