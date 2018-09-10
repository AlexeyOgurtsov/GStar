#pragma once

#include "ContainerSystem.h"
#include "TRBTree.h"

/************************************************************************************************************
* Sorted Set
************************************************************************************************************/
template<class T, class ComparerArg = TComparer<T>>
class TSortedSet
{
public:
	friend boost::serialization::access;

	/**
	* Any sorted set instance is to be treated as a friend to make it possible to optimize some operations. 
	*/
	friend class TSortedSet;

	/**
	* Comparison.
	*/
	template<class T, class ComparerArg, class OtherComparerArg>
	friend bool operator==(const TSortedSet<T, ComparerArg>& A, const TSortedSet<T, OtherComparerArg>& B);

	using KeyType = T;
	using ElementType = KeyType;

private:
	static auto GetThisTypeHelper()->std::remove_reference_t<decltype(*this)>;

	using KeyValueType = TKeyValue<KVType<T, NoValue>>;
	using ContImplType = TRBTree<KVType<T, NoValue>, ComparerArg>;

public:
	using IteratorType = typename ContImplType::KeyIteratorType;
	using ConstIteratorType = IteratorType;

	/**
	* Type of this container.
	*/
	using ThisType = decltype(GetThisTypeHelper());
	
	/**
	* Capacity to be used for the buffer by default.
	*/
	constexpr static int DEFAULT_CAPACITY = ContImplType::DEFAULT_CAPACITY;

	/**
	* Constructs with the default capacity.
	*/
	TSortedSet() : TSortedSet{ DEFAULT_CAPACITY } {}

	/**
	* Constructs with the given capacity.
	*/
	explicit TSortedSet(int InCapacity) : Cont { InCapacity } {}

	/**
	* Constructs as a copy of another container.
	*/
	TSortedSet(const TSortedSet& InOther) = default;

	/**
	* Constructs as a copy of another container.
	*/
	template<class OtherComparerArg>
	TSortedSet(const TSortedSet<T, OtherComparerArg>& InOther) : Cont(InOther.Cont) {}

	/**
	* Copy-assigns another container.
	*/
	TSortedSet& operator=(const TSortedSet& InOther) = default;

	/**
	* Copy-assigns another container.
	*/
	template<class OtherComparerArg>
	ThisType& operator=(const TSortedSet<T, OtherComparerArg>& InOther)
	{
		return Cont = InOther.Cont;
	}

	/**
	* Move-constructs.
	*/
	TSortedSet(TSortedSet&& InOther) = default;

	/**
	* Move-constructs.
	*/
	template<class OtherComparerArg>
	TSortedSet(TSortedSet<T, OtherComparerArg>&& InOther) : Cont(std::move(InOther.Cont)) {}

	/**
	* Move-assigns another container.
	*/
	TSortedSet& operator=(TSortedSet&& InOther) = default;

	/**
	* Move-assigns another container.
	*/
	template<class OtherComparerArg>
	ThisType& operator=(TSortedSet<T, OtherComparerArg>&& InOther)
	{
		Cont = std::move(InOther.Cont);
		return *this;
	}

	/**
	* Count of elements.
	*/
	__forceinline int Num() const { return Cont.Num(); }

	/**
	* Returns true if buffer is empty.
	*/
	__forceinline bool Empty() const { return Cont.Empty(); }

	/**
	* Iterator to the first element.
	*/
	IteratorType Iterator() const { return Cont.KeyIterator(); }

	/**
	* Iterator to the after-the-last element.
	*/
	IteratorType EndIterator() const { return Cont.EndKeyIterator(); }

	/**
	* Iterator to the first element.
	*/
	ConstIteratorType ConstIterator() const { return Iterator(); }

	/**
	* Iterator to the after-the-last element.
	*/
	ConstIteratorType ConstEndIterator() const { return EndIterator(); }

	IteratorType begin() const { return Iterator(); }
	IteratorType end() const { return EndIterator(); }
	IteratorType cbegin() const { return ConstIterator(); }
	IteratorType cend() const { return ConstEndIterator(); }

	/**
	* Returns minimal key.
	* Container must be NON-empty.
	*/
	const KeyType& Min() const
	{
		BOOST_ASSERT_MSG( ! Empty(), "TSortedSet::Min: Container must be NON-empty");
		return Cont.MinKey();
	}

	/**
	* Returns maximal key.
	* Container must be NON-empty.
	*/
	const KeyType& Max() const
	{
		BOOST_ASSERT_MSG( ! Empty(), "TSortedSet::Max: Container must be NON-empty");
		return Cont.MaxKey();
	}

	/**
	* boost::serialization support.
	*/
	template<class Archive>
	void serialize(Archive& Ar, const unsigned int Version)
	{
		Ar & Cont;
	}

	/**
	* Count bytes needed to store this class without overhead of capacity.
	* Include both the class layout and dynamic memory.
	*/
	int32_t CountTotalBytes() const
	{
		return Cont.CountTotalBytes();
	}

	/**
	* Count bytes needed to store only the minimum size required to store the given buffer.
	*/
	int32_t CountMinimumSizeBytes() const
	{
		return Cont.CountMinimumSizeBytes();
	}

	/**
	* Counts the overhead of capacity.
	*/
	int32_t CountCapacityOverhead() const
	{
		return CountTotalBytes() - CountMinimumSizeBytes();
	}


	/**
	* Count only bytes that are to be serialized.
	*/
	int32_t CountSerializeBytes() const
	{
		return Cont.CountSerializeBytes();
	}

	/**
	* Size of a single element instance.
	* NOTE: For pointers accounts only pointer size and not the size of pointed-to content.
	*/
	static constexpr int32_t ElementSize()
	{
		return Cont.CountElementSize();
	}

	/**
	* Clears the container.
	*/
	void Clear()
	{
		Cont.Clear();
	}

	/**
	* Makes the buffer capable to contain at least the given amount of elements.
	*/
	void ReserveGrow(int InNewCapacity)
	{
		Cont.ReserveGrow(InNewCapacity);
	}

	/**
	* Frees extra space.
	* Optionally rearranges the elements in the buffer, so they are linear in memory.
	*/
	void ShrinkToFit()
	{
		Cont.ShrinkToFit();
	}

	/**
	* Returns true, if contains the given key.
	*/
	bool Contains(const KeyType& InKey) const
	{
		return Cont.Contains(InKey);
	}

	/**
	* Returns true, if contains element satisfying the given predicate.
	*/
	template<class Pred>
	bool ContainsPredicate(const Pred& P) const
	{
		return Cont.ContainsKeyPredicate(P);
	}

	/**
	* Returns true, if contains element that does NOT satisfy the given predicate.
	*/
	template<class Pred>
	bool ContainsNotPredicate(const Pred& P) const
	{
		return Cont.ContainsNotKeyPredicate(P);
	}

	/**
	* Returns true, if contains element satisfying the given predicate in the given range.
	*/
	template<class Pred>
	bool ContainsPredicate_InRange(const Pred& P, ConstIteratorType ItFirst, ConstIteratorType ItLast) const
	{
		return Cont.ContainsKeyPredicate_InRange(P, ItFirst.ToBaseIterator(), ItLast.ToBaseIterator());
	}

	/**
	* Returns true, if contains element that does NOT satisfy the given predicate in the given range.
	*/
	template<class Pred>
	bool ContainsNotPredicate_InRange(const Pred& P, ConstIteratorType ItFirst, ConstIteratorType ItLast) const
	{
		return Cont.ContainsNotKeyPredicate_InRange(P, ItFirst.ToBaseIterator(), ItLast.ToBaseIterator());
	}

	/**
	* Checks that all elements satisfy the given predicate.
	*/
	template<class Pred>
	bool All(const Pred& P) const
	{
		return AllKeys(P);
	}

	/*
	* Removes the given key from the container.
	*
	* @returns: true, if was found and removed (otherwise false). 
	*/
	bool Remove(const KeyType& InKey)
	{
		return Cont.Remove(InKey);
	}

	/*
	* Removes the given key from the container using the given comparer.
	*
	* @returns: true, if was found and removed (otherwise false).
	*/
	template<class SearchKeyType, class Comparer>
	bool Remove(const SearchKeyType& InKey, Comparer InComparer)
	{
		return Cont.Remove(InKey, InComparer);
	}

	/**
	* Removes the first key that satisfy the given predicate.
	*
	* @returns: true if element with the given predicate was in the container (otherwise, false).
	*/
	template<class Pred>
	bool RemoveFirstPredicate(const Pred& P)
	{
		return Cont.RemoveFirstKeyPredicate(P);
	}

	/**
	* Removes the first key that satisfy the given predicate in the given range.
	*
	* @returns: true if element with the given predicate was in the container (otherwise, false);
	*/
	template<class Pred>
	bool RemoveFirstPredicate_InRange(const Pred& P, ConstIteratorType FirstIt, ConstIteratorType LastIt)
	{
		return Cont.RemoveFirstKeyPredicate_InRange(P, FirstIt.ToBaseIterator(), LastIt.ToBaseIterator());
	}

	/**
	* Removes all elements that satisfy the given predicate.
	*
	* @returns: number of removed elements.
	* @see: RemoveFirstPredicate
	*/
	template<class Pred>
	int32_t RemoveAllPredicate(const Pred& P)
	{
		return Cont.RemoveAllKeyPredicate(P);
	}

	/**
	* Removes all elements that satisfy the given predicate.
	*
	* @returns: number of removed elements.
	* @see: RemoveFirstPredicate
	*/
	template<class Pred>
	int32_t RemoveAllPredicate_InRange(const Pred& P, ConstIteratorType FirstIt, ConstIteratorType LastIt)
	{
		return Cont.RemoveAllKeyPredicate_InRange(P, FirstIt.ToBaseIterator(), LastIt.ToBaseIterator());
	}

	/*
	* Moves the given key from the container.
	*
	* @returns: true, if was found and moved (otherwise false).
	*/
	//bool MoveKey(const KeyType& InKey, KeyType& OutMovedKey)
	//{
	//	return Cont.MoveKeyOnly(InKey, OutMovedKey);
	//}

	/**
	* Adds a new key to the container.
	*
	* @Returns: true if was added (or false if already was in the container).
	*/
	bool Add(const KeyType& InKey)
	{
		static_assert(std::is_copy_constructible_v<KeyType>, "TSortedSet: Add: KeyType must be copy-constructible");
		return Cont.Add(KeyValueType{InKey, NoValue{}});
	}


	/**
	* Adds a new key to the container by moving.
	*
	* @Returns: true if was added (or false if already was in the container).
	*/
	bool Add(KeyType&& InKey)
	{
		static_assert(std::is_move_constructible_v<KeyType>, "TSortedSet: Add (&&): KeyType must be move-constructible");
		return Cont.Add(KeyValueType{std::move(InKey), NoValue{}});
	}


	/**
	* Adds keys from C-array buffer. 
	* Keys are copied.
	*/
	void Add(const KeyType* pInSource, int32_t InCopiedCount)
	{
		BOOST_REQUIRE(pInSource);
		// @TODO: Optimize: reserve the space
		//Cont.ReserveGrow(Count + InCopiedCount);
		for (int i = 0; i < InCopiedCount; i++) 
		{
			Add(pInSource[i]);
		}
	}

	/**
	* Adds the given range of keys, assuming that the range is sorted.
	*/
	void AddSorted(const KeyType* pInSource, int32_t InCopiedCount)
	{
		Add(pInSource, InCopiedCount);
	}

	/**
	* Adds keys from C-array buffer.
	* Keya are moved.
	*/
	void AddMoved(KeyType* pInSource, int32_t InCopiedCount)
	{
		BOOST_ASSERT(pInSource);
		// @TODO: Optimize: reserve the space
		//Buffer.ReserveGrow(Count + InCopiedCount);
		for (int i = 0; i < InCopiedCount; i++)
		{
			Add(std::move(pInSource[i]));
		}
	}

	/**
	* Adds keys from C-array buffer, assuming that the range is sorted.
	*/
	void AddMovedSorted(KeyType* pInSource, int32_t InCopiedCount)
	{
		AddMoved(pInSource, InCopiedCount);
	}

	template<class Strm>
	void Print(Strm& S) const
	{
		S << Cont;
	}

private:
	ContImplType Cont;
};

template<class T, class ComparerArg, class OtherComparerArg>
bool operator==(const TSortedSet<T, ComparerArg>& A, const TSortedSet<T, OtherComparerArg>& B)
{
	return A.Cont == B.Cont;
}

template<class T, class ComparerArg, class OtherComparerArg>
bool operator!=(const TSortedSet<T, ComparerArg>& A, const TSortedSet<T, OtherComparerArg>& B)
{
	return ! operator==(A, B);
}

template
<
	class Strm, class T, class ComparerArg, 
	typename = typename std::enable_if<typename IsOutputStream<Strm>::Value>::type
>
Strm& operator<<(Strm& S, const TSortedSet<T, ComparerArg>& InCont)
{
	InCont.Print(S);
	return S;
}

/**
* TODO:
* 1. Constructors
* 2. Copying/Moving
* 3. Output
* 4. Equality
* 6. Operations:
* 6.1. Getters
* 6.2. Add
* 6.3. Remove
* 6.3.0. 
* 6.3.0.1. TODO: Add conversion from KeyIterator to KeyValueIterator.
* 6.3.1. Remove by key
* 6.3.2. Remove all
* 6.3.3. Remove first
* 7. Find
*
* DONE:
* 1. Iterator
* 2. Contains
* 3. Extra:
* 3.1. Serialize
*/