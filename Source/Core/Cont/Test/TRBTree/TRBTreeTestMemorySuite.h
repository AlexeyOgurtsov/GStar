#pragma once

#include "TRBTreeTestUtils.h"

namespace
{
	BOOST_AUTO_TEST_SUITE(Core)
	BOOST_AUTO_TEST_SUITE(Container)
	BOOST_AUTO_TEST_SUITE(TRBTreeTestSuite)

	BOOST_AUTO_TEST_SUITE
	(
		MemoryTestSuite,
		*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal")
		*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/MovingTestSuite")
	)

	BOOST_AUTO_TEST_CASE(ReserveGrowTest)
	{
		constexpr int INITIAL_COUNT = 7;

		MoveOnlyTree T;
		T.Add(MoveOnlyTree::KeyValueType{ 5, std::make_unique<std::string>("five") });
		T.Add(MoveOnlyTree::KeyValueType{ 1, std::make_unique<std::string>("one") });
		T.Add(MoveOnlyTree::KeyValueType{ 7, std::make_unique<std::string>("seven") });
		T.Add(MoveOnlyTree::KeyValueType{ 3, std::make_unique<std::string>("three") });
		T.Add(MoveOnlyTree::KeyValueType{ 2, std::make_unique<std::string>("two") });
		T.Add(MoveOnlyTree::KeyValueType{ 4, std::make_unique<std::string>("four") });
		T.Add(MoveOnlyTree::KeyValueType{ 6, std::make_unique<std::string>("six") });
		BOOST_REQUIRE_EQUAL(T.Num(), INITIAL_COUNT);

		T.ReserveGrow(INITIAL_COUNT * 10);

		BOOST_REQUIRE_EQUAL(T.Num(), INITIAL_COUNT);
		for (int i = 1; i <= INITIAL_COUNT; i++)
		{
			BOOST_REQUIRE(T.Contains(i));
		}
	}

	BOOST_AUTO_TEST_CASE
	(
		ShrinkToFitTest,
		*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/MemoryTestSuite/ReserveGrowTest")
	)
	{
		constexpr int INITIAL_COUNT = 50;

		MoveOnlyTree T;
		for (int i = 1; i <= INITIAL_COUNT; i++)
		{
			char buf[64];
			snprintf(buf, 64, "%d", i);
			T.Add(MoveOnlyTree::KeyValueType{ i, std::make_unique<std::string>(buf) });
		}

		T.ShrinkToFit();

		BOOST_REQUIRE_EQUAL(T.Num(), INITIAL_COUNT);
		for (int i = 1; i <= INITIAL_COUNT; i++)
		{
			BOOST_REQUIRE(T.Contains(i));
		}

		int removeKey = 1;
		while (true)
		{
			if (!T.Remove(removeKey))
			{
				break;
			}
			removeKey += 3;
		}

		BOOST_REQUIRE_EQUAL(T.Num(), INITIAL_COUNT);
		for (int i = 1; i <= INITIAL_COUNT; i++)
		{
			BOOST_REQUIRE(T.Contains(i));
		}
	}

	BOOST_AUTO_TEST_SUITE_END() // MemoryTestSuite

	BOOST_AUTO_TEST_SUITE_END() // TRBTreeTestSuite
	BOOST_AUTO_TEST_SUITE_END() // Container
	BOOST_AUTO_TEST_SUITE_END() // Core
} // anonymous