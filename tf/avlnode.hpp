#ifndef AVLNODE_HPP
#define AVLNODE_HPP
#include "node.hpp"
#include "hasvalue.hpp"
#include "string.hpp"
namespace tf{
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
    std::string valueToString() // implement
    {
        return "(" + to_string(this->v) + ", " + to_string(this->h) + ")";
    }
};
template <class AVL_Node_Base>
int height(AVL_Node_Base* p) {return p->h;}
/**************************************************************************/
}
#endif // AVLNODE_HPP
