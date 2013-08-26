#ifndef BST_HPP
#define BST_HPP

#include "node.hpp"
#include "countable.hpp"
#include "hasvalue.hpp"
#include "iterator.hpp"

#include <functional>

namespace tf{
// bst_base
template <class TLinkNode,
          class Cmp = std::less<typename TLinkNode::value_type> >
struct bst_base : public countable
{
    typedef TLinkNode  node_t;
    typedef typename TLinkNode::value_type Value;
    typedef TLinkNode* link_t;


    /**************************************************************************/

    struct iterator
    {
        typedef TLinkNode node_t;
        typedef TLinkNode* link_t;

        typedef std::bidirectional_iterator_tag iterator_category;
        typedef typename TLinkNode::value_type value_type;
        typedef value_type& reference;
        typedef value_type* pointer;
        typedef int difference_type;

        link_t _p;
        link_t _head;

        iterator(link_t p, link_t head):_p(p), _head(head){}

        iterator& operator++() { _p = bstSuccessor(_p, _head); return *this; }
        iterator& operator--() { _p = _p->l; return *this; }
        reference operator*() { return _p->v; }
        bool operator==(const iterator& other) { return _p == other._p; }
        bool operator!=(const iterator& other) { return _p != other._p; }
    };


    /**************************************************************************/
    /*
     *countable:
         size_t cnt;
    */
    link_t head;
    Cmp cmp;
    /**************************************************************************/

    bst_base(Cmp cmp_ = std::less<Value>()) :
        countable(0),
        head(new TLinkNode()),
        cmp(cmp_)
    {}
    ~bst_base()
    {
        if (!empty())   bstDeleteNodeRecusive(root(), head);
        delete head;
    }
    /**************************************************************************/
    link_t leftMost() { return bstLeftMost(root(), head); }
    link_t rightMost() { return bstRightMost(root(), head); }
    void leftRotation(link_t n) { bstLeftRotation(n, head); }
    void rightRotation(link_t n) { bstRightRotation(n, head); }
    bool isRoot(link_t pnode) { return parent(pnode) == head; }
    bool empty() const{ return size() == 0; }
    link_t root() { return parent(head); }
    link_t getNewNodeAsLeaf(const Value& value) { return bstGetNewNodeAsLeaf(value, head); }
    /**************************************************************************/
    void nodeShapRecusive(link_t subtree ,size_t table_cnt = 0)
    {
        std::string delimiter = std::string(table_cnt, '\t');
        if (subtree == head) std::cout << delimiter << "[None]" << std::endl;
        else
        {
            nodeShapRecusive(right(subtree), table_cnt + 1);
            std::cout << delimiter + subtree->valueToString() << std::endl;
            nodeShapRecusive(left(subtree), table_cnt + 1);
        }
    }
    void treeShap()
    {
        nodeShapRecusive(root());
    }

    std::string nodeToStringRecusive(link_t root)
    {
        if (root == this->head) return "[None]";
        else return
                (
                    "[" +
                    nodeToStringRecusive(root->l) +
                    " (" +
                    root->valueToString() + // has_value
                    ") " +
                    nodeToStringRecusive(root->r) +
                    "]"
                    );
    }
    /**************************************************************************/
    link_t next(link_t pnode)
    {
        return bstSuccessor(pnode, head);
    }
    link_t prev(link_t pnode)
    {
        return bstPredecessor(pnode, head);
    }
    link_t minimum()
    {
        if (!empty()) return leftMost();
        else return head;
    }
    link_t maximum()
    {
        if (!empty()) return rightMost();
        else return head;
    }
    /**************************************************************************/
    link_t findInsertPosition(const Value& value)
    {
        return bstFindInsertPosition(root(), head, value, cmp);
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
            increaseCnt();
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
            increaseCnt();
            return newnode;

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

        delete d;
    }
    /**************************************************************************/
    iterator begin() { return iterator(leftMost(), head); }
    iterator end() { return iterator(head, head); }
};
}
#endif // BST_HPP
