#include "Core/Mem/Helper/LinearAllocHelper.h"
#include <cstdlib> // malloc/realloc/free
#include <cstring> // memcpy

LinearAllocHelper::LinearAllocHelper(int InCapacity) :
	Offset(0)
,	MaxLength(InCapacity)
{
	Ptr = static_cast<Byte*>(malloc(MaxLength));
}

void LinearAllocHelper::AppendBytes(const void* pInSrc, int InSize)
{
	BOOST_ASSERT(pInSrc);
	if(Slack() < InSize)
	{
		GrowBuffer(InSize);
	}
	memcpy(static_cast<Byte*>(Ptr) + Offset, pInSrc, InSize);
}

void* LinearAllocHelper::CommitAlloc(bool bShrinkToFit)
{
	BOOST_ASSERT(Ptr);
	BOOST_ASSERT_MSG(Offset > 0, "LinearAllocHelper::CommitAlloc: unable to commit alloc: nothing is allocated");

	if(bShrinkToFit && Slack() > 0)
	{
		ReallocBuffer(Offset);
	}

	// Prevent from using the alloc helper again
	Ptr = nullptr;
	
	return Ptr;
}

void LinearAllocHelper::DeleteAlloc(void* pInPtr)
{
	free(pInPtr);
}

/**
* Grows buffer and sets extra data. 
* Moves old data properly. 
*/
void LinearAllocHelper::GrowBuffer(int InNewSize)
{
	int const NewCapacity = 2 * InNewSize;
	ReallocBuffer(NewCapacity); 
}

/**
* Reallocs buffer with exact capacity.
* Moves old data properly.
*/
void LinearAllocHelper::ReallocBuffer(int InNewSize)
{
	Ptr = static_cast<Byte*>(realloc(Ptr, InNewSize)); 
}