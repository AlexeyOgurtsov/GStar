#pragma once

#include "TRBTreeTestUtils.h"
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

namespace
{
	BOOST_AUTO_TEST_SUITE(Core)
	BOOST_AUTO_TEST_SUITE(Container)
	BOOST_AUTO_TEST_SUITE(TRBTreeTestSuite)

	BOOST_AUTO_TEST_CASE(SerializationTest)
	{
		constexpr int COUNT = 10;

		BOOST_TEST_CHECKPOINT("Initialization");
		IntRBTree T;
		for (int i = 0; i < COUNT; i++)
		{
			BOOST_REQUIRE(T.Add(i, NoValue{}));
		}

		std::string serialization_buffer;

		BOOST_TEST_CHECKPOINT("Output archive");
		std::stringstream s_in_out_strm{ serialization_buffer, std::ios::in | std::ios::out };
		boost::archive::text_oarchive out_archive{ s_in_out_strm };
		out_archive << T;

		BOOST_TEST_CHECKPOINT("Input archive");
		s_in_out_strm.seekg(0);
		boost::archive::text_iarchive input_archive{ s_in_out_strm };
		IntRBTree T_deserialized;
		input_archive >> T_deserialized;
		BOOST_REQUIRE_EQUAL(T_deserialized.Num(), COUNT);
		IntRBTree::KeyValueType buf[COUNT];
		T_deserialized.CopyTo(buf);
		for (int i = 0; i < COUNT; i++)
		{
			BOOST_REQUIRE_EQUAL(buf[i].Key, i);
		}
	}

	BOOST_AUTO_TEST_SUITE_END() // TRBTreeTestSuite
	BOOST_AUTO_TEST_SUITE_END() // Container
	BOOST_AUTO_TEST_SUITE_END() // Core
} // anonymous