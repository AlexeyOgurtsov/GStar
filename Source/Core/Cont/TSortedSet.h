#include "ContainerSystem.h"
#include "TRBTree.h"
#include <type_traits>

/************************************************************************************************************
* Sorted Set
************************************************************************************************************/
template<class T, class ComparerArg>
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

	/**
	* KeyType
	*/
	using KeyType = T;

private:
	static auto GetThisTypeHelper()->std::remove_reference_t<decltype(*this)>;

public:
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
	TSortedSet(const ThisType& InOther) = default;

	/**
	* Constructs as a copy of another container.
	*/
	template<class OtherComparerArg>
	TSortedSet(const TSortedSet<T, OtherComparerArg>& InOther) : Cont(InOther.Cont) {}

	/**
	* Copy-assigns another container.
	*/
	ThisType& operator=(const ThisType& InOther) = default;

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
	TSortedSet(ThisType&& InOther) = default;

	/**
	* Move-constructs.
	*/
	template<class OtherComparerArg>
	TSortedSet(TSortedSet<T, OtherComparerArg>&& InOther) : Cont(std::move(InOther.Cont)) {}

	/**
	* Move-assigns another container.
	*/
	ThisType& operator=(ThisType&& InOther) = default;

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
	void serialize(Archive& Ar, const unsinged int Version)
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
	int32_t ElementSize() const
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
	* Returns true, if contains the given key.
	*/
	bool Contains(const KeyType& InKey) const
	{
		return Cont.Contains(InKey);
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
		return Cont.AddCheck(KeyValueType{InKey, NoValue{}});
	}


	/**
	* Adds a new key to the container by moving.
	*
	* @Returns: true if was added (or false if already was in the container).
	*/
	bool Add(KeyType&& InKey)
	{
		static_assert(std::is_move_constructible_v<KeyType>, "TSortedSet: Add (&&): KeyType must be move-constructible");
		return Cont.AddCheck(KeyValueType{std::move(InKey), NoValue{}});
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
	using KeyValueType = TKeyValue<T, NoValue>;
	using ContImplType = TRBTree<KVType<T, NoValue>, ComparerArg>;

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
* 5. Serialization
* 6. Operations:
* 6.1. Getters
* 6.2. Add
* 6.3. Remove
* 7. Iterator
*/