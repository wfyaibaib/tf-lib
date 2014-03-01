#ifndef H_QSORT
#define H_QSORT
// http://codepad.org/ejsQSqVL
#include <iostream>
#include <iterator>
#include <algorithm>

namespace tf {
// qsort
//int arr[] = {9, 8, 7, 6, 5, 4, 3, 2, 1};
template <class RandomAccessIter>
void qsort(RandomAccessIter first, RandomAccessIter last)
{
	if (first == last || first == last - 1)
	{
		return ;
	}
	RandomAccessIter p1 = first;
	RandomAccessIter p2 = first;
	
	for (; p2 != last - 1; )
	{
		//std::cout << "*p1 = " << *p1 << "	*p2 =" << *p2 << std::endl; 
		if (*p2 < *(last - 1))
		{
			std::iter_swap(p1, p2);
			++p1;
			++p2;
		}
		else
		{
			++p2;
		}
	} 
	//std::cout << "after loop, iter_swap()"<< std::endl;
	std::iter_swap(p1, p2);
	qsort(first, p1);
	qsort(p1 + 1, last);
}
//int main()
//{
//	qsort(arr, arr + 9);
//	std::copy(arr, arr + 9, std::ostream_iterator<int>(std::cout , " "));
//}
	
}
#endif