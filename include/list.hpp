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
	

	typedef std::bidirectional_iterator_tag iterator_category; // �������ͣ� ˫��
	typedef T value_type;									// ֵ���ͣ�  
	typedef T* pointer;					 				// ָ�����ͣ� -> �õ�
	typedef T& reference;									//  �������ͣ� * �õ�
	typedef std::ptrdiff_t difference_type;					// ����������

	typedef size_t size_type;


	//�ڲ���װһ�� �ڵ�ָ��
	link_type p_node;
	// �� �ڵ�ָ�� ����
	list_iterator(link_type p) : p_node(p) {}
	// Ĭ�Ϲ���
	list_iterator() {}
	// ���ƹ���
	list_iterator(const self_type& x) : p_node(x.p_node) {}

	// ���� �������� �еȣ��Ƚ��ڲ���װ��ָ���Ƿ����
	bool operator==(const self_type& x) const
	{
		return p_node == x.p_node;
	}
	bool operator!=(const self_type& x) const
	{
		return p_node != x.p_node;
	}

	// ������ ���ؽڵ����ݲ��ֵ�����
	reference operator*() const
	{
		return (*p_node).data;
	}
	// ��Ա��ȡ ���ؽڵ����ݲ��ֵĵ�ַ
	pointer operator->() const
	{
		return &(operator*());
	}

	// ǰ ++ , ��������
	self_type& operator++()
	{
		p_node = (link_type)((*p_node).next);
		return *this;
	}
	// �� ++�� ����ֵ
	self_type operator++(int)
	{
		self tmp = *this;
		++*this;
		return tmp;
	}
	// ǰ -- , ��������
	self_type& operator--()
	{
		p_node = (*p_node).prev;
		return *this;
	}
	// �� --�� ����ֵ
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
	typedef list_iterator<T> iterator;//���Զ���Ϊ �ڲ���
	typedef T& reference;
	typedef T* pointer;
	typedef T value_type;

	// �������������

	typedef reverse_iterator<iterator> reverse_iterator;

	//end() ���� �� p_node ����� ������
	iterator end() const
	{
		return p_node;
	}
	//begin() ���� end����һ���ڵ������
	iterator begin() const
	{
		return (*p_node).next;
	}
	// rbegin() ���� end()
	reverse_iterator rbegin() const
	{
		return reverse_iterator(end());
	}
	// rend() ���� begin()
	reverse_iterator rend() const
	{
		return reverse_iterator(begin());
	}
	
	// �Ƿ��
	bool empty() const
	{
		node->next == node ;
	}
	// ����������С
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
	//	std::cout << "���� insert" << std::endl;
		link_type temp = new list_node<T>;

	//	std::cout << "���� �ڵ�data " << std::endl;

		temp->data = x;

	//	std::cout << "���� next " << std::endl;

		temp->next = position.p_node;

	//	std::cout << "���� prev " << std::endl;

		temp->prev = position.p_node->prev;

	//	std::cout << "���� position prev " << std::endl;


		position.p_node->prev->next = temp;

	//	std::cout << "���� ǰ��ڵ� next " << std::endl;


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
	
	//���캯��,һ���սڵ�
	void list_init()
	{
		p_node  = new list_node<T>();

	//	std::cout << "���� next " << std::endl;

		p_node->next = p_node;

	//	std::cout << "���� prev " << std::endl;

		p_node->prev = p_node;
	}
	list()
	{
		p_node  = new list_node<T>();

	//	std::cout << "���� next " << std::endl;

		p_node->next = p_node;

	//	std::cout << "���� prev " << std::endl;

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
	// ���� ����
	list(const list& x)
	{
		p_node  = new list_node<T>();

	//	std::cout << "���� next " << std::endl;

		p_node->next = p_node;

	//	std::cout << "���� prev " << std::endl;

		p_node->prev = p_node;

		assign(x.begin(), x.end());
	}
	// ɾ�� position ָ��Ľڵ�
	iterator erase(iterator position)
	{
		link_type p_node_prev = position.p_node->prev;
		link_type p_node_next = position.p_node->next;

		p_node_prev->next = p_node_next;
		p_node_next->prev = p_node_prev;

		delete position.p_node;

		return p_node_next;
	}
	// ɾ�� һ��
	iterator erase(iterator first, iterator last)
	{
		for ( ; first != last; ++first)
		{
			first = erase(first);
		}
		return first;
	}
	// ���� �º��� ɾ��һЩ����ν�ʵĽڵ�
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
	
	// clear ��� �� �ڱ��ڵ������нڵ�

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
	// ����
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
	// �� ���� ֵΪvalue�Ľڵ�ɾ��
	void remove(const T& value)
	{
		iterator first = begin();
		iterator last  = end();

		while (first != last)			//next	�ܼ�¼ first ��һ��λ�ã� ��ɾ������� first Ϊ next
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
	// ɾ����ֵ ��ͬ������Ԫ��
	void unique()
	{
		iterator first = begin();
		iterator last  = end();
		//	ֻ��һ���ڱ��ڵ�
		if (first == last)
		{
			return;
		}
		//	������һ�����ݽڵ�
		iterator next = first;			// fisrt ��¼���Զο�ʼ

		while (++next != last)			// next �� first��һ����ʼ
		{

			if (*next == *first)		// ��ͬ ɾ�� next;	����next ���´�loop
			{
				erase(next);
			}
			else						// ��ͬ ���� first , ���� next (��ʼ��)
			{
				first = next;
				
			}
			next  = first;
		}
	}
	// ���нӺϺ���������ӿ�
	// ����һ��������ȫ���������Ǳ�����
	void splice(iterator position, list& x)
	{
		if (!x.empty())
		{
			transfer(position, x.begin(), x.end());
		}

	}
	// ����һ��������һ���ڵ�
	void splice(iterator position, list&, iterator i)	//��2������ָʾ�����нڵ����������� ����
	{
		iterator j = i;
		++j;
		if (position == i || position == j)
		{
			return ;
		}
		return transfer(position, i, j);
	}
	// ����һ��������һ������
	void splice(iterator position, list&, iterator first, iterator last)
	{
		if (first != last)
		{
			transfer(position, first, last);
		}
	}
	// �ϲ�,����һ���������кϲ���������
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

				iterator next = first2;	// ��¼first2
				transfer(first1, first2, ++next);	// ����first2, first2ʧЧ
				first2 = next;					// �ָ� first2
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
	// ���к����� ������ԭ����ʱ�� ���������к�����㣬����ԭ�������γ�3������size() Ϊ 0 
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
	link_type p_node; // list ֻҪ һ���ڵ�ָ�뼴��


};
}
#endif
