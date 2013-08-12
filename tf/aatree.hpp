#ifndef AATREE_HPP
#define AATREE_HPP
#include "avlnode.hpp"
#include "bst.hpp"
namespace tf{
template <class Value, class Cmp = std::less<Value> >
class aatree : public bst_base<avlnode<Value>, Cmp>
{
    /*
    bst_base<avlnode<Value>, Cmp>:
        countable:
             size_t cnt;
        link_t head;            // alvnode<Value>*
        Cmp cmp;
    */
protected:
    bool isRed(link_t pnode)
    {
        return pnode != this->root() && height(pnode) == height(parent(pnode));
    }
    bool isBlack(link_t pnode)
    {
        return !isRed(pnode);
    }

public:
    typedef avlnode<Value> node_t;
    typedef avlnode<Value>* link_t;

    aatree()
    {
        this->head->p = this->head->l = this->head->r = this->head;
        this->head->h = 0;
    }

    void insertOneNode(const Value &value, bool unique)
    {
        // find new node's parent
        link_t p = findInsertPosition(value);
        if (p == this->head)
        {// insert as root
            // new node will be leaf node
            link_t newnode = getNewNodeAsLeaf(value);
            this->head->p = newnode;
            newnode->p = this->head;
            this->increaseCnt();
         //  insertAdjust(newnode);// height is 1
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
      //  left child
      link_t p = parent(newnode);
      if (newnode == left(p))
      {

      }

  }
  void skew(link_t pnode)
  {
      rightRotation(pnode);
  }
  void split(link_t pnode)
  {
      leftRotation(pnode);
      ++(parent(pnode)->h);
  }

};

}
#endif // AATREE_HPP
