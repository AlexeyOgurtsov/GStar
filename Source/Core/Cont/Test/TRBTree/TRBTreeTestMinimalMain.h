#pragma once

#include "TRBTreeTestUtils.h"

namespace
{
	BOOST_AUTO_TEST_SUITE(Core)
	BOOST_AUTO_TEST_SUITE(Container)
	BOOST_AUTO_TEST_SUITE(TRBTreeTestSuite)
	BOOST_AUTO_TEST_SUITE(Minimal)
	
		/**
		* Tests a minimal set of operations, to make it possible to write other tests dependent on it.
		* - Construction test;
		* - Num, Empty tests;
		* - Contains minimal test;
		* - Add minimal test;
		*/
		BOOST_AUTO_TEST_CASE(FirstMinimal)
	{
		//
		// WARNING!!! This keys are ordered (see test).
		//
		const int KEY_ONE = 1;
		const int KEY_TWO = 2;
		const int KEY_ZERO = 0;

		BOOST_TEST_CHECKPOINT("Construction");
		IntRBTree T;
		BOOST_REQUIRE_EQUAL(0, T.Num());
		BOOST_REQUIRE(T.Empty());

		BOOST_TEST_CHECKPOINT("Find on empty");
		BOOST_REQUIRE(!T.Contains(KEY_ONE));
		BOOST_REQUIRE(!T.Find(KEY_ONE));
		BOOST_REQUIRE(!T.FindValue(KEY_ONE));

		BOOST_TEST_CHECKPOINT("Add");
		BOOST_REQUIRE(T.Add(KEY_ONE, NoValue{}));
		BOOST_REQUIRE_EQUAL(1, T.Num());
		BOOST_REQUIRE(!T.Empty());
		BOOST_REQUIRE(T.Contains(KEY_ONE));
		BOOST_REQUIRE(!T.Contains(KEY_TWO));
		BOOST_REQUIRE(!T.Contains(KEY_ZERO));
		BOOST_REQUIRE(T.Find(KEY_ONE));
		BOOST_REQUIRE_EQUAL(KEY_ONE, T.Find(KEY_ONE)->Key);

		BOOST_TEST_CHECKPOINT("Add Already Included");
		BOOST_REQUIRE(!T.Add(KEY_ONE, NoValue{}));
		BOOST_REQUIRE_EQUAL(1, T.Num());

		BOOST_TEST_CHECKPOINT("Add another (on the right of the root)");
		BOOST_REQUIRE(T.Add(KEY_TWO, NoValue{}));
		BOOST_REQUIRE(T.Contains(KEY_TWO));
		BOOST_REQUIRE(T.Contains(KEY_ONE));
		BOOST_REQUIRE_EQUAL(KEY_TWO, T.Find(KEY_TWO)->Key);
		BOOST_REQUIRE_EQUAL(KEY_ONE, T.Find(KEY_ONE)->Key);
		BOOST_REQUIRE_EQUAL(2, T.Num());

		BOOST_TEST_CHECKPOINT("Add another (on the left of the root)");
		BOOST_REQUIRE(T.Add(KEY_ZERO, NoValue{}));
		BOOST_REQUIRE(T.Contains(KEY_ZERO));
		BOOST_REQUIRE(T.Contains(KEY_TWO));
		BOOST_REQUIRE(T.Contains(KEY_ONE));
		BOOST_REQUIRE_EQUAL(KEY_TWO, T.Find(KEY_TWO)->Key);
		BOOST_REQUIRE_EQUAL(KEY_ONE, T.Find(KEY_ONE)->Key);
		BOOST_REQUIRE_EQUAL(KEY_ZERO, T.Find(KEY_ZERO)->Key);
		BOOST_REQUIRE_EQUAL(3, T.Num());
	}

	BOOST_AUTO_TEST_CASE
	(
		MinMax,
		*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/FirstMinimal")
	)
	{
		//
		// WARNING!!! This keys are ordered (see test).
		//
		const int KEY_ONE = 1;
		const int KEY_TWO = 2;
		const int KEY_THREE = 3;
		const int KEY_FOUR = 4;
		const int KEY_FIVE = 5;
		const int KEY_SIX = 6;
		const int KEY_SEVEN = 7;

		BOOST_TEST_CHECKPOINT("Construction");
		IntRBTree T;

		BOOST_TEST_CHECKPOINT("Add");
		BOOST_REQUIRE(T.Add(KEY_FIVE, NoValue{}));
		BOOST_REQUIRE(T.Add(KEY_THREE, NoValue{}));
		BOOST_REQUIRE(T.Add(KEY_SEVEN, NoValue{}));
		BOOST_REQUIRE(T.Add(KEY_SIX, NoValue{}));
		BOOST_REQUIRE(T.Add(KEY_TWO, NoValue{}));
		BOOST_REQUIRE(T.Add(KEY_ONE, NoValue{}));
		BOOST_REQUIRE(T.Add(KEY_FOUR, NoValue{}));
		BOOST_REQUIRE_EQUAL(T.Num(), KEY_SEVEN);

		BOOST_TEST_CHECKPOINT("MinMax");
		BOOST_REQUIRE_EQUAL(KEY_ONE, T.Min().Key);
		BOOST_REQUIRE_EQUAL(KEY_SEVEN, T.Max().Key);
	}
		
	BOOST_AUTO_TEST_SUITE_END() // Minimal	
	BOOST_AUTO_TEST_SUITE_END() // TRBTreeTestSuite
	BOOST_AUTO_TEST_SUITE_END() // Container
	BOOST_AUTO_TEST_SUITE_END() // Core
} // anonymous