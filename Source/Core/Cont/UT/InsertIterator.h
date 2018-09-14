#pragma once

#include "IteratorUtils.h"

/**
* Inserts element from the given position of the container.
*/
template<class ContType>
class TInsertIterator : 
	public TIteratorBase
	<
		TInsertIterator<ContType>,
		/*IsConst*/false, EIteratorClass::Forward
	>
{
public:
	using BaseIteratorType = typename ContType::IteratorType;
	using ElementType = typename ContType::ElementType;

	static_assert(ExactDefaultConstructible<ElementType>::Value, "TInsertIterator: Now only default-constructible types are supported");

	/**
	* Default-constructs iterator by default-constructing the underlying iterator.
	*/
	TInsertIterator() = default;

	/**
	* Constructs iterator from the given container and underlying iterator.
	*/
	TInsertIterator(ContType* pInCont, BaseIteratorType InIt) :
		pCont{pInCont}
	,	It{InIt}
	{
		BOOST_ASSERT(pCont);
	}

	/**
	* Constructs iterator by copying the other iterator.
	*/
	TInsertIterator(const TInsertIterator& InOther) = default;

	/**
	* Constructs iterator by moveing the other iterator.
	*/
	TInsertIterator(TInsertIterator&& InOther) = default;

	/**
	* Copies the iterator.
	*/
	TInsertIterator& operator=(const TInsertIterator& InOther) = default;

	/**
	* Move-copies the iterator.
	*/
	TInsertIterator& operator=(TInsertIterator&& InOther) = default;

	
	bool operator==(const TInsertIterator& OtherIt) const
	{
		return It != OtherIt.It;
	}

	bool operator!=(TInsertIterator& OtherIt) const
	{
		return ! (operator==(OtherIt))
	}

	bool IsEnd() const
	{
		// We never should return end
		return false;
	}

	operator bool () const
	{
		return ! IsEnd();
	} 

	
	ElementType& Get() const
	{
		return It.Get();
	} 

	ElementType& operator*() const
	{
		return Get();
	} 

	ElementType* GetPtr() const
	{
		return It.GetPtr();
	} 

	ElementType* operator->() const
	{
		return GetPtr();
	} 

	TInsertIterator& operator++(int)
	{
		BaseIteratorType OldIt = It;
		pContType->Insert(OldIt, ElementType{});
		if(OldIt)
		{
			It = GetNextIt(OldIt);
		}
		return *this; 
	}

	TInsertIterator operator++(int)
	{
		TInsertIterator OldIt = *this;
		++It;
		return OldIt; 
	}

	/**
	* Returns base iterator.
	*/
	BaseIteratorType ToBaseIterator() const { return It; }

private:
	ContType* pContType = nullptr;
	BaseIteratorType It;
};

/**
* TODO Insert iterator:
* 1. Requirements to the underlying type?:
* 1.1. Should be default-constructible or copy constructible?
* 2. Should we insert a new element at the construction time?
*
* TODO:
* 1. Emplace iterator
* 2. Insert at back iterator
*/