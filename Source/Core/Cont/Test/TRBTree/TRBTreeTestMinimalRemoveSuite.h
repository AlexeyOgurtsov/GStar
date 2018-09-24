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
			ComplexRemoveTestSuite,
			*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/RemoveSuite")
		)

		BOOST_AUTO_TEST_CASE(Remove_CountTen_RemoveFour)
	{
		//
		// WARNING!!! Very important: this is particular RARE case of removing!
		//
		constexpr int INITIAL_COUNT = 10;
		constexpr int KEY_TO_REMOVE = 4;

		BOOST_TEST_CHECKPOINT("Making initial");
		IntRBTree T;
		for (int i = 1; i <= INITIAL_COUNT; i++)
		{
			BOOST_REQUIRE(T.Add(i, NoValue{}));
		}

		BOOST_TEST_CHECKPOINT("Removing");
		T.Remove(KEY_TO_REMOVE);

		BOOST_TEST_CHECKPOINT("Checking");
		BOOST_REQUIRE_EQUAL(T.Num(), (INITIAL_COUNT - 1));
		for (int i = 1; i <= INITIAL_COUNT; i++)
		{
			if (i != KEY_TO_REMOVE)
			{
				BOOST_REQUIRE(T.Contains(i));
			}
		}
	}

	BOOST_AUTO_TEST_CASE
	(
		Remove_CountTen_RemoveFive,
		*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/ComplexRemoveTestSuite/Remove_CountTen_RemoveFour")
	)
	{

		//
		// WARNING!!! Very important: this is particular RARE case of removing!
		//
		constexpr int INITIAL_COUNT = 10;
		constexpr int KEY_TO_REMOVE = 5;

		BOOST_TEST_CHECKPOINT("Making initial");
		IntRBTree T;
		for (int i = 1; i <= INITIAL_COUNT; i++)
		{
			BOOST_REQUIRE(T.Add(i, NoValue{}));
		}

		BOOST_TEST_CHECKPOINT("Removing");
		T.Remove(KEY_TO_REMOVE);

		BOOST_TEST_CHECKPOINT("Checking");
		BOOST_REQUIRE_EQUAL(T.Num(), (INITIAL_COUNT - 1));
		for (int i = 1; i <= INITIAL_COUNT; i++)
		{
			if (i != KEY_TO_REMOVE)
			{
				BOOST_REQUIRE(T.Contains(i));
			}
		}
	}

	BOOST_AUTO_TEST_CASE
	(
		Remove_CountTen_RemoveSixth,
		*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/ComplexRemoveTestSuite/Remove_CountTen_RemoveFive")
	)
	{

		//
		// WARNING!!! Very important: this is particular RARE case of removing!
		//
		constexpr int INITIAL_COUNT = 10;
		constexpr int KEY_TO_REMOVE = 6;

		BOOST_TEST_CHECKPOINT("Making initial");
		IntRBTree T;
		for (int i = 1; i <= INITIAL_COUNT; i++)
		{
			BOOST_REQUIRE(T.Add(i, NoValue{}));
		}

		BOOST_TEST_CHECKPOINT("Removing");
		T.Remove(KEY_TO_REMOVE);

		BOOST_TEST_CHECKPOINT("Checking");
		BOOST_REQUIRE_EQUAL(T.Num(), (INITIAL_COUNT - 1));
		for (int i = 1; i <= INITIAL_COUNT; i++)
		{
			if (i != KEY_TO_REMOVE)
			{
				BOOST_REQUIRE(T.Contains(i));
			}
		}
	}

	BOOST_AUTO_TEST_CASE
	(
		Remove_Count30_Remove25,
		*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/ComplexRemoveTestSuite/Remove_CountTen_RemoveSixth")
	)
	{
		//
		// WARNING!!! Very important: this is particular RARE case of removing!
		//
		constexpr int INITIAL_COUNT = 30;
		constexpr int KEY_TO_REMOVE = 25;

		BOOST_TEST_CHECKPOINT("Making initial");
		IntRBTree T;
		for (int i = 1; i <= INITIAL_COUNT; i++)
		{
			BOOST_REQUIRE(T.Add(i, NoValue{}));
		}

		BOOST_TEST_CHECKPOINT("Removing");
		T.Remove(KEY_TO_REMOVE);

		BOOST_TEST_CHECKPOINT("Checking");
		BOOST_REQUIRE_EQUAL(T.Num(), (INITIAL_COUNT - 1));
		for (int i = 1; i <= INITIAL_COUNT; i++)
		{
			if (i != KEY_TO_REMOVE)
			{
				BOOST_REQUIRE(T.Contains(i));
			}
		}
	}

	BOOST_AUTO_TEST_SUITE_END() // ComplexRemoveTestSuite
	BOOST_AUTO_TEST_SUITE_END() // Minimal
	BOOST_AUTO_TEST_SUITE_END() // TRBTreeTestSuite
	BOOST_AUTO_TEST_SUITE_END() // Container
	BOOST_AUTO_TEST_SUITE_END() // Core
} // anonymous