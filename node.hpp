#ifndef NODE_HPP
#define NODE_HPP
#include <iostream>
#include <string>
#include <functional>
namespace tf {
/**************************************************************************/
    template <class Node>
    struct slink
    {
        Node *r;
        slink():r(0){}
    };

    template <class SLinkNode>
    SLinkNode* right(SLinkNode* p) {return p->r;}

    template <class SLinkNode>
    SLinkNode* rightMost(SLinkNode* p, SLinkNode* end_ptr = 0)
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
    template <class Node>
    struct dlink : public slink<Node>
    {
        Node *l;
        dlink():l(0){}
    };

    template <class DLinkNode>
    DLinkNode* left(DLinkNode* p) {return p->l;}

    template <class DLinkNode>
    DLinkNode* leftMost(DLinkNode* p, DLinkNode* end_ptr = 0)
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
            if (cmp(inserted, value(p)))
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

    template <class TLinkNode>
    TLinkNode* parent(TLinkNode* p) {return p->p;}

    template <class TLinkNode>
    TLinkNode* uncle(TLinkNode* pnode)
    {
        TLinkNode* p = parent(pnode);
        TLinkNode* pp = parent(p);
        if (left(pp) == p) return right(pp);
        else return left(pp);
    }

    template <class TLinkNode>
    TLinkNode* sibling(TLinkNode* pnode)
    {
        TLinkNode* p = parent(pnode);
        if (left(p) == pnode) return right(p);
        else return left(p);

    }

    template <class TLinkNode>
    void bstLeftRotation(TLinkNode* n, TLinkNode* head_end = 0)// head:root's parent, all null point to it
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

    template <class TLinkNode>
    void bstRightRotation(TLinkNode* n, TLinkNode* head_end = 0)
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

    template <class TLinkNode>
    TLinkNode* bstSuccessor(TLinkNode* pnode, TLinkNode* end_head)
    {
        if (right(pnode) != end_head) return leftMost(right(pnode), end_head);
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

    template <class TLinkNode>
    TLinkNode* bstPredecessor(TLinkNode* pnode, TLinkNode* head_end)
    {
        if (left(pnode) != end_head) return rightMost(left(pnode), end_head);
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

    template <class TLinkNode>
    TLinkNode* bstIsRoot(TLinkNode* p, TLinkNode* head = 0) {return parent(p) == head;}
//

    // normal binary tree insert
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
                TLinkNode* newnode = getNewNodeAsLeaf(value, head_end);
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
    template <class TLinkNode>
    void bstDeleteOneNode(TLinkNode del, TLinkNode head)
    {
        TLinkNode d = del;
        if (del->l != head && del->r != head)// cnt >= 3
        {
            d = bstSuccessor(del);
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
template <class Node>
struct avlnode_base : public tlink<Node>
{
    size_t h;// height
    avlnode_base():h(1){}
};
template <class Value>
struct avlnode : public avlnode_base<avlnode<Value> >
{
    Value v;
    avlnode(const Value& value = Value()) : v(value) {}
};
template <class AVL_Node_Base>
bool height(AVL_Node_Base* p) {return p->h;}
/**************************************************************************/
template <class Value, class Cmp = std::less<Value> >
class rbtree
{
public:
    typedef rbnode<Value> rbnode;
    typedef rbnode* link_t;

    std::string display_recusive(link_t root)
    {
        if (root == head) return "[None]";
        else return
            (
            "[" +
            display_recusive(root->l) +
            " (" +
            std::to_string(root->v) + ", " + (root->c ? "b" : "r") +
            ") " +
            display_recusive(root->r) +
            "]"
            );
    }

    rbnode* leftmost(rbnode* pnode) { return leftMost(pnode, head); }
    rbnode* rightmost(rbnode* pnode) { return rightMost(pnode, head); }
    void leftRotation(link_t n) { bstLeftRotation(n, head); }
    void rightRotation(link_t n) { bstRightRotation(n, head); }
    bool isRoot(rbnode *pnode) { return parent(pnode) == head; }
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

        head = new rbnode();
        head->p = head->l = head->r = head;
        head->c = true;

        cnt = 0;
    }


    link_t findInsertPosition(const Value& value) const
    {
        return bstFindInsertPosition(head->p, head, value, cmp);
 /*       link_t p = head;
        link_t c = head->p;
        while (c != head)
        {
            p = c;
            if (cmp(value, p->v))
                c = p->l;
            else
                c = p->r;
        }
        return p;*/
    }

    // normal binary tree insert
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
            std::cout << std::string(table_cnt, '\t') + "(" + std::to_string(subtree->v) + ", " + (subtree->c ? "b":"r") + ")" << std::endl;
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
int main()
{
    tf::rbtree<int> tree;
    const int N = 7;
    for (int i = 0; i < N; i++)
    {
        tree.insertOneNode(i);
        std::cout << "-----------------------------------------" << std::endl;
        std::cout << i << std::endl;
        tree.treeShap(tree.root());
    }

    //tree.leftRotation(tree.root());
    //tree.leftRotation(tree.root());
    //tree.rightRotation(tree.root());
    //tree.rightRotation(tree.root());


    return 0;
}


#endif // NODE_HPP
