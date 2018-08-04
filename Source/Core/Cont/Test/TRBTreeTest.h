#pragma once

#include <boost/test/included/unit_test.hpp>
#include "Core/Cont/TRBTree.h"

namespace
{

BOOST_AUTO_TEST_SUITE(Core)
BOOST_AUTO_TEST_SUITE(Container)
BOOST_AUTO_TEST_SUITE(TRBTreeTestSuite)

BOOST_AUTO_TEST_CASE(MainTest)
{
	using IntRBTree = TRBTree<KVType<int, NoValue>>;

	BOOST_TEST_CHECKPOINT("Construction");
	IntRBTree T;
	BOOST_REQUIRE_EQUAL(0, T.Num());
	BOOST_REQUIRE(T.Empty());
}

BOOST_AUTO_TEST_SUITE_END() // TRBTree
BOOST_AUTO_TEST_SUITE_END() // Container
BOOST_AUTO_TEST_SUITE_END() // Core

} // anonymous