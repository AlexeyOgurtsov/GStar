#pragma once

#include "Core/CoreSysMinimal.h"

/**
* TO-BE-TESTED.
*
* Provide a way to notify about the bad allocation.
*/

/**
* Allocates the objects lineraly in the memory in the order they were added.
* The allocation is guaranteed to be performed only at the point of calling CommitAlloc().
*
* Only one allocation (i.e. the CommitAlloc call) may be performed with the same instance.
*/
class LinearAllocHelper
{
public:
	/**
	* Capacity to be used initially if explicit capacity is not provided as argument.
	*/
	constexpr static int DEFAULT_CAPACITY = 64;	

	/**
	* Initializes with the given capacity.
	*/
	explicit LinearAllocHelper(int InCapacity = DEFAULT_CAPACITY);

	/**
	* Appends the given bytes to the end of the allocation.
	*/
	void AppendBytes(const Byte* pInSrc, int InSize);

	/*
	* Performs the allocation and returns pointer to the allocated object.
	*
	* WARNING!!! Never delete with the ordinary C++ delete, use the classes's DeleteAlloc!!!
	*
	* @arg bShrinkToFit: if possible, forces the new alloc to have an exact size, releasing the old.
	*/
	void* CommitAlloc(bool bShrinkToFit = false);

	/**
	* Deletes bytes allocated with the class.
	*/
	static void DeleteAlloc(void* pInPtr);

private:
	__forceinline int Slack() const { return MaxLength - Offset; }
	void GrowBuffer(int InNewSize);
	void ReallocBuffer(int InNewSize);

	/**
	* Pointer to the start of the allocated buffer.
	*/
	Byte* Ptr;
	int Offset;
	int MaxLength;
};