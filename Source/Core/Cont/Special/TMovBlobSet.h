#pragma once

#include "../ContainerSystem.h"
#include <set>
#include "../TVector.h"

struct DefaultMovBlobSetPolicy
{
	static constexpr float DEFAULT_CAPACITY = 1024;	
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
	TMovBlobSet() 
	{
		BOOST_ASSERT_MSG(false, "TMovBlobSet: default constructor: NOT yet implemented");
	}

	/**
	* Creates a set.
	* Initial buffer capacity is provided as an argument.
	*/
	TMovBlobSet(int InInitialBufferCapacity) 
	{
		BOOST_ASSERT_MSG(false, "TMovBlobSet: constructor: NOT yet implemented");
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
	__forceinline int Count() const
	{
		BOOST_ASSERT_MSG(false, "TMovBlobSet: Count: NOT yet implemented"); return 0;
	}

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
	struct BlobHeader
	{
		/**
		* Length of the Blob (excluding THE header!!!).
		*/
		int Length;

		BlobHeader(int InLength) : Length(InLength) {}
	};

	/**
	* Buffer of all blobs. 
	* Each blob contains the header immediately followed by the blob data (of length specified in the header).
	*/
	Eng::TVector<Byte> Buffer;
	//std::set<const Byte*, BlobsEqual> Set;
};