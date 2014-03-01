#ifndef FORWARD_LIST_H
#define FORWARD_LIST_H
#include <cstddef>
#include <iostream>
#include <iterator>
#include <algorithm>

namespace tf{
template <class T>
class forward_list_node
{
	
public:
	typedef T value_type;
	typedef forward_list_node<T> node_type;
	typedef node_type	self;
	typedef node_type* 	link_type;
	
	forward_list_node(value_type d = value_type()) : data(d), next(0) {}
	

	T data;
	link_type next;
		
};

template <class T>
class forward_list_iterator
{
public:
	typedef forward_list_node<T> node_type;
	typedef  node_type* link_type;
	typedef forward_list_iterator<T> self;
	typedef self iterator;
	
	typedef std::forward_iterator_tag iterator_category;
	typedef T value_type;
	typedef T* pointer;
	typedef	T& reference;
	typedef ptrdiff_t difference_type;
	
	forward_list_iterator(link_type p = 0) : p_node(p) {}
	reference operator*()
	{
		return p_node->data;
	}
	pointer operator->()
	{
		return &(operator*());
	}
	iterator& operator++()
	{
		p_node = p_node -> next;
		return *this;
	}
	iterator operator++(int)
	{
		iterator old = *this;
		++*this;
		return old;
	}


        link_type p_node;
	

	bool operator==(const self& rhs)
	{
		return p_node == rhs.p_node;
	}
	bool operator!=(const self& rhs)
	{
		return p_node != rhs.p_node;
	}
	
	
		
};

template <class T>
class forward_list
{
public:
	typedef forward_list_node<T> node_type;
	typedef node_type*	link_type;
	typedef forward_list_iterator<T> iterator;
	typedef T value_type;
	
	typedef forward_list<T> self;
	typedef size_t size_type;
	
	node_type v_node;
	
	forward_list() {v_node.next = &v_node;}
	bool empty() const
	{
		return v_node.next == &v_node;
	}
	size_type size() const
	{
		size_type s = 0;
		link_type vp = &v_node;
		link_type bp = vp->next;
		
		while (bp != vp)
		{
			++s;
			bp = bp->next;
		}
		return s;
	}
	
	iterator begin()
	{
		return v_node.next;
	}
	iterator end()
	{
		return &v_node;
	}
	iterator insert_after(iterator position, value_type value)
	{
		link_type newp= new node_type(value);
		
		link_type prev_p = position.p_node;
		link_type next_p = position.p_node->next;
		
		newp->next = next_p;
		prev_p->next = newp;
		
		return ++position;
	}
	void push_front(T value)
	{
		insert_after(end(), value);
	}
};
/*
int main()
{
	forward_list<int> ilist;
	ilist.push_front(1);
	ilist.push_front(2);
	ilist.push_front(3);
	std::copy(ilist.begin(), ilist.end(), std::ostream_iterator<int>(std::cout , " "));
	return 0;
}
*/
}
#endif