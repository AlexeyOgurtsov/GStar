#pragma once

#include "Core/Math/Vector.h"
#include <boost/test/included/unit_test.hpp>

namespace 
{
	BOOST_AUTO_TEST_SUITE(CoreSuite)

	BOOST_AUTO_TEST_SUITE(MathSuite)

	BOOST_AUTO_TEST_SUITE(BLASSuite)
	
	BOOST_AUTO_TEST_SUITE(MathVectorSuite)

	BOOST_AUTO_TEST_SUITE(MinimalSuite)

	BOOST_AUTO_TEST_CASE(VectorConstructionTest)
	{
		Vector DefaultConstructedV;
		Vector V { 1.F, 2.F, 3.F };
		BOOST_REQUIRE_EQUAL( V.X, 1.F );
		BOOST_REQUIRE_EQUAL( V.Y, 2.F );
		BOOST_REQUIRE_EQUAL( V.Z, 3.F );
	}

	BOOST_AUTO_TEST_CASE(CompareTest)
	{
		Vector const V_122 { 1.F, 2.F, 2.F };
		Vector const V_123 { 1.F, 2.F, 3.F };
		Vector const V_124 { 1.F, 2.F, 4.F };

		BOOST_REQUIRE_EQUAL(V_123, V_123);
		BOOST_REQUIRE(V_123 != V_122);
		BOOST_REQUIRE(V_123 != V_124);

		BOOST_REQUIRE(V_122 < V_123);
		BOOST_REQUIRE(V_123 > V_122);

		BOOST_REQUIRE(V_122 <= V_122);
		BOOST_REQUIRE(V_122 >= V_122);
		BOOST_REQUIRE(V_122 <= V_123);
		BOOST_REQUIRE(V_123 >= V_122);

		Vector const V_022{ 0, 2.F, 2.F };
		BOOST_REQUIRE(V_022 < V_122);

		Vector const V_103{ 1.F, 0.F, 3.F };
		BOOST_REQUIRE(V_103 < V_123);

		Vector const V_120{ 1.F, 2.F, 0.F };
		Vector const V_130{ 1.F, 3.F, 0.F };
		BOOST_REQUIRE(V_120 < V_130);
	}

	BOOST_AUTO_TEST_SUITE_END() // MinimalSuite

	BOOST_AUTO_TEST_SUITE_END() // MathVectorSuite

	BOOST_AUTO_TEST_SUITE_END() // BLASSuite

	BOOST_AUTO_TEST_SUITE_END() // MathSuite

	BOOST_AUTO_TEST_SUITE_END() // CoreSuite
} // anonymous namespace