#ifndef AVLTREE_HPP
#define AVLTREE_HPP

#include "bst.hpp"
#include "avlnode.hpp"


namespace tf
{

/**************************************************************************/
template <class Value, class Cmp = std::less<Value> >
class avltree : public bst_base<avlnode<Value>, Cmp>
{
    /*
    bst_base<avlnode<Value>, Cmp>:
        countable:
             size_t cnt;
        link_t this->head;            // alvnode<Value>*
        Cmp cmp;
    */

public:
    typedef avlnode<Value> node_t;
    typedef avlnode<Value>* link_t;

    avltree()
    {
        this->head->p = this->head->l = this->head->r = this->head;
        this->head->h = 0;
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
        newnode->h = 1;//

        link_t changedChild = newnode;
        link_t adjustNode = parent(changedChild);

        while (adjustNode != this->head)
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
                        bstRightRotation(changedChild, this->head);
                        bstLeftRotation(adjustNode, this->head);

                        ++(changedChildLeft->h);
                        --(changedChild->h);
                        adjustNode->h = adjustNode->h - 2;

                        changedChild = changedChildLeft;
                        adjustNode = parent(changedChild);
                    }
                    else// right right
                    {
                        //                            std::cout << "rr" << std::endl;
                        bstLeftRotation(adjustNode, this->head);
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
                        bstLeftRotation(changedChild, this->head);
                        bstRightRotation(adjustNode, this->head);

                        ++(changedChildRight->h);
                        --(changedChild->h);
                        adjustNode->h = adjustNode->h - 2;

                        changedChild = changedChildRight;
                        adjustNode = parent(changedChild);
                    }
                    else// left left
                    {
                        //                            std::cout << "ll" << std::endl;
                        bstRightRotation(adjustNode, this->head);
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

        this->decreaseCnt();
        delete d;
        //treeShap(root());

        deleteAdjust(p, n);
    }
    void deleteAdjust(link_t adjustNode, link_t childChanged)// childChanged subtree decrease height
    {
        //      std::cout << "deleteAdjust: " << adjustNode->v << '\t' << childChanged->v << std::endl;

        while (adjustNode != this->head)// childChanged's height decrease
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
                    if (rr == this->head)// (rl != this->head)(rr == this->head)
                    {
                        //                           std::cout << "rr == this->head" << std::endl;
                        bstRightRotation(r, this->head);
                        bstLeftRotation(adjustNode, this->head);

                        adjustNode->h = adjustNode->h - 2;
                        --(r->h);
                        ++(rl->h);

                        childChanged = rl;
                        adjustNode = parent(childChanged);
                    }
                    else // (rr != this->head)(rl unknown)
                    {
                        //                      std::cout << "rr != this->head" << std::endl;
                        bstLeftRotation(adjustNode, this->head);
                        if (rl != this->head)
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
                    if (ll == this->head)// (lr != this->head)(ll == this->head)
                    {
                        //                         std::cout << "ll == this->head" << std::endl;
                        bstLeftRotation(l, this->head);
                        bstRightRotation(adjustNode, this->head);

                        adjustNode->h = adjustNode->h - 2;
                        --(l->h);
                        ++(lr->h);

                        childChanged = lr;
                        adjustNode = parent(childChanged);
                    }
                    else // (ll != this->head)(lr unknown)
                    {
                        //                         std::cout << "ll != this->head" << std::endl;
                        bstRightRotation(adjustNode, this->head);
                        if (lr != this->head)
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
protected:
    int factor(link_t pnode)
    {
        return height(left(pnode)) - height(right(pnode));
    }
    bool adjustHeight(link_t adjustNode, link_t changedChild)
    {
        if (height(adjustNode) == height(changedChild))
        {
            ++(adjustNode->h);
            return true;
        }
        else return false;
    }
};
}
#endif // AVLTREE_HPP
