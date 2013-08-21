#ifndef RANGE_HPP
#define RANGE_HPP

#include <iterator>
#include <ctime>
#include <cstdlib>
namespace tf{
template <class Value>
struct xrange
{
    typedef std::input_iterator_tag iterator_category;
    typedef Value value_type;
    typedef int difference_type;
    typedef void pointer;
    typedef xrange& reference;
    typedef const xrange& con_ref;

    value_type _start;
    value_type _skip;
    value_type _end;

    xrange(){}
    xrange(value_type start, value_type end, value_type skip) : _start(start), _end(end), _skip(skip) {}
    reference operator++()
    {
        value_type rt = _start;
        _start += _skip;
        return *this;
    }
    value_type operator*()
    {
        return _start;
    }
    bool operator==(const reference)
    {
        return false;
    }

    bool operator!=(const reference)
    {
        return _skip > 0 ? _start < _end : _start > _end;
    }
};
struct xrandom
{
    int _first;
    int _last;
    xrandom(int first = 0, int last = 10) : _first(first), _last(last)
    {
        srand(time(0));
    }
    int operator()()
    {
        return rand()%(_last - _first) + _first;
    }
};
}

#endif // RANGE_HPP
