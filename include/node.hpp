#ifndef NODE_HPP
#define NODE_HPP
#include <string>
#include "string.hpp"
#include <iostream>
#include <functional>
namespace tf {
/**************************************************************************/
// slink template
    template <class Node>
    struct slink
    {
        Node *r;
        slink():r(0){}
    };
// right
    template <class SLinkNode>
    SLinkNode* right(SLinkNode* p) {return p->r;}
// bst right most
    template <class SLinkNode>
    SLinkNode* bstRightMost(SLinkNode* p, SLinkNode* end_ptr = 0)
    {
        SLinkNode* c = right(p);
        while (c != end_ptr)
        {
            p = c;
            c = right(p);
        }
        return p;
    }

/**************************************************************************/
// dlink template
    template <class Node>
    struct dlink : public slink<Node>
    {
        Node *l;
        dlink():l(0){}
    };
// left
    template <class DLinkNode>
    DLinkNode* left(DLinkNode* p) {return p->l;}
// bstLeftMost
    template <class DLinkNode>
    DLinkNode* bstLeftMost(DLinkNode* p, DLinkNode* end_ptr = 0)
    {
        DLinkNode* c = left(p);
        while (c != end_ptr)
        {
            p = c;
            c = left(p);
        }
        return p;
    }

// allocate a new node as a leaf node
    template <class Value, class DLinkNode>
    DLinkNode* bstGetNewNodeAsLeaf(const Value& v, DLinkNode* head_end)
    {
        DLinkNode* newNode = new  DLinkNode(v);
        newNode->l = newNode->r = head_end;
        return newNode;
    }
// delete tree recusive
    template <class DLinkNode>
    void bstDeleteNodeRecusive(DLinkNode* root, DLinkNode* head_end)
    {
        if (left(root) != head_end) bstDeleteNodeRecusive(left(root), head_end);
        if (right(root) != head_end) bstDeleteNodeRecusive(right(root), head_end);
        delete root;
    }
// displate bst to std::cout
    template <class DLinkNode>
    void bstDisplayNodeRecusive(DLinkNode* root, DLinkNode* head_end, size_t table_cnt = 0)
    {
        if (root == head_end) std::cout << std::string(table_cnt, '\t') << "[None]" << std::endl;
        else
        {
            bstDisplayNodeRecusive(right(root), head_end, table_cnt + 1);
            std::cout << std::string(table_cnt, '\t') << root->valueToString() << std::endl;
            bstDisplayNodeRecusive(left(root), head_end, table_cnt + 1);
        }
    }
// bst find
    template <class DLinkNode, class Value, class Cmp>
    DLinkNode* bstFindInsertPosition(DLinkNode* pstart,
        DLinkNode* head_end,
        const Value& inserted,
        Cmp cmp = std::less<Value>())
    {// start is head or null, return start;else return where insert
        DLinkNode* p = pstart;
        DLinkNode* c = p;
        while (c != head_end)
        {
            p = c;
            if (cmp(inserted, p->v))
                c = left(p);
            else
                c = right(p);
        }
        return p;
    }

/**************************************************************************/
    template <class Node>
    struct tlink : public dlink<Node>
    {
        Node *p;
        tlink() : p(0){}
    };
// parent node
    template <class TLinkNode>
    TLinkNode* parent(TLinkNode* p) {return p->p;}
// uncle node
    template <class TLinkNode>
    TLinkNode* uncle(TLinkNode* pnode)
    {
        TLinkNode* p = parent(pnode);
        TLinkNode* pp = parent(p);
        if (left(pp) == p) return right(pp);
        else return left(pp);
    }
// sibling node
    template <class TLinkNode>
    TLinkNode* sibling(TLinkNode* pnode)
    {
        TLinkNode* p = parent(pnode);
        if (left(p) == pnode) return right(p);
        else return left(p);

    }
// left rotation
    template <class TLinkNode>
    void bstLeftRotation(TLinkNode* n, TLinkNode* head_end)// head:root's parent, all null point to it
    {
        TLinkNode* p = parent(n);
        TLinkNode* r = right(n);
        TLinkNode* rl = left(r);

        n->r = rl;
        if (rl != head_end) rl->p = n;

        r->l = n;
        n->p = r;

        r->p = p;
        if (p == head_end) // p is head
            if (p == 0) return;// no head_end implementation, return;
            else p->p = r;// implementation with head_end ,n is root, then r is root
        else if (n == left(p))  p->l = r;
        else p->r = r;
    }
// right rotation
    template <class TLinkNode>
    void bstRightRotation(TLinkNode* n, TLinkNode* head_end)
    {
        TLinkNode* p = parent(n);
        TLinkNode* l = left(n);
        TLinkNode* lr = right(l);

        n->l = lr;
        if (lr != head_end) lr->p = n;

        l->r = n;
        n->p = l;

        l->p = p;
        if (p == head_end) // p is head
            if (p == 0) return;// no head_end implementation, return;
            else p->p = l;// implementation with head_end ,n is root, then r is root
        else if (n == left(p)) p->l = l;
        else p->r = l;
    }
// successor
    template <class TLinkNode>
    TLinkNode* bstSuccessor(TLinkNode* pnode, TLinkNode* end_head)
    {
        if (right(pnode) != end_head) return bstLeftMost(right(pnode), end_head);
        else
        {
            TLinkNode* c = pnode;
            TLinkNode* p = parent(c);
            while (p != 0 && c == right(p))
            {
                c = p;
                p = parent(p);
            }
            return p;// return end_head if pnode is max one
        }
    }
// predecessor
    template <class TLinkNode>
    TLinkNode* bstPredecessor(TLinkNode* pnode, TLinkNode* head_end)
    {
    if (left(pnode) != head_end) return bstRightMost(left(pnode), head_end);
        else
        {
            TLinkNode* c = pnode;
            TLinkNode* p = parent(c);
            while (p != 0 && c == left(p))
            {
                c = p;
                p = parent(p);
            }
            return p;// return end_head if pnode is min one
        }
    }
// is root
    template <class TLinkNode>
    TLinkNode* bstIsRoot(TLinkNode* p, TLinkNode* head = 0) {return parent(p) == head;}
//  insert
    template <class TLinkNode, class Value, class Cmp>
    TLinkNode* bstInsertOneNode(TLinkNode* pstart, TLinkNode* head_end, const Value& inserted,
        bool unique = false,
        Cmp cmp = std::less<Value>())
    {
        // find new node's parent
        TLinkNode* p = bstFindInsertPosition(pstart, head_end, inserted, cmp);
        if (p == head_end)
        {// insert as root
            // new node will be leaf node
            TLinkNode* newnode = bstGetNewNodeAsLeaf(inserted, head_end);
            if (head_end != 0) head_end->p = newnode;
            newnode->p = head_end;
            //             cnt++;
            //             insertAdjust(newnode);
            return newnode;
        }
        else// not insert as root
        {
            if (unique && !cmp(inserted, value(p)) && !cmp(value(p), inserted)) return 0;
            else// multi set
            {
                TLinkNode* newnode = getNewNodeAsLeaf(inserted, head_end);
                newnode->p = p;
                if (cmp(inserted, value(p)))
                    p->l = newnode;
                else
                    p->r = newnode;
                //                 cnt++;
                //                 insertAdjust(newnode);
                return newnode;
            }
        }
    }
//  delete
    template <class TLinkNode>
    void bstDeleteOneNode(TLinkNode del, TLinkNode head)
    {
        TLinkNode d = del;
        if (del->l != head && del->r != head)// cnt >= 3
        {
            d = bstSuccessor(del, head);
            del->v = d->v;// copy data from next
        }

        TLinkNode n = right(d) == head ? left(d) : right(d);// d has right sub tree, or both subtree are empty; n: head null, head not null, intern nood
        TLinkNode p = parent(d);// p : intern node, head null, head but not null
        if (p == head)// delete root
        {
            if (head == 0)// delte root, p == null
            {
                if (n != 0) n->p = 0;// n is not empty, n become root
            }
            else// delete root, head imp
            {
                head->p = n;// head's parent is root
                if (n != head) n->p = p;
            }
        }
        else// delete internal node, not root
        {
            if (d == left(p)) p->l = n;
            else  p->r = n;
            if (n != head) n->p = p;// n is internal node
        }
        delete d;
        return;
    }
    /**************************************************************************/
    template <class Node>
    struct flink : public tlink<Node>
    {
        Node *d;
        flink() : d(0){}
    };
    // right
    template <class FLinkNode>
    FLinkNode* down(FLinkNode* p) {return p->d;}
    /**************************************************************************/



}


#endif // NODE_HPP
