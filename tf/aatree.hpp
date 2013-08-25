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
    bool isAATree(link_t root)
    {
        if (root == this->head) return true;

        int h = height(root);
        int lh = height(left(root));
        int rh = height(right(root));
        int ph = height(parent(root));

//        if (h != lh + 1) return false;
//        if (h != rh && h != rh + 1) return false;


//        if (h == ph && h == rh) return false;

        if ((h != lh + 1) ||
                (h != rh && h != rh + 1) ||
                (h == ph && h == rh)
                )
        {
            std::cout << "not aa , at: " << root->valueToString() << std::endl;
            return false;
        }

        return isAATree(left(root)) && isAATree(right(root));
    }

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
 //         std::cout << changed << std::endl;
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
  void lowerAndAdjust(link_t adjustNode)
  {
//      std::cout << "after bstDelete:" << std::endl;
//      this->treeShap();

//      std::cout << "head height: " << height(this->head) << std::endl;
      while (adjustNode != this->head)
      {
//          std::cout << "adjust node:" << adjustNode->valueToString() << std::endl;
          int leftDiff = height(adjustNode) - height(left(adjustNode));
          int rightDiff = height(adjustNode) - height(right(adjustNode));

//          std::cout << "<" << leftDiff << ", " << rightDiff << ">" << std::endl;
          if (leftDiff == 2)// left loss height
          {
              if (rightDiff == 1)// right child is black
              {
//                  std::cout << "right child is black" << std::endl;
                  if (height(right(right(adjustNode))) == height(right(adjustNode)))//has right right :red
                  {
//                      std::cout << "has right right" << std::endl;
                      --(adjustNode->h);
                      split(adjustNode);
                      return;
                  }
                  else// has no right right, next loop
                  {
//                      std::cout << "has no right right" << std::endl;
                      --(adjustNode->h);
                      adjustNode = parent(adjustNode);
                  }
              }
              else // right is red
              {
//                  std::cout << "right child is red" << std::endl;

                  --(adjustNode->h);
                  --(right(adjustNode)->h);

                  bool hasRedChild1 = (height(right(left(right(adjustNode)))) == height(left(right(adjustNode))));
                  bool hasRedChild2 = (height(right(right(right(adjustNode)))) == height(right(right(adjustNode))));
//                  std::cout << "(" << hasRedChild1 << ", " << hasRedChild2 << ")" << std::endl;
                  skew(right(adjustNode));// common op
                  if (hasRedChild1)
                  {
                      skew(right(right(adjustNode)));
                      // adjust r rr rrr rrrr (rrrrr)
                      split(adjustNode);
                      // (adjust r (null rr (null rrr (null rrrr (rrrrr)))
                      link_t p = parent(adjustNode);
                      split(right(p));
                  }
                  else if (hasRedChild2)
                  {
                      split(adjustNode);
                      link_t p = parent(adjustNode);
                      split(right(p));
                  }
                  else
                  {
                      split(adjustNode);
                  }

              }
          }
          else if (rightDiff == 2)// right loss height, next loop
          {// leftDiff must be 1,

              --(adjustNode->h);
              // ((head left *) adjust *)
              if (height(left(adjustNode)) == height(right(left(adjustNode))))
              {
                  leftRotation(left(adjustNode));
                  rightRotation(adjustNode);
                  ++(parent(adjustNode)->h);
                  return;

              }
              else
              {
                  rightRotation(adjustNode);
                  adjustNode = parent(parent(adjustNode));
              }
          }
          else//
          {
              return;
          }
      }
  }

  void deleteAdjust(link_t adjustNode, link_t childChanged)
  {
      lowerAndAdjust(adjustNode);
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
