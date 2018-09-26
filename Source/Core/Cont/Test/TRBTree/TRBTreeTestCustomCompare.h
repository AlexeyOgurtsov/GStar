#pragma once

#include "TRBTreeTestUtils.h"

namespace
{
	BOOST_AUTO_TEST_SUITE(Core)
	BOOST_AUTO_TEST_SUITE(Container)
	BOOST_AUTO_TEST_SUITE(TRBTreeTestSuite)
	BOOST_AUTO_TEST_SUITE
	(
		CustomCompare,
		*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal")
	)
	BOOST_AUTO_TEST_CASE
	(
		CompareArgTest,
		*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/CustomCompare/CompareIntWithPointerToInt")
	)
	{

		using CustomIntPtrRBTree = TRBTree<KVType<const int*, NoValue>, TestByValueComparer>;

		const int KEY_ONE = 1;
		const int* const PTR_TO_KEY_ONE = &KEY_ONE;

		BOOST_TEST_CHECKPOINT("Construction");
		CustomIntPtrRBTree T;

		BOOST_TEST_CHECKPOINT("Add");
		BOOST_REQUIRE(T.Add(PTR_TO_KEY_ONE, NoValue{}));

		BOOST_TEST_CHECKPOINT("Find");
		const CustomIntPtrRBTree::KeyValueType* const pFound = T.Find(PTR_TO_KEY_ONE);

		BOOST_REQUIRE(pFound);
		BOOST_REQUIRE_EQUAL(pFound->Key, PTR_TO_KEY_ONE);
	}

	BOOST_AUTO_TEST_CASE(CompareIntWithPointerToInt)
	{
		const int KEY_ONE = 1;
		const int* const PTR_TO_KEY_ONE = &KEY_ONE;

		BOOST_TEST_CHECKPOINT("Construction");
		IntRBTree T;

		BOOST_TEST_CHECKPOINT("Add");
		BOOST_REQUIRE(T.Add(KEY_ONE, NoValue{}));

		BOOST_TEST_CHECKPOINT("Find");
		const IntRBTree::KeyValueType* const pFound = T.Find(PTR_TO_KEY_ONE, IntWithIntPointerComparer());
		BOOST_REQUIRE(pFound);
		BOOST_REQUIRE_EQUAL(pFound->Key, KEY_ONE);
	}

	BOOST_AUTO_TEST_SUITE_END() // CustomCompare
	BOOST_AUTO_TEST_SUITE_END() // TRBTreeTestSuite
	BOOST_AUTO_TEST_SUITE_END() // Container
	BOOST_AUTO_TEST_SUITE_END() // Core
} // anonymous