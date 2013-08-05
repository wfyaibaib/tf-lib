#ifndef SPLAYTREE_H
#define SPLAYTREE_H
#include "bst.hpp"
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
        this->head->p = this->head->l = this->head->r = this->head;
    }
    void splay(link_t pnode)
    {
        while (pnode != this->root())
        {
            if (parent(pnode) != this->root())//
            {
                link_t p = parent(pnode);
                link_t pp = parent(p);
                if (p == left(pp))
                {
                    if (pnode == right(p))
                        bstLeftRotation(p, this->head);
                    bstRightRotation(pp, this->head);
                }
                else
                {
                    if (pnode == left(p))
                        bstRightRotation(p, this->head);
                    bstLeftRotation(pp, this->head);
                }
            }
            else
            {
                link_t p = parent(pnode);
                if (pnode == left(p)) bstRightRotation(p, this->head);
                else bstLeftRotation(p, this->head);
                return;
            }
        }
    }

    link_t insertOneNode(const Value& value, bool unique = false)
    {
//        std::cout << "insertOneNode: " << value << std::endl;
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
            if (unique && !cmp(value, p->v) && !cmp(p->v, value)) return 0;
            else// multi set
            {
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
    }

    void insertAdjust(link_t newnode)
    {
        splay(newnode);
    }
    void deleteOneNode(link_t del)
    {
        splay(del);
        bstDeleteOneNode(del, this->head);
    }
};
}

#endif // SPLAYTREE_H
