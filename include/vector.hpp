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
	typedef T			value_type;			// ֵ����
	typedef value_type*	pointer;				// ָ������
	typedef value_type*	iterator;			// ��������
	typedef value_type&	reference;			// ��������
	typedef size_t		size_type;			// ��������
	typedef ptrdiff_t		defference_type;		// ������������
protected:
	typedef std::allocator<T> data_allocator;
	
	iterator		start;					// 
	iterator		finish;
	iterator		end_of_storage;
	
	void insert_aux(iterator position, const T& x);// 
	void deallocate()	// �����ڴ�
	{
		if (start)// ��ʼָ�벻Ϊ��
		{
			data_allocator::deallocate(start, end_of_storage - start);//����start��ʼ���ڴ�
		}
	}
	iterator allocate_and_fill(size_type n, const T& x)
	{
		iterator result = data_allocator::allocate(n);//����n���ռ䣬�����׵�ַ
		std::uninitialized_fill_n(result, n, x);			// ���n��x
		return result;								// �����׵�ַ
	}
	void fill_initialize(size_type n, const T& value)
	{
		start = allocate_and_fill(size_type n, const T& value);
		finish = start + n;
		end_of_storage = finish;
	}
public:
	// ��ʼ����
	iterator begin() const	
	{
		return start;
	}
	// ĩβ����
	iterator end() const
	{
		return finish;
	}
	// ����
	size_type size() const
	{
		return size_type(end() - begin());
	}
	// �ܷ���
	size_type capacity() const
	{
		return size_type(end_of_storage - begin());
	}
	// �Ƿ��
	bool empty() const
	{
		return begin() == end();
	}
	// ����
	reference operator[](size_type n)
	{
		return *(begin() + n);
	}

	// �չ���
	vector() : start(0), finish(0), end_of_storage(0) {}
	// count, value �Ĺ���
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

	//����
	~vector()
	{
		data_allocator::destroy(start);
	}

};

}

#endif
