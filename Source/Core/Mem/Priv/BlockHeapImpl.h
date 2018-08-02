#pragma once

namespace BlockHeapImpl
{
	struct AllocHeader;

	struct Policy
	{
		int DefaultBlockCapacity;
	};

	/**
	* Implementation of the BlockHeap class.
	*/
	class BlockHeap
	{
	public:
		~BlockHeap();

		explicit BlockHeap(const Policy& InPolicy);
		explicit BlockHeap(int InBlockCapacity);

		BlockHeap(const BlockHeap&) = delete;
		BlockHeap& operator=(const BlockHeap&) = delete;
		
		BlockHeap(BlockHeap&&);
		BlockHeap& operator=(BlockHeap&&);

		void* StoreBytes(const Policy* pInPolicy, const void* pInSrc, int InSize);

	private:
		AllocHeader *ReserveBlock(const Policy* pInPolicy, int InDesiredSize);
		AllocHeader *AppendBlockOfExactCapacity(int InBlockCapacity);

		BlockHeapImpl::AllocHeader* pHead = nullptr;
		BlockHeapImpl::AllocHeader* pTop = nullptr;
	};
} // BlockHeapImpl