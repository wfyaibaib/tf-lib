#ifndef INSERT_SORT
#define INSERT_SORT

#include <iostream>
#include <iterator>

namespace tf{
template <class RAI>
void insert_sort(RAI first, RAI last)
{
	if (first == last)
	{
		return ;
	}
	for (RAI iter = first + 1; iter != last; ++iter)
	{
		auto tmp = *iter;
		RAI it = iter - 1;
		for (; it != first - 1 && *it > tmp; --it)
		{
			*(it + 1) = *it;
		}
		*(it + 1) = tmp;
	}
}
/*
int main()
{
	int arr[] = {5, 4, 3, 2, 1};
	insert_sort(arr, arr + 5);
	std::copy(arr, arr + 5, std::ostream_iterator<int>(std::cout , " "));
}
*/
}
#endif