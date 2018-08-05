#pragma once

#include <boost/test/included/unit_test.hpp>
#include "Core/Cont/TRBTree.h"

namespace
{

using IntRBTree = TRBTree<KVType<int, NoValue>>;


BOOST_AUTO_TEST_SUITE(Core)
BOOST_AUTO_TEST_SUITE(Container)
BOOST_AUTO_TEST_SUITE(TRBTreeTestSuite)

/**
* Tests a minimal set of operations, to make it possible to write other tests dependent on it.
* - Construction test;
* - Num, Empty tests;
* - First, Last tests;
* - Contains minimal test;
* - Add minimal test;
*/
BOOST_AUTO_TEST_CASE(FirstMinimal)
{
	const int KEY_ONE = 1;

	BOOST_TEST_CHECKPOINT("Construction");
	IntRBTree T;
	BOOST_REQUIRE_EQUAL(0, T.Num());
	BOOST_REQUIRE(T.Empty());
	BOOST_REQUIRE( ! T.Contains(KEY_ONE) );
}

BOOST_AUTO_TEST_CASE(CopyUnorderedToTest, *boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/FirstMinimal"))
{
	constexpr int NUM = 5;
	IntRBTree::KeyValueType DestBuf[NUM];

	BOOST_TEST_CHECKPOINT("Construction");
	IntRBTree T;

	BOOST_TEST_CHECKPOINT("Copying to buffer");
	T.CopyUnorderedTo(DestBuf);
}

BOOST_AUTO_TEST_SUITE_END() // TRBTree
BOOST_AUTO_TEST_SUITE_END() // Container
BOOST_AUTO_TEST_SUITE_END() // Core

} // anonymous