#pragma once

/**
* Result of A,B comparison: A < B
*/
constexpr int COMPARE_LESS = -1;

/**
* Result of A,B comparison: A == B
*/
constexpr int COMPARE_EQUAL = 0;

/**
* Result of A,B comparison: A > B
*/
constexpr int COMPARE_GREATER = 1;

/**
* Helper function for checking result of the Comparer's Compare function on equality.
*/
__forceinline bool CompareEqual(int InCompareResult) { return COMPARE_EQUAL == InCompareResult; }

/**
* Helper function for checking result of the Comparer's Compare function on A < B.
*/
__forceinline bool CompareLess(int InCompareResult) { return COMPARE_LESS == InCompareResult; }

/**
* Helper function for checking result of the Comparer's Compare function on A > B.
*/
__forceinline bool CompareGreater(int InCompareResult) { return COMPARE_GREATER == InCompareResult; }

/**
* Helper function for checking result of the Comparer's Compare function on A <= B.
*/
__forceinline bool CompareLessOrEqual(int InCompareResult) { return InCompareResult <= COMPARE_EQUAL; }

/**
* Helper function for checking result of the Comparer's Compare function on A >= B.
*/
__forceinline bool CompareGreaterOrEqual(int InCompareResult) { return COMPARE_EQUAL <=  InCompareResult; }

/**
* Default comparer.
* Uses the operator< for less comparison and > for greater comparison.
*/
template<class LT, class RT>
struct TGeneralComparer
{
	/**
	* Compares the given values and returns compare code.
	* Used to avoid double comparison for the case of types with heavy comparison.
	*
	* @returns:
	* - COMPARE_LESS if A less than B;
	* - COMPARE_GREATER if B less than A;
	* - COMPARE_EQUAL if equal;
	*/
	int Compare(const LT& A, const RT& B) const
	{
		if (A < B)
		{
			return COMPARE_LESS;
		}

		if (B < A)
		{
			return COMPARE_GREATER;
		}

		return COMPARE_EQUAL;
	}
};

/**
* Type alias for comparer where both left and right sides are equal.
*/
template<class T>
using TComparer = TGeneralComparer<T, T>;

/**
* Compares values that the pointers point to.
* Neither of the sides may be nullptr.
*/
template<class T>
struct TDereferenceComparer
{
	int Compare(const T* pA, const T* pB) const
	{
		BOOST_ASSERT_MSG(pA, "TDereferenceComparer: left pointer is NOT allowed to be nullptr");
		BOOST_ASSERT_MSG(pB, "TDereferenceComparer: right pointer is NOT allowed to be nullptr");
		return DefaultCompare(*pA, *pB);
	}
};

/**
* Compares the given value and the value that the pointer points to.
* Pointer may be nullptr.
*/
template<class T>
struct TRightDereferenceComparer
{
	int Compare(const T& A, const T* pB) const
	{
		BOOST_ASSERT_MSG(pB, "TRightDereferenceComparer: right pointer is NOT allowed to be nullptr");
		return DefaultCompare(A, *pB);
	}
};

/**
* Compares the given value and the value that the pointer points to.
* Pointer may be nullptr.
*/
template<class T>
struct TLeftDereferenceComparer
{
	int Compare(const T* pA, const T& B) const
	{
		BOOST_ASSERT_MSG(pA, "TLeftDereferenceComparer: left pointer is NOT allowed to be nullptr");
		return DefaultCompare(*pA, B);
	}
};

/**
* Performs comparison with the default comparer
*/
template<class LT, class RT>
int DefaultCompare(const LT& A, const RT& B)
{
	return TGeneralComparer<LT,RT>().Compare(A, B);
}

template<class LT, class RT>
bool CompareLess(const LT& A, const RT& B, TGeneralComparer<LT, RT> Comparer)
{
	return CompareLess(Comparer.Compare(A, B));
}

template<class LT, class RT>
bool CompareGreater(const LT& A, const RT& B, TGeneralComparer<LT, RT> Comparer)
{
	return CompareGreater(Comparer.Compare(A, B));
}

template<class LT, class RT>
bool CompareEqual(const LT& A, const RT& B, TGeneralComparer<LT, RT> Comparer)
{
	return CompareEqual(Comparer.Compare(A,B));
}

template<class LT, class RT>
bool CompareLessOrEqual(const LT& A, const RT& B, TGeneralComparer<LT, RT> Comparer)
{
	return CompareLessOrEqual(Comparer.Compare(A, B));
}

template<class LT, class RT>
bool CompareGreaterOrEqual(const LT& A, const RT& B, TGeneralComparer<LT, RT> Comparer)
{
	return CompareGreaterOrEqual(Comparer.Compare(A, B));
}