#pragma once

#include "Core/Cont/TVector.h"
#include <boost/test/included/unit_test.hpp>
#include <memory> // std::unique_ptr
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <string>


namespace
{

/**
* Test resize policy
*/
template<class T>
struct TestSmallSBOResizePolicy : public TVectorResizePolicyBase<T>
{
	constexpr static const char Name[] = "Default";

	constexpr static int32_t SBO_LENGTH = 4;

	constexpr int32_t CalcNewCapacity(int32_t Length) const
	{
		// Constant is used until the capacity is reached the given value
		const float GROW_FACTOR = 1.5F;

		// Constant is used for contains with big size
		const float GROW_FACTOR_BIG = 1.2F;

		const int32_t BIG_SIZE = 1024 * 1024;

		if (Length * sizeof(T) < BIG_SIZE)
		{
			return static_cast<int32_t>(Length * GROW_FACTOR);
		}
		else
		{
			return static_cast<int32_t>(Length * GROW_FACTOR_BIG);
		}
	}
};


using UniqueStr = std::unique_ptr<std::string>;

BOOST_AUTO_TEST_SUITE(Core)
BOOST_AUTO_TEST_SUITE(Container)
BOOST_AUTO_TEST_SUITE(TVectorTestSuite)


BOOST_AUTO_TEST_SUITE_END() // TVectorTestSuite
BOOST_AUTO_TEST_SUITE_END() // Container
BOOST_AUTO_TEST_SUITE_END() // Core

} 