#pragma once

#include "TVectorTestUtils.h"

namespace
{

BOOST_AUTO_TEST_SUITE(Core)
BOOST_AUTO_TEST_SUITE(Container)
BOOST_AUTO_TEST_SUITE(TVectorTestSuite)

BOOST_AUTO_TEST_CASE(SimplePod_SmallEqualityTest, *boost::unit_test::depends_on("Core/Container/TVectorTestSuite/SimplePod_SmallMainTest"))
{
	TVector<int> V_left;
	TVector<int> V_right;
	BOOST_TEST_CHECKPOINT("EqualityTest_Empty");
	BOOST_REQUIRE(V_left == V_right);

	BOOST_TEST_CHECKPOINT("NonEqualityTest_1_and_empty");
	V_left.Add(1);
	BOOST_REQUIRE(V_left != V_right);
	BOOST_REQUIRE(V_right != V_left);

	BOOST_TEST_CHECKPOINT("EqualityTest_1_and_1");
	V_right.Add(1);
	BOOST_REQUIRE(V_left == V_right);
	BOOST_REQUIRE(V_right == V_left);

	BOOST_TEST_CHECKPOINT("NonEqualityTest_1_and_1_2");
	V_right.Add(2);
	BOOST_REQUIRE(V_left != V_right);
	BOOST_REQUIRE(V_right != V_left);

	BOOST_TEST_CHECKPOINT("EqualityTest_1_2_and_1_2");
	V_left.Add(2);
	BOOST_REQUIRE(V_left == V_right);
	BOOST_REQUIRE(V_right == V_left);
}

BOOST_AUTO_TEST_CASE(SimplePod_ForEach, *boost::unit_test::depends_on("Core/Container/TVectorTestSuite/SimplePod_SmallMainTest"))
{
	BOOST_TEST_CHECKPOINT("ForEach const empty");
	const TVector<int> V_empty;
	int numItems_empty = 0;
	for (const auto& elem : V_empty) { numItems_empty++; }
	BOOST_REQUIRE_EQUAL(numItems_empty, 0);

	BOOST_TEST_CHECKPOINT("ForEach Non-empty");
	TVector<int> V{ 1, 2, 3 };
	int numItems = 0;
	for (auto& elem : V) { numItems++; }
	BOOST_REQUIRE_EQUAL(numItems, 3);
}


BOOST_AUTO_TEST_SUITE_END() // TVectorTestSuite
BOOST_AUTO_TEST_SUITE_END() // Container
BOOST_AUTO_TEST_SUITE_END() // Core

} 