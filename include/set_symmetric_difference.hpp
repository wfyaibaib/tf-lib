#ifndef SET_SYMMETRIC_DIFFERENCE
#define SET_SYMMETRIC_DIFFERENCe
#include <algorithm>
namespace tf{

// 对称差
template <class InputIter1, class InputIter2, class OutputIter>
OutputIter set_symmetric_difference(InputIter1 first1, InputIter1 last1,
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
		else if(*first2 < *first1)
		{
			*result = *first2;
			++first2;
			++result;
		}// 不等取较小，相等均移动
		else
		{
			++first1;
			++first2;
		}
	}
	return std::copy(first1, last1, fufanfan::copy(first2, last2, result));
}


}
#endif