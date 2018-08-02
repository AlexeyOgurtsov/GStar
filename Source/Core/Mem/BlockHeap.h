#pragma once 

#include "Priv/BlockHeapImpl.h"

/**
* Determines how to grow the heap.
*/
struct DefaultBlockHeapPolicy
{
	/**
	* Block size to be used by default.
	*/
	static constexpr int DEFAULT_BLOCK_SIZE = 4 * 1024;
};

/**
* Minimizes alloc calls by allocating in blocks;
* Destruction is NOT supported (but allocs are automatically cleared at the destruction time)
*/

template<class HeapPolicyArg = DefaultBlockHeapPolicy>
class BlockHeap
{
public:
	/**
	* Automatically destructs all the allocs.
	*/
	~BlockHeap() = default;

	/**
	* Constructs the heap with one block of the default capacity.
	*/
	BlockHeap() : Impl{ ImplPolicyFromPolicy(InBlockCapacity) } {}

	/**
	* Constructs the heap with one block of the given capacity.
	*/
	explicit BlockHeap(int InBlockCapacity) : Impl{ InBlockCapacity } {}

	/**
	* Forbid copying.
	*/
	BlockHeap(BlockHeap& InOther) = delete;
	
	/**
	* Forbid copying.
	*/
	BlockHeap& operator=(BlockHeap& InOther) = delete;

	/**
	* Constructs by moving.
	*/
	BlockHeap(BlockHeap&& InOther) : Impl(std::move(InOther)) {}

	/**
	* Moves.
	*/
	BlockHeap& operator=(BlockHeap&& InOther) { Impl = std::move(InOther.Impl); return *this; }

	/**
	* Copies bytes and stores on the heap.
	*/
	void* StoreBytes(const void* pInSrc, int InSize)
	{
		BlockHeapImpl::Policy ImplPolicy = ImplPolicyFromPolicy();
		return Impl.StoreBytes(&ImplPolicy, pInSrc, InSize);
	}

private:
	__forceinline BlockHeapImpl::Policy ImplPolicyFromPolicy() const 
	{
		BlockHeapImpl::Policy ImplPolicy;
		ImplPolicy.DefaultBlockCapacity = DefaultBlockHeapPolicy::DEFAULT_BLOCK_SIZE;
		return ImplPolicy;
	}
	BlockHeapImpl::BlockHeap Impl;
};