#pragma once

#include "TRBTreeTestUtils.h"

namespace
{
	BOOST_AUTO_TEST_SUITE(Core)
	BOOST_AUTO_TEST_SUITE(Container)
	BOOST_AUTO_TEST_SUITE(TRBTreeTestSuite)
	BOOST_AUTO_TEST_SUITE(Minimal)

		BOOST_AUTO_TEST_SUITE
		(
			FindSuite,
			*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/AddSuite/AddComplex")
		)

		BOOST_AUTO_TEST_CASE(Corner_FindRoot_InRootOnlyTree)
	{
		constexpr int NUM = 1;
		constexpr int KEY_ONE = 1;

		constexpr int KEY_TO_FIND = KEY_ONE;

		BOOST_TEST_CHECKPOINT("Construction");
		IntRBTree T;
		BOOST_REQUIRE(T.Add(KEY_ONE, NoValue{}));

		BOOST_TEST_CHECKPOINT("Find");
		const IntRBTree::KeyValueType* const pFound = T.Find(KEY_TO_FIND);
		BOOST_REQUIRE(pFound);
		BOOST_REQUIRE_EQUAL(KEY_TO_FIND, pFound->Key);
	}

	BOOST_AUTO_TEST_CASE(Corner_FindRoot)
	{
		constexpr int NUM = 2;
		constexpr int KEY_ONE = 1;
		constexpr int KEY_TWO = 2;

		constexpr int KEY_TO_FIND = KEY_ONE;

		BOOST_TEST_CHECKPOINT("Construction");
		IntRBTree T;
		BOOST_REQUIRE(T.Add(KEY_ONE, NoValue{}));
		BOOST_REQUIRE(T.Add(KEY_TWO, NoValue{}));

		BOOST_TEST_CHECKPOINT("Find");
		const IntRBTree::KeyValueType* const pFound = T.Find(KEY_TO_FIND);
		BOOST_REQUIRE(pFound);
		BOOST_REQUIRE_EQUAL(KEY_TO_FIND, pFound->Key);
	}

	BOOST_AUTO_TEST_CASE(Corner_FindRoot_Inexistent)
	{
		constexpr int NUM = 2;
		constexpr int KEY_ONE = 1;
		constexpr int KEY_TWO = 2;

		constexpr int KEY_TO_FIND = 5;

		BOOST_TEST_CHECKPOINT("Construction");
		IntRBTree T;
		BOOST_REQUIRE(T.Add(KEY_ONE, NoValue{}));
		BOOST_REQUIRE(T.Add(KEY_TWO, NoValue{}));

		BOOST_TEST_CHECKPOINT("Find");
		const IntRBTree::KeyValueType* const pFound = T.Find(KEY_TO_FIND);
		BOOST_REQUIRE(!pFound);
	}

	BOOST_AUTO_TEST_CASE(Corner_FindRoot_InRootOnlyTree_Inexistent)
	{
		constexpr int NUM = 1;
		constexpr int KEY_ONE = 1;

		constexpr int KEY_TO_FIND = 5;

		BOOST_TEST_CHECKPOINT("Construction");
		IntRBTree T;
		BOOST_REQUIRE(T.Add(KEY_ONE, NoValue{}));

		BOOST_TEST_CHECKPOINT("Find");
		const IntRBTree::KeyValueType* const pFound = T.Find(KEY_TO_FIND);
		BOOST_REQUIRE(!pFound);
	}

	BOOST_AUTO_TEST_CASE(FindDeepest)
	{
		constexpr int KEY_TO_FIND = 3;

		constexpr int NUM = 9;

		constexpr int KEY_ONE = 1;
		constexpr int KEY_TWO = 2;
		constexpr int KEY_THREE = 3;
		constexpr int KEY_FOUR = 4;
		constexpr int KEY_FIVE = 5;
		constexpr int KEY_SIX = 6;
		constexpr int KEY_SEVEN = 7;
		constexpr int KEY_EIGHT = 8;
		constexpr int KEY_NINE = 9;

		BOOST_TEST_CHECKPOINT("Construction");
		IntRBTree T;
		BOOST_REQUIRE(T.Add(KEY_SIX, NoValue{}));
		BOOST_REQUIRE(T.Add(KEY_EIGHT, NoValue{}));
		BOOST_REQUIRE(T.Add(KEY_FOUR, NoValue{}));
		BOOST_REQUIRE(T.Add(KEY_SEVEN, NoValue{}));
		BOOST_REQUIRE(T.Add(KEY_NINE, NoValue{}));
		BOOST_REQUIRE(T.Add(KEY_TWO, NoValue{}));
		BOOST_REQUIRE(T.Add(KEY_THREE, NoValue{}));
		BOOST_REQUIRE(T.Add(KEY_FIVE, NoValue{}));
		BOOST_REQUIRE(T.Add(KEY_ONE, NoValue{}));

		BOOST_TEST_CHECKPOINT("Find");
		const IntRBTree::KeyValueType* pFound = T.Find(KEY_TO_FIND);
		BOOST_REQUIRE(pFound);
		BOOST_REQUIRE_EQUAL(KEY_TO_FIND, pFound->Key);
	}

	BOOST_AUTO_TEST_CASE(FindDeepestOne)
	{
		constexpr int KEY_TO_FIND = 7;

		constexpr int NUM = 9;

		constexpr int KEY_ONE = 1;
		constexpr int KEY_TWO = 2;
		constexpr int KEY_THREE = 3;
		constexpr int KEY_FOUR = 4;
		constexpr int KEY_FIVE = 5;
		constexpr int KEY_SIX = 6;
		constexpr int KEY_SEVEN = 7;
		constexpr int KEY_EIGHT = 8;
		constexpr int KEY_NINE = 9;

		BOOST_TEST_CHECKPOINT("Construction");
		IntRBTree T;
		BOOST_REQUIRE(T.Add(KEY_SIX, NoValue{}));
		BOOST_REQUIRE(T.Add(KEY_EIGHT, NoValue{}));
		BOOST_REQUIRE(T.Add(KEY_FOUR, NoValue{}));
		BOOST_REQUIRE(T.Add(KEY_SEVEN, NoValue{}));
		BOOST_REQUIRE(T.Add(KEY_NINE, NoValue{}));
		BOOST_REQUIRE(T.Add(KEY_TWO, NoValue{}));
		BOOST_REQUIRE(T.Add(KEY_THREE, NoValue{}));
		BOOST_REQUIRE(T.Add(KEY_FIVE, NoValue{}));
		BOOST_REQUIRE(T.Add(KEY_ONE, NoValue{}));

		BOOST_TEST_CHECKPOINT("Find");
		const IntRBTree::KeyValueType* pFound = T.Find(KEY_TO_FIND);
		BOOST_REQUIRE(pFound);
		BOOST_REQUIRE_EQUAL(KEY_TO_FIND, pFound->Key);
	}

	BOOST_AUTO_TEST_CASE(FindDeepestInexistent)
	{
		constexpr int KEY_TO_FIND = 10;

		constexpr int NUM = 9;

		constexpr int KEY_ONE = 1;
		constexpr int KEY_TWO = 2;
		constexpr int KEY_THREE = 3;
		constexpr int KEY_FOUR = 4;
		constexpr int KEY_FIVE = 5;
		constexpr int KEY_SIX = 6;
		constexpr int KEY_SEVEN = 7;
		constexpr int KEY_EIGHT = 8;
		constexpr int KEY_NINE = 9;

		BOOST_TEST_CHECKPOINT("Construction");
		IntRBTree T;
		BOOST_REQUIRE(T.Add(KEY_SIX, NoValue{}));
		BOOST_REQUIRE(T.Add(KEY_EIGHT, NoValue{}));
		BOOST_REQUIRE(T.Add(KEY_FOUR, NoValue{}));
		BOOST_REQUIRE(T.Add(KEY_SEVEN, NoValue{}));
		BOOST_REQUIRE(T.Add(KEY_NINE, NoValue{}));
		BOOST_REQUIRE(T.Add(KEY_TWO, NoValue{}));
		BOOST_REQUIRE(T.Add(KEY_THREE, NoValue{}));
		BOOST_REQUIRE(T.Add(KEY_FIVE, NoValue{}));
		BOOST_REQUIRE(T.Add(KEY_ONE, NoValue{}));

		BOOST_TEST_CHECKPOINT("Find");
		const IntRBTree::KeyValueType* pFound = T.Find(KEY_TO_FIND);
		BOOST_REQUIRE(!pFound);
	}

	BOOST_AUTO_TEST_SUITE_END() // FindSuite
	BOOST_AUTO_TEST_SUITE_END() // Minimal	
	BOOST_AUTO_TEST_SUITE_END() // TRBTreeTestSuite
	BOOST_AUTO_TEST_SUITE_END() // Container
	BOOST_AUTO_TEST_SUITE_END() // Core
} // anonymous