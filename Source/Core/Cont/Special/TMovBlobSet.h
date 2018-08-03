#pragma once

#include "Priv/TMovBlobSetImpl.h"

/**
* TODO:
* 1. Clear operation;
* 2. Iteration;
* 3. First/Last;
* 4. Resize;
* 5. LowerBound/UpperBound/Range/EqualRange
*/

struct DefaultMovBlobSetPolicy
{
	static constexpr float DEFAULT_CAPACITY = 1024;

	template<class T>
	using BufferResizePolicy = Eng::DefaultResizePolicy<T>;
};

/**
* Set of movable byte blobs.
*
* Blobs are owned and always stored linearly in memory.
*
* Blobs can move, meaning their addresses may change.
* However, blob offsets relative to the buffer start remain consistent among reallocations.
*
* Keys are immutable.
*
* Keys may be removed from the set, however corresponding allocated space will not be freed.
*
* Keys can be iterated in sorted order.
*
* Keys are compared lexicographically.
*/
template<class MovBlobSetPolicy = DefaultMovBlobSetPolicy>
class TMovBlobSet
{
public:
	/**
	* Creates a set.
	* Buffer is initialized with default storage capacity.
	*/
	TMovBlobSet() : TMovBlobSet{ MovBlobSetPolicy::DEFAULT_CAPACITY } {}

	/**
	* Creates a set.
	* Initial buffer capacity is provided as an argument.
	*/
	explicit TMovBlobSet(int InInitialBufferCapacity) :
		S{ /*TMovBlobSetImpl::BlobEqual{}*/ }
	{
		Buffer.ResizeGrow(InInitialBufferCapacity);
	}

	/**
	* Constructs a copy.
	*/
	TMovBlobSet(const TMovBlobSet& InOther) = default;

	/**
	* Copies.
	*/
	TMovBlobSet& operator=(const TMovBlobSet& InOther) = default;

	/**
	* Moves.
	*/
	TMovBlobSet(TMovBlobSet&& InOther) = default;

	/**
	* Copy-moves.
	*/
	TMovBlobSet& operator=(TMovBlobSet&& InOther) = default;

	/**
	* Returns count of elements in the set.
	*/
	__forceinline int Count() const { return S.size(); }

	/**
	* Returns true if set is empty.
	*/
	__forceinline bool Empty() const { return 0 == Count(); }

	/**
	* Copies sequence of bytes into the buffer and registers in the set if the key is not in the set yet.
	*
	* @returns: index of the key (already registered or new).
	*/
	int Add(const void* pInBytes, int InSize)
	{
		BOOST_ASSERT_MSG(false, "TMovBlobSet: Add: NOT yet implemented"); return 0;

		// TODO: Check contained

		// NOTE: Added index always must be getted before the Append
		int const AddedIndex = Buffer.LastIndex();
		Buffer.Append(static_cast<const Byte*>(pInBytes), InSize);

		// TODO: Add to set
	}

	/**
	* Removes the key from the set.
	* @returns: true if was removed (otherwise it was not in the set already).
	*/
	bool Remove(const void* pInBytes, int InSize)
	{
		BOOST_ASSERT_MSG(false, "TMovBlobSet: Remove: NOT yet implemented"); return false;
	}

	/**
	* Returns true if the set contains the given key.
	*/
	bool Contains(const void* pInBytes, int InSize) const
	{
		BOOST_ASSERT_MSG(false, "TMovBlobSet: Contains: NOT yet implemented"); return false;
	}

private:
	Eng::TVector<Byte, DefaultMovBlobSetPolicy::BufferResizePolicy> Buffer;

	/**
	* Set of Blob indices into the Buffer.
	*/
	// TODO: Set cannot be used
	std::set<int> S;
};