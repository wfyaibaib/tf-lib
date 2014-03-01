#ifndef FUFANFAN_LIST
#define FUFANFAN_LIST
#include <iterator>
#include <cstddef>
#include <iterator>
namespace tf
{
// list_node
template <class T> 
class list_node
{
public:
	typedef void* void_pointer;
	typedef list_node<T>* node_pointer;

	node_pointer prev;
	node_pointer next;

	T data;
};
//list_iterator
template <class T>
class list_iterator : public std::iterator<std::bidirectional_iterator_tag, T, std::ptrdiff_t, T*, T&>
{
public:
//	typedef list_iterator<T, Ptr, Ref> self;
	typedef list_iterator<T> self_type;
	typedef typename list_node<T>::node_pointer link_type;
	

	typedef std::bidirectional_iterator_tag iterator_category; // 迭代类型， 双向
	typedef T value_type;									// 值类型，  
	typedef T* pointer;					 				// 指针类型， -> 得到
	typedef T& reference;									//  引用类型， * 得到
	typedef std::ptrdiff_t difference_type;					// 迭代差类型

	typedef size_t size_type;


	//内部封装一个 节点指针
	link_type p_node;
	// 用 节点指针 构造
	list_iterator(link_type p) : p_node(p) {}
	// 默认构造
	list_iterator() {}
	// 复制构造
	list_iterator(const self_type& x) : p_node(x.p_node) {}

	// 两个 迭代器的 判等，比较内部封装的指针是否相等
	bool operator==(const self_type& x) const
	{
		return p_node == x.p_node;
	}
	bool operator!=(const self_type& x) const
	{
		return p_node != x.p_node;
	}

	// 解引用 返回节点数据部分的引用
	reference operator*() const
	{
		return (*p_node).data;
	}
	// 成员存取 返回节点数据部分的地址
	pointer operator->() const
	{
		return &(operator*());
	}

	// 前 ++ , 返回引用
	self_type& operator++()
	{
		p_node = (link_type)((*p_node).next);
		return *this;
	}
	// 后 ++， 返回值
	self_type operator++(int)
	{
		self tmp = *this;
		++*this;
		return tmp;
	}
	// 前 -- , 返回引用
	self_type& operator--()
	{
		p_node = (*p_node).prev;
		return *this;
	}
	// 后 --， 返回值
	self_type operator--(int)
	{
		self tmp = *this;
		--*this;
		return tmp;
	}

	

};
	

//list

template <class T, class Alloc = std::allocator<T> >
class list
{
public:
	typedef list_node<T>* link_type;
	typedef size_t size_type;
	typedef list_iterator<T> iterator;//可以定义为 内部类
	typedef T& reference;
	typedef T* pointer;
	typedef T value_type;

	// 定义逆向迭代器

	typedef reverse_iterator<iterator> reverse_iterator;

	//end() 返回 用 p_node 构造的 迭代器
	iterator end() const
	{
		return p_node;
	}
	//begin() 返回 end的下一个节点迭代器
	iterator begin() const
	{
		return (*p_node).next;
	}
	// rbegin() 返回 end()
	reverse_iterator rbegin() const
	{
		return reverse_iterator(end());
	}
	// rend() 返回 begin()
	reverse_iterator rend() const
	{
		return reverse_iterator(begin());
	}
	
	// 是否空
	bool empty() const
	{
		node->next == node ;
	}
	// 返回容器大小
	size_type size() const
	{
		size_t s = 0;
		for (iterator it = begin(); it != end(); ++it)
		{
			++s;
		}
		return s;
		//return std::distance(begin(), end());
	}
	// 
	reference front()
	{
		return *begin();
	}
	reference back()
	{
		return *(--end());
	}
	//
	iterator insert(iterator position, const T& x)
	{
	//	std::cout << "进入 insert" << std::endl;
		link_type temp = new list_node<T>;

	//	std::cout << "设置 节点data " << std::endl;

		temp->data = x;

	//	std::cout << "设置 next " << std::endl;

		temp->next = position.p_node;

	//	std::cout << "设置 prev " << std::endl;

		temp->prev = position.p_node->prev;

	//	std::cout << "设置 position prev " << std::endl;


		position.p_node->prev->next = temp;

	//	std::cout << "设置 前面节点 next " << std::endl;


		position.p_node->prev		 = temp;

		return temp;
		
	}
	void push_back(const T& x)
	{
		insert(end(), x);
	}
	void push_front(const T& x)
	{
		insert(begin(), x);
	}
	
	//构造函数,一个空节点
	void list_init()
	{
		p_node  = new list_node<T>();

	//	std::cout << "设置 next " << std::endl;

		p_node->next = p_node;

	//	std::cout << "设置 prev " << std::endl;

		p_node->prev = p_node;
	}
	list()
	{
		p_node  = new list_node<T>();

	//	std::cout << "设置 next " << std::endl;

		p_node->next = p_node;

	//	std::cout << "设置 prev " << std::endl;

		p_node->prev = p_node;

		
	}
	list(size_type n, value_type value)
	{
		list_init();
		while (n--)
		{
			push_back(value);
		}
	}
	void resize(size_type n)
	{
		clear();
		while (n--)
		{
			push_back(value_type());
		}
	}
	// 复制 构造
	list(const list& x)
	{
		p_node  = new list_node<T>();

	//	std::cout << "设置 next " << std::endl;

		p_node->next = p_node;

	//	std::cout << "设置 prev " << std::endl;

		p_node->prev = p_node;

		assign(x.begin(), x.end());
	}
	// 删除 position 指向的节点
	iterator erase(iterator position)
	{
		link_type p_node_prev = position.p_node->prev;
		link_type p_node_next = position.p_node->next;

		p_node_prev->next = p_node_next;
		p_node_next->prev = p_node_prev;

		delete position.p_node;

		return p_node_next;
	}
	// 删除 一段
	iterator erase(iterator first, iterator last)
	{
		for ( ; first != last; ++first)
		{
			first = erase(first);
		}
		return first;
	}
	// 根据 仿函数 删除一些满足谓词的节点
	template <class Predicate>
	iterator erase_if(iterator first, iterator last, Predicate pred)
	{
		for (; first != last; ++first)
		{
			if (pred(*first))
			{
				first = erase(first);
			}
		}

	}
	
	// clear 清除 除 哨兵节点外所有节点

	void clear()
	{
		link_type cur = p_node->next;

		while (cur != p_node)
		{
			link_type tmp = cur;
			cur = cur->next;
			delete tmp;
		}

		p_node->next = p_node;
		p_node->prev = p_node;
	}
	// 分配
	template <class InputIter>
	void assign(InputIter first, InputIter last)
	{
		clear();
		for ( ; first != last; ++first)
		{
			push_back(*first);
		}
	}
	// 
	// 将 所有 值为value的节点删除
	void remove(const T& value)
	{
		iterator first = begin();
		iterator last  = end();

		while (first != last)			//next	总记录 first 下一个位置， 若删除则更正 first 为 next
		{
			iterator next = first;
			++next;
			if (*first == value)
			{
				erase(first);
				first = next;
			}
		}
	}
	template <class Predicate>
	void remove_if(iterator first, iterator last, Predicate pred)
	{
		erase_if(begin(), end(), pred);
	}
	// 删除数值 相同的连续元素
	void unique()
	{
		iterator first = begin();
		iterator last  = end();
		//	只有一个哨兵节点
		if (first == last)
		{
			return;
		}
		//	至少有一个数据节点
		iterator next = first;			// fisrt 记录测试段开始

		while (++next != last)			// next 从 first后一个开始
		{

			if (*next == *first)		// 相同 删除 next;	更正next ，下次loop
			{
				erase(next);
			}
			else						// 不同 更正 first , 更正 next (初始化)
			{
				first = next;
				
			}
			next  = first;
		}
	}
	// 剪切接合函数，对外接口
	// 剪切一个容器的全部，不能是本容器
	void splice(iterator position, list& x)
	{
		if (!x.empty())
		{
			transfer(position, x.begin(), x.end());
		}

	}
	// 剪切一个容器的一个节点
	void splice(iterator position, list&, iterator i)	//第2个参数指示被剪切节点所在容器， 无用
	{
		iterator j = i;
		++j;
		if (position == i || position == j)
		{
			return ;
		}
		return transfer(position, i, j);
	}
	// 剪切一个容器的一段序列
	void splice(iterator position, list&, iterator first, iterator last)
	{
		if (first != last)
		{
			transfer(position, first, last);
		}
	}
	// 合并,将另一个已序序列合并到本容器
	void merge(list& x)
	{
		iterator first1 = begin();
		iterator last1	= end();
		iterator first2 = x.begin();
		iterator last2	= x.end();

		while (first1 != last1 && first2 != last2)
		{
			if (*first2 < *first1)
			{
			//	std::cout << "*first2 = " << *first2 << std::endl;
			//	std::cout << "*first1 = " << *first1 << std::endl;

				iterator next = first2;	// 记录first2
				transfer(first1, first2, ++next);	// 剪切first2, first2失效
				first2 = next;					// 恢复 first2
			}
			else
			{
				++first1;
			}
		}
		if (first2 != last2)
		{
			transfer(last1, first2, last2);
		}
		
	}
	//

protected:
	// 剪切函数， 作用于原序列时， 若剪切序列含插入点，则在原序列上形成3个环，size() 为 0 
	void transfer(iterator position, iterator first, iterator last)
	{
		if (position != last)
		{
			link_type position_prev_p_node = position.p_node->prev;
			link_type first_prev_p_node	= first.p_node->prev;
			link_type last_prev_p_node		= last.p_node->prev;

			position_prev_p_node->next = first.p_node;
			last_prev_p_node->next		=	position.p_node;

			position.p_node->prev	= last_prev_p_node;
			first.p_node->prev	= position_prev_p_node;

			first_prev_p_node->next = last.p_node;
			last.p_node->prev	=	first_prev_p_node;
		}
	}
public:
	~list()
	{
		clear();
		delete p_node;
//		std::cout << "destructor!" << std::endl;
	}


	
protected:
	link_type p_node; // list 只要 一个节点指针即可


};
}
#endif
