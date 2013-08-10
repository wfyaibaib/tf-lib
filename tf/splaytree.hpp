#ifndef SPLAYTREE_H
#define SPLAYTREE_H
#include "node.hpp"
#include <functional>
#include <string>
namespace tf
{
template <class Value>
struct splaynode : public tlink<splaynode<Value> >,
                   public has_value<Value>
{
    splaynode(const Value& value = Value()) : has_value<Value>(value) {}
    std::string valueToString()
    {
        return "(" + to_string(this->v) + ")";
    }
 };

template <class Value, class Cmp = std::less<Value> >
class splaytree : public bst_base<splaynode<Value>, Cmp>
{
public:
    typedef splaynode<Value> node_t;
    typedef splaynode<Value>* link_t;

    splaytree()
    {
        head->p = head->l = head->r = head;
    }

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
    void splay(link_t pnode)
    {
        while (pnode != root())
        {
            if (parent(pnode) != root())//
            {
                link_t p = parent(pnode);
                link_t pp = parent(p);
                if (p == left(pp))
                {
                    if (pnode == right(p))
                        bstLeftRotation(p, head);
                    bstRightRotation(pp, head);
                }
                else
                {
                    if (pnode == left(p))
                        bstRightRotation(p, head);
                    bstLeftRotation(pp, head);
                }
            }
            else
            {
                link_t p = parent(pnode);
                if (pnode == left(p)) bstRightRotation(p, head);
                else bstLeftRotation(p, head);
                return;
            }
        }
    }

    void insertAdjust(link_t newnode)
    {
        splay(newnode);
    }
    void deleteOneNode(link_t del)
    {
        splay(del);
        bstDeleteOneNode(del, head);
    }

};
}

#endif // SPLAYTREE_H
