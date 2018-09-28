#pragma once

#include "TVectorTestUtils.h"

namespace
{

BOOST_AUTO_TEST_SUITE(Core)
BOOST_AUTO_TEST_SUITE(Container)
BOOST_AUTO_TEST_SUITE(TVectorTestSuite)

BOOST_AUTO_TEST_CASE(SerializationTest)
{
	constexpr int COUNT = 10;

	BOOST_TEST_CHECKPOINT("Initialization");
	TVector<int> V;
	for (int i =0; i < COUNT; i++)
	{
		V.Add(i);
	}


	std::string serialization_buffer;
	serialization_buffer.resize(100);

	BOOST_TEST_CHECKPOINT("Output archive");
	std::stringstream s_in_out_strm{ serialization_buffer, std::ios::out | std::ios::in };

	boost::archive::text_oarchive out_archive { s_in_out_strm };
	out_archive << V;

	BOOST_TEST_CHECKPOINT("Input archive");
	s_in_out_strm.seekg(0);
	boost::archive::text_iarchive input_archive{ s_in_out_strm };
	TVector<int> T_deserialized;
	input_archive >> T_deserialized;
	BOOST_REQUIRE_EQUAL(T_deserialized.Len(), COUNT);
	for (int i = 0; i < COUNT; i++)
	{
		BOOST_REQUIRE_EQUAL(V[i], i);
	}
}

BOOST_AUTO_TEST_SUITE_END() // TVectorTestSuite
BOOST_AUTO_TEST_SUITE_END() // Container
BOOST_AUTO_TEST_SUITE_END() // Core

} 