#pragma once

/**
* Default comparer.
* Uses the operator< for less comparison and > for greater comparison.
*/
template<class LT, class RT>
struct TComparer
{
	/**
	* Compares the given values and returns compare code.
	* Used to avoid double comparison for the case of types with heavy comparison.
	*
	* @returns:
	* - MINUSE ONE if A less than B;
	* - ONE if B less than A;
	* - ZERO if equal;
	*/
	int Compare(const LT& A, const RT& B) const
	{
		if (A < B)
		{
			return -1;
		}

		if (B < A)
		{
			return 1;
		}

		return 0;
	}
};

/**
* Performs comparison with the default comparer
*/
template<class LT, class RT>
int DefaultCompare(const LT& A, const RT& B)
{
	return TComparer<LT,RT>().Compare(A, B);
}

template<class LT, class RT>
bool CompareLess(const LT& A, const RT& B, TComparer<LT, RT> Comparer)
{
	return Comparer.Compare(A, B) < 0;
}

template<class LT, class RT>
bool CompareGreater(const LT& A, const RT& B, TComparer<LT, RT> Comparer)
{
	return Comparer.Compare(A, B) > 0;
}


template<class LT, class RT>
bool CompareEqual(const LT& A, const RT& B, TComparer<LT, RT> Comparer)
{
	return Comparer.Compare(A,B) == 0;
}


template<class LT, class RT>
bool CompareLessOrEqual(const LT& A, const RT& B, TComparer<LT, RT> Comparer)
{
	return Comparer.Compare(A, B) <= 0;
}

template<class LT, class RT>
bool CompareGreaterOrEqual(const LT& A, const RT& B, TComparer<LT, RT> Comparer)
{
	return Comparer.Compare(A, B) >= 0;
}