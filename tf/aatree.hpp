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

    link_t insertOneNode(const Value &value, bool unique = false)
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
                insertAdjust(parent(newnode));
                return newnode;
            }
        }
    }
  void insertAdjust(link_t adjustnode)
  {
      int unChangedCnt = 0;
      while (adjustnode != this->head)
      {
          link_t nextadjust = parent(adjustnode);
          bool changed = false;
          //  has left child and height is equal
          if (left(adjustnode) != this->head &&
                  height(left(adjustnode)) == height(adjustnode))
          {
//              std::cout << "skew: " << adjustnode->valueToString() << std::endl;
              skew(adjustnode);
              changed = true;
              nextadjust = parent(adjustnode);// old left(adjust)

          }
          // has right right and height is equal
          if (right(right(adjustnode)) != this->head &&
                  height(right(right(adjustnode))) == height(adjustnode))
          {
//              std::cout << "split: " << adjustnode->valueToString() << std::endl;

              split(adjustnode);
              changed  = true;
              // nextadjust = parent(adjustnode)

          }
          std::cout << changed << std::endl;
          if (changed) unChangedCnt = 0;
          else if(++unChangedCnt == 2) break;
          else;

          adjustnode = nextadjust;
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
  void deleteAdjust(link_t adjustNode, link_t childChanged)
  {
      // ((head del del_right) adjust (*))
      if (childChanged != this->head)// replaced nod is right red node
          return;
      else // deleted node is black and has no right child
      {//((head del head) adjust (( head redleft *) red (head redright *))
       // or ((head del head) adjust (head black *))
          if (height(adjustNode) != height(right(adjustNode)))
          {
              if (right(right(adjustNode)) != this->head)
              {//((head del head) adjust (head black red))
                  --(adjustNode->h);
                  split(adjustNode);
                  return;
              }
              else//((head del head) adjust (head black head))
              {
                  --(adjustNode->h);
              }

          }

          --(adjustNode->h);
          insertAdjust(adjustNode);
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
