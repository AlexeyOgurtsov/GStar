#pragma once

#include "../BlockHeap.h"

#include <boost/test/included/unit_test.hpp>
#include <cstdio> // strlen
#include <cstdlib> // memcmp
#include <string>

namespace
{

BOOST_AUTO_TEST_SUITE(Core)
BOOST_AUTO_TEST_SUITE(Mem)
BOOST_AUTO_TEST_SUITE(BlockHeapTest)

BOOST_AUTO_TEST_CASE(OneAlloc_SufficientBuffer)
{
	BOOST_TEST_CHECKPOINT("Construction");
	const int TEST_BUFFER_CAPACITY = 8;
	BlockHeap Heap { TEST_BUFFER_CAPACITY };

	BOOST_TEST_CHECKPOINT("Allocation");
	std::string const AllocStr { "test" };
	void* pAllocatedTestStr = Heap.StoreBytes(AllocStr.data(), AllocStr.size());

	BOOST_REQUIRE_EQUAL(0, memcmp(pAllocatedTestStr, AllocStr.data(), AllocStr.size()));
}

BOOST_AUTO_TEST_CASE(ManyAllocs_InsufficientBuffer)
{
	BOOST_TEST_CHECKPOINT("Construction");
	const int TEST_BUFFER_CAPACITY = 8;
	BlockHeap Heap{ TEST_BUFFER_CAPACITY };

	BOOST_TEST_CHECKPOINT("Allocation");
	std::string const AllocStr{ "test" };
	void* pAllocatedTestStr = Heap.StoreBytes(AllocStr.data(), AllocStr.size());

	BOOST_TEST_CHECKPOINT("Allocation_second");
	std::string const AllocStr_second { "test_second" };
	void* pAllocatedTestStr_second = Heap.StoreBytes(AllocStr_second.data(), AllocStr_second.size());

	BOOST_REQUIRE_EQUAL(0, memcmp(pAllocatedTestStr, AllocStr.data(), AllocStr.size()));
	BOOST_REQUIRE_EQUAL(0, memcmp(pAllocatedTestStr_second, AllocStr_second.data(), AllocStr_second.size()));
}

BOOST_AUTO_TEST_SUITE_END() // BlockHeapTest 
BOOST_AUTO_TEST_SUITE_END() // Mem
BOOST_AUTO_TEST_SUITE_END() // Core

} // anonymous