#ifndef SET_UNION_H
#define SET_UNION_H
#include <algorithm>
namespace tf{
//���������������㷨
// ����
template <class InputIter1, class InputIter2, class OutputIter>
OutputIter set_union(InputIter1 first1, InputIter1 last1,
					InputIter2 first2, InputIter2 last2,
						OutputIter result)
{
	for ( ; first1 != last1 && first2 != last2; ++result)
	{
		if (*first1 < *first2)
		{
		
			*result = *first1;
			++first1;
		}
		else if(*first2 < *first1)
		{
			*result = *first2;
			++first2;
		}
		else// ���ͬʱ�ƶ�����֤����Ԫ����ͬ�߸���ȡ�ϴ�
		{
			*result = *first1;
			++first1;
			++first2;
		}
	}
	return std::copy(first1, last1,std::copy(first2, last2, result));//Ϭ���Ĵ��� ������� std��ͻ
	
}
template <class InputIter1, class InputIter2, class OutputIter, class Compare>
OutputIter set_union(InputIter1 first1, InputIter1 last1,
					InputIter2 first2, InputIter2 last2,
						OutputIter result, Compare cmp)
{
	for ( ; first1 != last1 && first2 != last2; ++result)
	{
		if (cmp(*first1, *first2))
		{
		
			*result = *first1;
			++first1;
		}
		else if(cmp(*first2, *first1))
		{
			*result = *first2;
			++first2;
		}
		else// ���ͬʱ�ƶ�����֤����Ԫ����ͬ�߸���ȡ�ϴ�
		{
			*result = *first1;
			++first1;
			++first2;
		}
	}
	return std::copy(first1, last1, std::copy(first2, last2, result));//Ϭ���Ĵ��� ������� std��ͻ
	
}
}
#endif