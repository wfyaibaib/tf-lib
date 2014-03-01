#ifndef SET_DIFFERENCE_H
#define SET_DIFFERENCE_H
#include <algorithm>
namespace tf{
// 差集
template <class InputIter1, class InputIter2, class OutputIter>
OutputIter set_difference(InputIter1 first1, InputIter1 last1,
						InputIter2 first2, InputIter2 last2,
						OutputIter result)
{
	for ( ; first1 != last1 && first2 != last2; )
	{
		if (*first1 < *first2)
		{
			*result = *first1;
			++first1;
			++result;
		}
		// 序列2小，移动序列2，相等均移动
		else if (*first2 < *first1)
		{
			++first2;
		}
		else
		{
			++first1;
			++first2;
		}
	}
	return std::copy(first1, last1, result);
}

template <class InputIter1, class InputIter2, class OutputIter, class Compare>
OutputIter set_difference(InputIter1 first1, InputIter1 last1,
						InputIter2 first2, InputIter2 last2,
						OutputIter result,
						Compare cmp)
{
	for ( ; first1 != last1 && first2 != last2; )
	{
		if (cmp(*first1, *first2))
		{
			*result = *first1;
			++first1;
			++result;
		}
		// 序列2小，移动序列2，相等均移动
		else if (cmp(*first2, *first1))
		{
			++first2;
		}
		else
		{
			++first1;
			++first2;
		}
	}
	return std::copy(first1, last1, result);
}
}
#endif