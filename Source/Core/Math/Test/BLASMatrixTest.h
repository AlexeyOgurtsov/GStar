#pragma once

#include "Core/Math/Matrix.h"
#include <boost/test/included/unit_test.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

namespace 
{
	BOOST_AUTO_TEST_SUITE(CoreSuite)

	BOOST_AUTO_TEST_SUITE(MathSuite)

	BOOST_AUTO_TEST_SUITE(BLASSuite)
	
	BOOST_AUTO_TEST_SUITE(MathMatrixSuite)

	BOOST_AUTO_TEST_CASE(SerializationSuite)
	{
		// TODO
	}

	BOOST_AUTO_TEST_SUITE(MinimalSuite)

	BOOST_AUTO_TEST_CASE(MatrixConstructionTest)
	{
		// It must compile:
		Matrix DefaultConstructedM;
		
		Matrix const MatFromVec
		{
			Vector{1,2,3},
			Vector{4,5,6},
			Vector{7,8,9},
			Vector{10,11,12}
		};
		BOOST_REQUIRE_EQUAL(1, MatFromVec.M[0][0]);
		BOOST_REQUIRE_EQUAL(2, MatFromVec.M[0][1]);
		BOOST_REQUIRE_EQUAL(3, MatFromVec.M[0][2]);

		BOOST_REQUIRE_EQUAL(4, MatFromVec.M[1][0]);
		BOOST_REQUIRE_EQUAL(5, MatFromVec.M[1][1]);
		BOOST_REQUIRE_EQUAL(6, MatFromVec.M[1][2]);

		BOOST_REQUIRE_EQUAL(7, MatFromVec.M[2][0]);
		BOOST_REQUIRE_EQUAL(8, MatFromVec.M[2][1]);
		BOOST_REQUIRE_EQUAL(9, MatFromVec.M[2][2]);

		BOOST_REQUIRE_EQUAL(10, MatFromVec.M[3][0]);
		BOOST_REQUIRE_EQUAL(11, MatFromVec.M[3][1]);
		BOOST_REQUIRE_EQUAL(12, MatFromVec.M[3][2]);

		BOOST_REQUIRE_EQUAL(0, MatFromVec.M[0][3]);
		BOOST_REQUIRE_EQUAL(0, MatFromVec.M[1][3]);
		BOOST_REQUIRE_EQUAL(0, MatFromVec.M[2][3]);
		BOOST_REQUIRE_EQUAL(1.F, MatFromVec.M[3][3]);
	}

	BOOST_AUTO_TEST_CASE(CompareTest)
	{
		// TODO
	}

	BOOST_AUTO_TEST_SUITE_END() // MinimalSuite

	BOOST_AUTO_TEST_SUITE_END() // MathMatrixSuite

	BOOST_AUTO_TEST_SUITE_END() // BLASSuite

	BOOST_AUTO_TEST_SUITE_END() // MathSuite

	BOOST_AUTO_TEST_SUITE_END() // CoreSuite
} // anonymous namespace