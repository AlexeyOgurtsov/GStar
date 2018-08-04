#pragma once

#include "../TKeyValue.h"
#include <boost/test/included/unit_test.hpp>

#include <string>

namespace
{

BOOST_AUTO_TEST_SUITE(Core)
BOOST_AUTO_TEST_SUITE(Container)
BOOST_AUTO_TEST_SUITE(UT)
BOOST_AUTO_TEST_SUITE(TKeyValueSuite)


BOOST_AUTO_TEST_CASE(MainTest)
{
	using KeyValue_IntStr = TKeyValue<KVType<std::string,int>>;

	BOOST_TEST_CHECKPOINT("Construction");
	std::string const KV_REF_KEY = "One";
	int const KV_REF_VALUE = 1;	

	KeyValue_IntStr const KV { KV_REF_KEY, KV_REF_VALUE };
	BOOST_REQUIRE_EQUAL(KV.Key, KV_REF_KEY);
	BOOST_REQUIRE_EQUAL(KV.Value, KV_REF_VALUE);
}

BOOST_AUTO_TEST_CASE(MakeKeyValue)
{
	using KeyValue_IntStr = TKeyValue<KVType<std::string, int>>;

	BOOST_TEST_CHECKPOINT("Construction");
	std::string KV_REF_KEY = "One";
	int KV_REF_VALUE = 1;

	//KeyValue_IntStr const KV ( MakeKeyValue(KV_REF_KEY, KV_REF_VALUE) );
	//BOOST_REQUIRE_EQUAL(KV.Key, KV_REF_KEY);
	//BOOST_REQUIRE_EQUAL(KV.Value, KV_REF_VALUE);
}

BOOST_AUTO_TEST_CASE(EqualityTest)
{
	using KeyValue_IntStr = TKeyValue<KVType<std::string, int>>;

	BOOST_TEST_CHECKPOINT("Construction");
	std::string const KV_REF_KEY = "One";
	int const KV_REF_VALUE = 1;
	KeyValue_IntStr const KV{ KV_REF_KEY, KV_REF_VALUE };
	BOOST_REQUIRE(KV == KV);

	BOOST_TEST_CHECKPOINT("Construction2");
	std::string const KV_REF_KEY_2 = "One";
	int const KV_REF_VALUE_2 = 2;
	KeyValue_IntStr const KV_2{ KV_REF_KEY_2, KV_REF_VALUE_2 };

	BOOST_REQUIRE(KV != KV_2);
}

BOOST_AUTO_TEST_CASE(HashTest)
{
	using KeyValue_IntStr = TKeyValue<KVType<std::string, int>>;

	BOOST_TEST_CHECKPOINT("Construction");
	std::string const KV_REF_KEY = "One";
	int const KV_REF_VALUE = 1;
	KeyValue_IntStr const KV{ KV_REF_KEY, KV_REF_VALUE };
	BOOST_REQUIRE_EQUAL(std::hash<KeyValue_IntStr>() (KV), std::hash<KeyValue_IntStr>() (KV));

	BOOST_TEST_CHECKPOINT("Construction2");
	std::string const KV_REF_KEY_2 = "One";
	int const KV_REF_VALUE_2 = 2;
	KeyValue_IntStr const KV_2{ KV_REF_KEY_2, KV_REF_VALUE_2 };

	BOOST_REQUIRE(std::hash<KeyValue_IntStr>() (KV) != std::hash<KeyValue_IntStr>() (KV_2));
}


BOOST_AUTO_TEST_SUITE_END() // TKeyValueSuite
BOOST_AUTO_TEST_SUITE_END() // UT
BOOST_AUTO_TEST_SUITE_END() // Container
BOOST_AUTO_TEST_SUITE_END() // Core

} // anonymous