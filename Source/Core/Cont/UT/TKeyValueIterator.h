#pragma once

#include "IteratorUtils.h"
#include "TKeyValue.h"
#include <type_traits>

/**
* Forward key iterator.
*
* Key is always provided for const access only.
*
* @param: bExposeIterator - if true, the underlying iterator is to be exposed for public access.
*/
template<class IteratorTypeArg, bool bExposeIterator = false>
class TForwardKeyIterator : public TIteratorBase
<
	TForwardKeyIterator<IteratorTypeArg>, 
	/*bIsConstArg*/std::true_type, EIteratorClass::Forward
>
{
public:
	using ThisType = TForwardKeyIterator<IteratorTypeArg, bExposeIterator>;

	using IteratorType = IteratorTypeArg;
	using KeyValueType = typename IteratorTypeArg::ElementType;
	using ElementType = const typename KeyValueType::KeyType;

	/**
	* Default ctor: Creates end iterator
	*/
	TForwardKeyIterator() = default;

	/**
	* Constructs from iterator.
	*
	* Iterator must return TKeyValue.
	*/
	TForwardKeyIterator(IteratorTypeArg InIterator) : It{InIterator}
	{
		//static_assert(std::is_base_of<>::value, "Iterator element type must be TKeyValue<K, V>");
	}

	/**
	* Copy-constructs.
	*/
	TForwardKeyIterator(const TForwardKeyIterator& InOther) = default;

	/**
	* Copy-assigns.
	*/
	TForwardKeyIterator& operator=(const TForwardKeyIterator& InOther) = default;

	/**
	* Copy-constructs.
	*/
	template<class OtherIteratorType>
	TForwardKeyIterator(const TForwardKeyIterator<OtherIteratorType>& InOther) :
		It{InOther.It}
	{
		static_assert(IteratorAssignableByConst<IteratorTypeArg, OtherIteratorType>::Value, "TForwardKeyIterator: copy ctor: underlying iterators must be assignable");
	}

	/**
	* Copy-assigns.
	*/
	template<class OtherIteratorType>
	TForwardKeyIterator& operator=(const TForwardKeyIterator<OtherIteratorType>& InOther)
	{
		static_assert(IteratorAssignableByConst<IteratorTypeArg, OtherIteratorType>::Value, "TForwardKeyIterator: copy-assignment(=): underlying iterators must be assignable");
		It = InOther.It;
		return *this;
	}


	ElementType& Get() const 
	{
		return It->Key; 
	}

	ElementType& operator*() const
	{
		return Get();
	}

	ElementType* GetPtr() const
	{
		return &Get();
	}

	ElementType* operator->() const
	{
		return GetPtr();
	}

	ThisType operator++()
	{
		++It;
		return *this;
	}

	ThisType operator++(int)
	{
		ThisType It { *this };
		++It;
		return It;
	}

	/**
	* NOT Is end iterator.
	*/
	__forceinline operator bool() const
	{
		return It;
	}

	/**
	* Is end iterator.
	*/
	__forceinline bool operator!() const
	{
		return ! It;
	}

	__forceinline bool IsEnd() const
	{
		return It.IsEnd();
	}

	__forceinline bool IsFloatingEnd() const
	{
		return It.IsFloatingEnd();
	}

	__forceinline bool AtStart() const 
	{
		return It.AtStart(); 
	}

	template<class OtherIteratorType>
	bool operator==(const TForwardKeyIterator<OtherIteratorType>& InOther) const
	{
		static_assert(AreIteratorsComparable<IteratorTypeArg, OtherIteratorType>::Value, "TForwardKeyIterator: operator==: iterators must be comparable");
		return It == InOther.It;
	}

	template<class OtherIteratorType>
	bool operator!=(const TForwardKeyIterator<OtherIteratorType>& InOther) const
	{
		static_assert(AreIteratorsComparable<IteratorTypeArg, OtherIteratorType>::Value, "TForwardKeyIterator: operator!=: iterators must be comparable");
		return !(operator==(InOther));
	}

	template<typename = typename std::enable_if<bExposeIterator>::type>
	IteratorTypeArg GetBaseIterator() const { return It; }

private:
	IteratorTypeArg It;
};

/**
* TODO: 
* 1. Ctor
* 2. operator==
* 3. Access operators:
* 3.1. Dereference
* 4. Increment
*/