#pragma once

#include "../LinearAllocHelper.h"

#include <boost/test/included/unit_test.hpp>
#include <cstdio> // strlen
#include <cstdlib> // memcmp
#include <string>

namespace
{

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
	int const FIRST_ALLOC_LEN = strlen(FIRST_ALLOC);
	LinearAlloc.AppendBytes(FIRST_ALLOC, FIRST_ALLOC_LEN);
	
	int const REFERENCE_LENGTH = FIRST_ALLOC_LEN;

	BOOST_TEST_CHECKPOINT("Commit");
	void* const pAlloc = LinearAlloc.CommitAlloc();
	BOOST_REQUIRE(pAlloc);
	BOOST_REQUIRE_EQUAL(0, memcmp(static_cast<const char*>(pAlloc), FIRST_ALLOC, REFERENCE_LENGTH));

	BOOST_TEST_CHECKPOINT("DeleteAlloc");
	LinearAllocHelper::DeleteAlloc(pAlloc);
}

BOOST_AUTO_TEST_CASE(ManyAllocs_BufferLow)
{
	BOOST_TEST_CHECKPOINT("Construction");
	const int TEST_BUFFER_CAPACITY = 8;
	LinearAllocHelper LinearAlloc(TEST_BUFFER_CAPACITY);

	BOOST_TEST_CHECKPOINT("FirstAlloc");
	const char* FIRST_ALLOC = "test";
	int const FIRST_ALLOC_LEN = strlen(FIRST_ALLOC);
	LinearAlloc.AppendBytes(FIRST_ALLOC, FIRST_ALLOC_LEN);

	BOOST_TEST_CHECKPOINT("SecondAlloc");
	const char* SECOND_ALLOC = "Second";
	int const SECOND_ALLOC_LEN = strlen(SECOND_ALLOC);
	LinearAlloc.AppendBytes(SECOND_ALLOC, SECOND_ALLOC_LEN);

	std::string const REFERENCE_STRING = std::string(FIRST_ALLOC) + std::string(SECOND_ALLOC);
	int const REFERENCE_STRING_LEN = REFERENCE_STRING.size();

	
	int const REFERENCE_LENGTH = FIRST_ALLOC_LEN;

	BOOST_TEST_CHECKPOINT("Commit");
	void* const pAlloc = LinearAlloc.CommitAlloc();
	BOOST_REQUIRE(pAlloc);
	BOOST_REQUIRE_EQUAL(0, memcmp(static_cast<const char*>(pAlloc), REFERENCE_STRING.c_str(), REFERENCE_STRING_LEN));

	BOOST_TEST_CHECKPOINT("DeleteAlloc");
	LinearAllocHelper::DeleteAlloc(pAlloc);

}

BOOST_AUTO_TEST_SUITE_END() // LinearAllocHelperTest 

BOOST_AUTO_TEST_SUITE_END() // Mem
BOOST_AUTO_TEST_SUITE_END() // Core

} // anonymous
