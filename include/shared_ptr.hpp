#ifndef H_SHARED_PTR
#define H_SHARED_PTR

#include <iostream>

namespace tf{
template <class T>
class shared_ptr
{
public:
	typedef T 		value_type;
	typedef T*		pointer;
	typedef T&		reference;
	
	typedef shared_ptr self;
	
	shared_ptr(pointer p_ = 0) : p(p_), used(new size_t(1))
	{ 
//		std:: cout << "construct" << std::endl;
	}
	~shared_ptr() 
	{
		dec_used();
	}
	shared_ptr(const self& rhs)
	{
		//std::cout << "copy construct" << std::endl;
		p = rhs.p;
		used = rhs.used;
		inc_used();
	}
	self& operator=(const self& rhs)
	{
	//	std::cout << "operator = " << std::endl;
		if (this == &rhs)	return *this;
		dec_used();
		p = rhs.p;
		used = rhs.used;
		inc_used();
		return *this;
	}
	
	reference operator*(){ return *p;}
	pointer operator->(){ return p;}
	
	
	
private:
	pointer p;
	size_t * used;
	
	void inc_used() {++*used;}
	void dec_used() {if (--*used == 0) destroy();}
	void destroy()	
	{	
		delete p; 
		delete used;
//		std::cout << "delete space" << std::endl;
	}
	
};
/*
int main()
{
	shared_ptr<int> s(new int(5));
	shared_ptr<int> s1(s);
	std::cout << *s1 << std::endl;
	*s1 = 2;
	std::cout << *s << std::endl;
	shared_ptr<int> s2(new int(3));
	s2 = s1;
	std::cout << *s2 << std::endl;
	
	
	return 0;
}
*/
}

#endif