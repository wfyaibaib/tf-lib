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
class splaytree
{
public:
    typedef splaynode<Value> node_t;
    typedef splaynode<Value>* link_t;

    std::string display_recusive(link_t root)
    {
        if (root == head) return "[None]";
        else return
            (
            "[" +
            display_recusive(root->l) +
            " (" +
            to_string(root->v) +
            ") " +
            display_recusive(root->r) +
            "]"
            );
    }

    link_t leftmost(link_t pnode) { return bstLeftMost(pnode, head); }
    link_t rightmost(link_t pnode) { return bstRightMost(pnode, head); }
    void leftRotation(link_t n) { bstLeftRotation(n, head); }
    void rightRotation(link_t n) { bstRightRotation(n, head); }
    bool isRoot(link_t pnode) { return parent(pnode) == head; }
    bool empty() { return cnt == 0; }
    link_t root() { return parent(head); }
    link_t getNewNodeAsLeaf(const Value& value) { return bstGetNewNodeAsLeaf(value, head); }

    std::string display() { return display_recusive(root()); }

    link_t head;
    size_t cnt;
    Cmp cmp;

    splaytree()
    {
        cmp = Cmp();

        head = new node_t();
        head->p = head->l = head->r = head;

        cnt = 0;
    }
    ~splaytree()
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

    void treeShap(link_t subtree ,size_t table_cnt = 0)
    {
        if (subtree == head) std::cout << std::string(table_cnt, '\t') << "[None]" << std::endl;
        else
        {

            treeShap(right(subtree), table_cnt + 1);
            std::cout << std::string(table_cnt, '\t') + "(" + to_string(subtree->v)  + ")" << std::endl;
            treeShap(left(subtree), table_cnt + 1);
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

#endif // SPLAYTREE_H
