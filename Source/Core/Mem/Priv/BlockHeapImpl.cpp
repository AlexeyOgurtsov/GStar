#include "BlockHeapImpl.h"
#include "Core/CoreSysMinimal.h"
#include <cstdlib> // malloc/free
#include <new> // placement new

namespace BlockHeapImpl
{
	struct AllocHeader
	{
		/**
		* Size, including the header.
		*/
		int Size;

		/**
		* Already allocated size (or offset to the top byte).
		* WARNING!!!
		* Offset is relative to the header start,
		* so initially offset must be equal to the size of the header.
		*/
		int Offset;

		/**
		* Points to next allocation header (or nullptr).
		*/
		AllocHeader* pNext = nullptr;

		/**
		* Helper to determine the slack.
		*/
		__forceinline int Slack() const { return Size - Offset; }

		AllocHeader(int InSize) : Size(InSize), Offset(sizeof(AllocHeader)) {}
	};

	namespace
	{
		void DestroyBlock(AllocHeader* pHeader)
		{
			free(pHeader);
		}

		AllocHeader* AllocBlock(int InExactBlockSize)
		{
			BOOST_ASSERT_MSG(InExactBlockSize > 0, "BlockHeap does NOT allow to reserve blocks of zero size");
			int const MallocSize = sizeof(AllocHeader) + InExactBlockSize;
			void* pBlock = malloc(MallocSize);
			AllocHeader* pAllocHeader = new(pBlock) AllocHeader{ MallocSize };
			return pAllocHeader;
		}

		void* AppendDataToBlock(AllocHeader* pDestBlock, const void* pInSrc, int InSize)
		{
			void* pDestBlockVoid = pDestBlock;
			Byte* const pDestData = static_cast<Byte*>(pDestBlockVoid) + pDestBlock->Offset;
			memcpy(pDestData, pInSrc, InSize);
			pDestBlock->Offset += InSize;
			return pDestData;
		}
	} // anonymous

	BlockHeap::~BlockHeap()
	{
		AllocHeader* pNextBlock = nullptr;
		for (AllocHeader* pBlock = pHead; pBlock; pBlock = pNextBlock)
		{
			pNextBlock = pBlock->pNext;
			DestroyBlock(pBlock);
		}
	}

	BlockHeap::BlockHeap(const Policy& InPolicy) :
		BlockHeap{InPolicy.DefaultBlockCapacity}
	{
	}

	BlockHeap::BlockHeap(int InBlockCapacity)
	{
		// Note: we may NOT use the AppendBlockOfExactCapacity here (see it for details).
		pHead = pTop = AllocBlock(InBlockCapacity);
	}

	BlockHeap::BlockHeap(BlockHeap&& InOther) :
		pHead(InOther.pHead)
	,	pTop(InOther.pTop)
	{
		InOther.pHead = nullptr;
		InOther.pTop = nullptr;
	}

	BlockHeap& BlockHeap::operator=(BlockHeap&& InOther)
	{
		pHead = InOther.pHead;
		pTop = InOther.pTop;
		InOther.pHead = nullptr;
		InOther.pTop = nullptr;
		return *this;
	}

	void* BlockHeap::StoreBytes(const Policy* pInPolicy, const void* pInSrc, int InSize)
	{
		BOOST_ASSERT(pInPolicy);
		BOOST_ASSERT(pInSrc);
		BOOST_ASSERT_MSG(InSize > 0, "BlockHeap does NOT allow to perform allocations of zero size");
		BOOST_ASSERT(pTop);
		if (pTop->Slack() < InSize)
		{
			ReserveBlock(pInPolicy, InSize);
		}
		return AppendDataToBlock(pTop, pInSrc, InSize);
	}

	AllocHeader *BlockHeap::ReserveBlock(const Policy* pInPolicy, int InDesiredSize)
	{
		int const BlockSize = 
			InDesiredSize <= pInPolicy->DefaultBlockCapacity ? pInPolicy->DefaultBlockCapacity : InDesiredSize;
		return AppendBlockOfExactCapacity(BlockSize);
	}

	/**
	* WARNING!!! Should NOT be used to alloc the first block (because the pHead is NOT yet initialized).
	*/
	AllocHeader* BlockHeap::AppendBlockOfExactCapacity(int InBlockCapacity)
	{
		pTop->pNext = AllocBlock(InBlockCapacity);
		pTop = pTop->pNext;
		return pTop;
	}
} // BlockHeapImpl