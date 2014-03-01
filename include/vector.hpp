#ifndef FUFANFAN_VECTOR
#define FUFANFAN_VECTOR

#include <iostream>
#include <memory>

namespace tf
{
template <class T, class Alloc = std::allocator<T> >
class vector
{
public:
	typedef T			value_type;			// 值类型
	typedef value_type*	pointer;				// 指针类型
	typedef value_type*	iterator;			// 迭代类型
	typedef value_type&	reference;			// 引用类型
	typedef size_t		size_type;			// 容量类型
	typedef ptrdiff_t		defference_type;		// 迭代距离类型
protected:
	typedef std::allocator<T> data_allocator;
	
	iterator		start;					// 
	iterator		finish;
	iterator		end_of_storage;
	
	void insert_aux(iterator position, const T& x);// 
	void deallocate()	// 回收内存
	{
		if (start)// 起始指针不为空
		{
			data_allocator::deallocate(start, end_of_storage - start);//回收start开始的内存
		}
	}
	iterator allocate_and_fill(size_type n, const T& x)
	{
		iterator result = data_allocator::allocate(n);//分配n个空间，返回首地址
		std::uninitialized_fill_n(result, n, x);			// 填充n个x
		return result;								// 返回首地址
	}
	void fill_initialize(size_type n, const T& value)
	{
		start = allocate_and_fill(size_type n, const T& value);
		finish = start + n;
		end_of_storage = finish;
	}
public:
	// 开始迭代
	iterator begin() const	
	{
		return start;
	}
	// 末尾迭代
	iterator end() const
	{
		return finish;
	}
	// 已用
	size_type size() const
	{
		return size_type(end() - begin());
	}
	// 总分配
	size_type capacity() const
	{
		return size_type(end_of_storage - begin());
	}
	// 是否空
	bool empty() const
	{
		return begin() == end();
	}
	// 索引
	reference operator[](size_type n)
	{
		return *(begin() + n);
	}

	// 空构造
	vector() : start(0), finish(0), end_of_storage(0) {}
	// count, value 的构造
	vector(size_type n, const T& value)
	{
		fill_initialize(n, value);
	}
	vector(int n, const T& value)
	{
		fill_initialize(n, value);
	}
	vector(long n, const T& value)
	{
		fill_initialize(n, value);
	}

	explicit vector(size_type n)
	{
		fill_initialize(n, T());
	}

	//析构
	~vector()
	{
		data_allocator::destroy(start);
	}

};

}

#endif
