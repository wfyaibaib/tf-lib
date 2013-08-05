#ifndef AVLTREE_HPP
#define AVLTREE_HPP


#include <bst.hpp>

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
struct avlnode : public avlnode_base<avlnode<Value> > ,
                 public has_value<Value>
{
    avlnode(const Value& value = Value()) : has_value<Value>(value) {}
    std::string valueToString()
    {
        return "(" + to_string(this->v) + ", " + to_string(this->h) + ")";
    }
};
template <class AVL_Node_Base>
int height(AVL_Node_Base* p) {return p->h;}
/**************************************************************************/
template <class Value, class Cmp = std::less<Value> >
class avltree : public bst_base<avlnode<Value>, Cmp>
{
    /*
    bst_base<avlnode<Value>, Cmp>:
        countable:
             size_t cnt;
        link_t head;            // alvnode<Value>*
        Cmp cmp;
    */
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


    avltree()
    {
        head = new node_t();
        head->p = head->l = head->r = head;
        head->h = 0;


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

        cnt--;
        delete d;
        //treeShap(root());

        deleteAdjust(p, n);
    }
    void deleteAdjust(link_t adjustNode, link_t childChanged)// childChanged subtree decrease height
    {
  //      std::cout << "deleteAdjust: " << adjustNode->v << '\t' << childChanged->v << std::endl;

            while (adjustNode != head)// childChanged's height decrease
            {
                int lh, rh;
                if (childChanged == left(adjustNode))// delete left
                {
       //             std::cout << "delete left " << std::endl;
                    lh = height(childChanged);
                    rh = height(right(adjustNode));
       //             std::cout << "< " << lh << ", " << rh << ">" << std::endl;
                    if (lh + 1 == rh) return;// old lh = old rh
                    else if (lh == rh)// old lh > old rh
                    {

                        --(adjustNode->h);
                        childChanged = adjustNode;
                        adjustNode = parent(adjustNode);

                    }
                    else//(lh + 1 < rh) old lh < old rh
                    {
                        link_t r = right(adjustNode);
                        link_t rl = left(r);
                        link_t rr = right(r);
                        if (rr == head)// (rl != head)(rr == head)
                        {
 //                           std::cout << "rr == head" << std::endl;
                            bstRightRotation(r, head);
                            bstLeftRotation(adjustNode, head);

                            adjustNode->h = adjustNode->h - 2;
                            --(r->h);
                            ++(rl->h);

                            childChanged = rl;
                            adjustNode = parent(childChanged);
                        }
                        else // (rr != head)(rl unknown)
                        {
      //                      std::cout << "rr != head" << std::endl;
                            bstLeftRotation(adjustNode, head);
                            if (rl != head)
                            {
                                --(adjustNode->h);
                                ++(r->h);

                            }
                            else
                            {
                                adjustNode->h = adjustNode->h - 2;
                            }
                            childChanged = r;
                            adjustNode = parent(childChanged);
                        }
                    }
                }
                else// right
                {
  //                  std::cout << "delete right " << std::endl;

                    rh = height(childChanged);
                    lh = height(left(adjustNode));

  //                  std::cout << "< " << lh << ", " << rh << ">" << std::endl;

                    if (rh + 1 == lh) return;// old rh = old lh
                    else if (rh == lh)// old lh < old rh
                    {
                        --(adjustNode->h);
                        childChanged = adjustNode;
                        adjustNode = parent(adjustNode);

                    }
                    else//(rh + 1 < lh) old lh > old rh
                    {
                        link_t l = left(adjustNode);
                        link_t ll = left(l);
                        link_t lr = right(l);
                        if (ll == head)// (lr != head)(ll == head)
                        {
   //                         std::cout << "ll == head" << std::endl;
                            bstLeftRotation(l, head);
                            bstRightRotation(adjustNode, head);

                            adjustNode->h = adjustNode->h - 2;
                            --(l->h);
                            ++(lr->h);

                            childChanged = lr;
                            adjustNode = parent(childChanged);
                        }
                        else // (ll != head)(lr unknown)
                        {
   //                         std::cout << "ll != head" << std::endl;
                            bstRightRotation(adjustNode, head);
                            if (lr != head)
                            {
                                --(adjustNode->h);
                                ++(l->h);

                            }
                            else
                            {
                                adjustNode->h = adjustNode->h - 2;
                            }
                            childChanged = l;
                            adjustNode = parent(childChanged);
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
#endif // AVLTREE_HPP
