#ifndef H_MAKE_HEAP
#define H_MAKE_HEAP
namespace tf{
#include <iostream>
#include <iterator>
// i parent index
// 2i + 1 left, 2i + 2 right
// size = last - first
//	index 0   to  size - 1
template <class RAI>
void down_to_keep_heap(RAI first, RAI last, RAI changed)
{
	size_t size = last - first;
	
	size_t changed_index = changed - first;
	size_t left_index = 2*changed_index + 1;
	size_t right_index = 2*changed_index + 2;
	
	size_t max_index = changed_index;
	if (left_index < size &&
		*(first + left_index) > *changed)
	{
		max_index = left_index;
	}
	if (right_index < size &&
		*(first + right_index) > *(first + max_index))
	{
		max_index = right_index;
	}
	if (max_index != changed_index)
	{
//		std::cout << "swap "<< *(first + max_index) << " "<< *changed << std::endl; ;
//		std::iter_swap(first + max_index, changed);
		down_to_keep_heap(first, last, first + max_index);
	}
	
}
template <class RAI>
void make_heap_(RAI first, RAI last)
{
	size_t size = last - first;
	if (size == 0 || size == 1)
	{
		return ;
	}
	for (int index = (size - 2)/2; index >= 0; --index)
	{
		//std::cout << "index = " << index << std::endl;
		down_to_keep_heap(first, last, first + index);
	}
}
/*
int main()
{
	int arr[] = {1, 2, 3, 4, 5};
	make_heap_(arr, arr + 5);
	std::copy(arr, arr + 5, std::ostream_iterator<int>(std::cout , " "));
}
*/
}
#endif