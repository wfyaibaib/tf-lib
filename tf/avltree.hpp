#ifndef AVLTREE_HPP
#define AVLTREE_HPP
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
struct avlnode : public avlnode_base<avlnode<Value> >
{
    Value v;
    avlnode(const Value& value = Value()) : v(value) {}
};
template <class AVL_Node_Base>
bool height(AVL_Node_Base* p) {return p->h;}
/**************************************************************************/
}
#endif // AVLTREE_HPP
