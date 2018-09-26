#pragma once

#include "TRBTreeTestUtils.h"

namespace
{
	BOOST_AUTO_TEST_SUITE(Core)
	BOOST_AUTO_TEST_SUITE(Container)
	BOOST_AUTO_TEST_SUITE(TRBTreeTestSuite)
	BOOST_AUTO_TEST_SUITE
	(
		MovingTestSuite,
		*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal")
	)

	BOOST_AUTO_TEST_CASE(AppendMovedKeyValueTest)
	{
		BOOST_TEST_CHECKPOINT("Initialization");
		const int KEY_CONST = 3;
		MoveOnlyTree T;
		BOOST_REQUIRE(T.Add(MoveOnlyTree::KeyValueType{ 1, std::make_unique<std::string>("VALUE") }));
		BOOST_REQUIRE(T.Add(2, std::make_unique<std::string>("VALUE")));
		BOOST_REQUIRE(T.Add(KEY_CONST, std::make_unique<std::string>("VALUE")));
		BOOST_REQUIRE_EQUAL(3, T.Num());
	}

	BOOST_AUTO_TEST_CASE(AddKeyValueTest_NoMovable)
	{
		BOOST_TEST_CHECKPOINT("Initialization");
		const int KEY_CONST = 3;
		const NoValue VALUE_CONST;

		IntRBTree T;
		IntRBTree::KeyValueType const KV{ 1, NoValue{} };
		BOOST_REQUIRE(T.Add(KV));
		BOOST_REQUIRE(T.Add(2, VALUE_CONST));
		BOOST_REQUIRE(T.Add(KEY_CONST, VALUE_CONST));
		BOOST_REQUIRE_EQUAL(3, T.Num());
	}

	BOOST_AUTO_TEST_SUITE_END() // MovingTestSuite
	BOOST_AUTO_TEST_SUITE_END() // Container
	BOOST_AUTO_TEST_SUITE_END() // TRBTreeTestSuite
	BOOST_AUTO_TEST_SUITE_END() // Core
}