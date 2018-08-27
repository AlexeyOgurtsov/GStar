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

	BOOST_AUTO_TEST_SUITE_END() // MinimalSuite

	BOOST_AUTO_TEST_SUITE_END() // MathVectorSuite

	BOOST_AUTO_TEST_SUITE_END() // BLASSuite

	BOOST_AUTO_TEST_SUITE_END() // MathSuite

	BOOST_AUTO_TEST_SUITE_END() // CoreSuite
} // anonymous namespace