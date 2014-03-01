#ifndef BUBBLE_SORT
#define BUBBLE_SORT

#include <iostream>
#include <iterator>
#include <algorithm>
// http://codepad.org/wmXD6MRN
namespace tf{
// bubble_sort
//int arr[] = {9, 8, 7, 6, 5, 4, 3, 2, 1};
template <class RandomAccessIter>
void bubble_sort(RandomAccessIter first, RandomAccessIter last)
{
	if (first == last)
	{
		return ;
	}
	bool finished = false;
	size_t len = last - first;
	
	while (--len && !finished)
	{
		finished = true;
		for (RandomAccessIter p1 = first;
			 p1 != first + len; 
			 ++p1)
		{
			
			if (*p1 > *(p1 + 1))
			{
				std::iter_swap(p1, p1 + 1);
				finished = false;
			}
		} 
	}
}
/*
int main()
{
	bubble_sort(arr, arr + 9);
	std::copy(arr, arr + 9, std::ostream_iterator<int>(std::cout , " "));
}
*/
}
#endif
