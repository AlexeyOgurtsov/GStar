#pragma once

#include <boost/test/included/unit_test.hpp>
#include "Core/Cont/TSortedSet.h"

BOOST_AUTO_TEST_SUITE(Core)
BOOST_AUTO_TEST_SUITE(Container)
BOOST_AUTO_TEST_SUITE(TSortedSetTestSuite)

BOOST_AUTO_TEST_CASE(MainTest)
{
	BOOST_TEST_CHECKPOINT("Construction");
	TSortedSet<int> S;
}

BOOST_AUTO_TEST_SUITE_END() // TSortedSetTestSuite
BOOST_AUTO_TEST_SUITE_END() // Container
BOOST_AUTO_TEST_SUITE_END() // Core