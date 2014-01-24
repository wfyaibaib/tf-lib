#ifndef DISJOINT_SET_H
#define DISJOINT_SET_H

#include "node.hpp"
namespace tf {
template <class Value>
class disjoint_set_node : public slink<disjoint_set_node<Value> >
{
    typedef Value* value_ptr_t;
    typedef disjoint_set_node<Value>* link_t;

    value_ptr_t pobj;
    int rank;
};
template <class Value>
class disjoint_set
{
public:
    typedef Value* obj_link_t;
    
    disjoint_set()
	{}
    ~disjoint_set()
	{}


};


}

#endif