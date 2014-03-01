#ifndef SET_INTERSECTION_H
#define SET_INTERSECTION_H
namespace tf{
// ����
template <class InputIter1, class InputIter2, class OutputIter>
OutputIter set_intersection(InputIter1 first1, InputIter1 last1,
						InputIter2 first2, InputIter2 last2,
						OutputIter result)
{
	for ( ; first1 != last1 && first2 != last2; )
	{
		if (*first1 < *first2)
		{
			++first1; 
		}
		else if (*first2 < *first1)
		{
			++first2;
		}
		else// ���ȣ��Ƶ���С��,ʹ��ȣ�ȡ����1,ͬʱ�ƶ���������
		{
			++result = *first1;

			++first1;
			++first2;

			++result;
		}
	}
	return result;
}

template <class InputIter1, class InputIter2, class OutputIter, class Compare>
OutputIter set_intersection(InputIter1 first1, InputIter1 last1,
						InputIter2 first2, InputIter2 last2,
						OutputIter result,
						Compare cmp)
{
	for ( ; first1 != last1 && first2 != last2; )
	{
		if (cmp(*first1, *first2))
		{
			++first1; 
		}
		else if (cmp(*first2, *first1))
		{
			++first2;
		}
		else// ���ȣ��Ƶ���С��,ʹ��ȣ�ȡ����1,ͬʱ�ƶ���������
		{
			++result = *first1;

			++first1;
			++first2;

			++result;
		}
	}
	return result;
}
}
#endif