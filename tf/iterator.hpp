#ifndef ITERATOR_HPP
#define ITERATOR_HPP
#include <iterator>
#include <cstddef>
#include "node.hpp"
namespace tf {
template <class Node>
struct iterator_base
{

};
template <class TLinkNode>
struct iterable
{
    typedef TLinkNode node_t;
    typedef TLinkNode* link_t;

    typedef std::bidirectional_iterator_tag iterator_category;
    typedef typename TLinkNode::value_type value_type;
    typedef value_type& reference;
    typedef value_type* pointer;
    typedef int difference_type;

    link_t _p;

    iterable(link_t p):_p(p) {}

    iterable& operator++()
    {
        _p = bstSuccessor();
        return *this;
    }
    iterable& operator--()
    {
        _p = _p->l;
    }

    reference operator*()
    {
        std::cout << "call *" << std::endl;
        return _p->v;
    }
    bool operator==(const iterable& other)
    {
        return _p == other._p;
    }
    bool operator!=(const iterable& other)
    {
        return _p != other._p;
    }
};
}
#endif // ITERATOR_HPP
