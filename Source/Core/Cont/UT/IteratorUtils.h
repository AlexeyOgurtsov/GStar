#pragma once

#include "Core/CoreSysMinimal.h"
#include <type_traits>

/**
* Non-template part of the iterator state (to facilitate the IsIterator check)
*/
class IteratorBase
{
public:
};

/**
* Base class for all iterators.
*/
template<class bIsConstArg>
class TIteratorBase : public IteratorBase
{
public:
	using IsConst = bIsConstArg;
};

/**
* Returns true if the given class is iterator.
*/
template<class T>
struct IsIterator
{
	static constexpr bool Value = std::is_base_of_v<IteratorBase, T>;
};

/**
* Returns true if the right iterator is assignable to the left one according to constantness rules.
*/
template<class LeftType, class RightType>
struct IteratorAssignableByConst
{
	static constexpr bool Value = ( LeftType::IsConst::value ) || ( LeftType::IsConst::value == RightType::IsConst::value );
};

/**
* Reverses order of iteration for the given iterator.
*/
template<class IteratorTypeArg>
class TReverseIterator
{
public:
	using IteratorType = typename IteratorTypeArg;

	using IsConst = typename IteratorType::IsConst;

	/**
	* Creates iterator in Default state.
	*/
	TReverseIterator() = default;

	/**
	* Creates iterator from the given iterator.
	*/
	TReverseIterator(IteratorType InIt) : It{ InIt } {}

	/**
	* Copy-constructs from another reverse iterator.
	*/
	template<class OtherIteratorType>
	TReverseIterator(const TReverseIterator<OtherIteratorType>& InIt)
	{
		operator=(InIt);
	}

	/**
	* Copies from another reverse iterator.
	*/
	template<class OtherIteratorType>
	TReverseIterator& operator=(const TReverseIterator<OtherIteratorType>& InIt)
	{
		static_assert( IteratorAssignableByConst::Value, "TReverse iterator: copy: const/non-const iterator mismatch");
		It = InIt;
		return *this;
	}

	/**
	* Advances iterator to the next element.
	*/
	TReverseIterator& operator++()
	{
		BOOST_ASSERT_MSG(!It.IsFloatingEnd(), "TReverseIterator: increment(++): underlying iterator is at the floating end");
		BOOST_ASSERT_MSG(!IsEnd(), "TReverseIterator: increment(++): already at end");
		if( ! It.AtStart() )
		{
			--It;
		}
		else
		{
			// making end iterator
			*this = TReverseIterator();
		}
		return *this;
	}

	/**
	* Advances iterator to the next element.
	*/
	TReverseIterator operator++(int)
	{
		TReverseIterator OldIt = *this;
		TReverseIterator::operator++();
		return OldIt;
	}

	/**
	* Advances iterator to the previous element.
	*/
	TReverseIterator& operator--()
	{
		BOOST_ASSERT_MSG(!It.IsFloatingEnd(), "TReverseIterator: decrement(--): underlying iterator is at the floating end");	
		++It;
		return *this;
	}

	/**
	* Advances iterator to the previous element.
	*/
	TReverseIterator operator--(int)
	{
		TReverseIterator OldIt = *this;
		TReverseIterator::operator--();
		return OldIt;
	}


	/**
	* Advances iterator forward by the given number of elements and returns a new iterator value.
	*/
	TReverseIterator operator+(int32_t Count) const
	{
		TReverseIterator NextIterator{ *this };
		NextIterator.Forward(Count);
		return NextIterator;
	}

	/**
	* Advances iterator forward by the given number of elements backward and returns a new iterator value.
	*/
	TReverseIterator operator-(int32_t Count) const
	{
		TReverseIterator NextIterator{ *this };
		NextIterator.Backward(Count);
		return NextIterator;
	}

	/**
	* Advances iterator forward by the given number of elements.
	*/
	void Forward(int32_t Count)
	{
		It.Backward(Count);
	}


	/**
	* Advances iterator backward by the given number of elements.
	*/
	void Backward(int32_t Count)
	{
		It.Forward(Count);
	} 

	/**
	* Returns original (non-reversed) iterator.
	*/
	IteratorType ToBaseIterator() const { return It; }

	/**
	* Current element.
	*/
	__forceinline decltype(auto) Get() const { return It.Get(); }

	/**
	* Gets ptr.
	*/
	__forceinline decltype(auto) GetPtr() const { return It.GetPtr(); }

	/**
	* Member access by pointer.
	*/
	__forceinline decltype(auto) operator->() const
	{
		return GetPtr();
	}


	/**
	* Current element.
	*/
	decltype(auto) operator*() const { return Get(); }

	/**
	* NOT Is end iterator.
	*/
	__forceinline operator bool() const
	{
		return !IsEnd();
	}

	/**
	* Is end iterator.
	*/
	__forceinline bool operator!() const
	{
		return IsEnd();
	}

	__forceinline bool IsEnd() const
	{
		return IsFloatingEnd();
	}

	__forceinline bool IsFloatingEnd() const
	{
		return It.IsFloatingEnd();
	}

	__forceinline bool AtStart() const { return It.AtLast(); }

	template<class OtherIteratorType>
	bool operator==(const TReverseIterator<OtherIteratorType>& InOther) const
	{
		return It == InOther.It;
	}

	template<class OtherIteratorType>
	bool operator!=(const TReverseIterator<OtherIteratorType>& InOther) const
	{
		return !(operator==(InOther));
	}

private:
	IteratorType It;
};


/**
* TODO:
* 1. Next/Prev operations.
* 2. IsConstIterator check.
* 3. AtStart:
* 3.1. Implement AtLast operation (points to the last element, NOT end).
*/