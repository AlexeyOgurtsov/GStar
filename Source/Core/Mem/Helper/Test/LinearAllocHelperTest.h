#pragma once

#include "../LinearAllocHelper.h"

#include <boost/test/included/unit_test.hpp>
#include <cstdio>

BOOST_AUTO_TEST_SUITE(Core)
BOOST_AUTO_TEST_SUITE(Mem)

BOOST_AUTO_TEST_SUITE(LinearAllocHelperTest)

BOOST_AUTO_TEST_CASE(OneAlloc_SufficientBuffer)
{
	BOOST_TEST_CHECKPOINT("Construction");
	const int TEST_BUFFER_CAPACITY = 8;
	LinearAllocHelper LinearAlloc(TEST_BUFFER_CAPACITY);
	
	BOOST_TEST_CHECKPOINT("FirstAlloc");
	const char* FIRST_ALLOC = "test";
	LinearAlloc.AppendBytes(FIRST_ALLOC, strlen(FIRST_ALLOC));

	BOOST_TEST_CHECKPOINT("Commit");
	void* const pAlloc = LinearAlloc.CommitAlloc();
	BOOST_REQUIRE(pAlloc);
	BOOST_REQUIRE_EQUAL(0, strcmp(static_cast<const char*>(pAlloc), FIRST_ALLOC));

	BOOST_TEST_CHECKPOINT("DeleteAlloc");
	LinearAllocHelper::DeleteAlloc(pAlloc);
}

BOOST_AUTO_TEST_SUITE_END() // LinearAllocHelperTest 

BOOST_AUTO_TEST_SUITE_END() // Mem
BOOST_AUTO_TEST_SUITE_END() // Core



