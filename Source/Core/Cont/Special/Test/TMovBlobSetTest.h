#pragma once

#include <boost/test/included/unit_test.hpp>
#include "../TMovBlobSet.h"

BOOST_AUTO_TEST_SUITE(Core)
BOOST_AUTO_TEST_SUITE(Container)
BOOST_AUTO_TEST_SUITE(Special)
BOOST_AUTO_TEST_SUITE(TMovBlobSetTestSuite)

BOOST_AUTO_TEST_CASE(TMovBlobSet_InitAddDeleteContains)
{
	std::string const TEST_PREF_KEY = "ABC";
	std::string const TEST_KEY = "ABCDEFG";

	BOOST_TEST_CHECKPOINT("Initialization");
	TMovBlobSet S{8};
	BOOST_REQUIRE( S.Empty() );
	BOOST_REQUIRE_EQUAL( 0, S.Count() );
	BOOST_REQUIRE( ! S.Contains(TEST_PREF_KEY.data(), TEST_PREF_KEY.size()) );

	BOOST_TEST_CHECKPOINT("Removing NON-registered key");
	bool const bRemoved = S.Remove(TEST_PREF_KEY.data(), TEST_PREF_KEY.size());
	BOOST_REQUIRE( ! bRemoved );

	BOOST_TEST_CHECKPOINT("Adding prefix key");
	int PrefKeyIndex = S.Add(TEST_PREF_KEY.data(), TEST_PREF_KEY.size());
	BOOST_REQUIRE(PrefKeyIndex >= 0);
	BOOST_REQUIRE(S.Contains(TEST_PREF_KEY.data(), TEST_PREF_KEY.size()));
	BOOST_REQUIRE( ! S.Contains(TEST_KEY.data(), TEST_KEY.size()) );
	BOOST_REQUIRE( ! S.Empty() );
	BOOST_REQUIRE_EQUAL( 1, S.Count() );

	BOOST_TEST_CHECKPOINT("Trying to remove a key that is NOT in the container");
	bool const bRemoved_Key_NotRegistered = S.Remove(TEST_KEY.data(), TEST_KEY.size());
	BOOST_REQUIRE(S.Contains(TEST_PREF_KEY.data(), TEST_PREF_KEY.size()));
	BOOST_REQUIRE( !bRemoved_Key_NotRegistered );
	BOOST_REQUIRE_EQUAL(1, S.Count());

	BOOST_TEST_CHECKPOINT("Adding prefix key a second time");
	int const PrefKeyIndex_2 = S.Add(TEST_PREF_KEY.data(), TEST_PREF_KEY.size());
	BOOST_REQUIRE_EQUAL(PrefKeyIndex_2, PrefKeyIndex);
	BOOST_REQUIRE(S.Contains(TEST_PREF_KEY.data(), TEST_PREF_KEY.size()));
	BOOST_REQUIRE_EQUAL(1, S.Count());

	BOOST_TEST_CHECKPOINT("Adding key");
	int const KeyIndex = S.Add(TEST_KEY.data(), TEST_KEY.size());
	BOOST_REQUIRE(KeyIndex >= 0);
	BOOST_REQUIRE(S.Contains(TEST_PREF_KEY.data(), TEST_PREF_KEY.size()));
	BOOST_REQUIRE(S.Contains(TEST_KEY.data(), TEST_KEY.size()));
	BOOST_REQUIRE_EQUAL(2, S.Count());

	BOOST_TEST_CHECKPOINT("Removing prefix key");
	bool const bRemoved_ExistingPrefixKey = S.Remove(TEST_PREF_KEY.data(), TEST_PREF_KEY.size());
	BOOST_REQUIRE(bRemoved_ExistingPrefixKey);
	BOOST_REQUIRE( ! S.Contains(TEST_PREF_KEY.data(), TEST_PREF_KEY.size()) );
	BOOST_REQUIRE_EQUAL(1, S.Count());
	BOOST_REQUIRE(S.Contains(TEST_KEY.data(), TEST_KEY.size()));

	BOOST_TEST_CHECKPOINT("Removing key");
	bool const bRemoved_ExistingKey = S.Remove(TEST_KEY.data(), TEST_KEY.size());
	BOOST_REQUIRE(bRemoved_ExistingKey);
	BOOST_REQUIRE(S.Empty());
	BOOST_REQUIRE( ! S.Contains(TEST_KEY.data(), TEST_KEY.size()) );
}

BOOST_AUTO_TEST_CASE(CopyConstruct, *boost::unit_test::depends_on("Core/Container/Special/TMovBlobSetTestSuite/TMovBlobSet_InitAddDeleteContains"))
{
	std::string const TEST_PREF_KEY = "ABC";
	std::string const TEST_KEY = "ABCDEFG";

	BOOST_TEST_CHECKPOINT("Initializing source");
	TMovBlobSet Src{ 8 };
	Src.Add(TEST_PREF_KEY.data(), TEST_PREF_KEY.size());
	Src.Add(TEST_KEY.data(), TEST_KEY.size());

	BOOST_TEST_CHECKPOINT("Copy-constructing");
	TMovBlobSet Dest{ Src };
	BOOST_REQUIRE_EQUAL( Src.Count(), Dest.Count() );
	BOOST_REQUIRE_EQUAL
	( 
		Src.Contains(TEST_PREF_KEY.data(), TEST_PREF_KEY.size()),
		Dest.Contains(TEST_PREF_KEY.data(), TEST_PREF_KEY.size())
	);
	BOOST_REQUIRE_EQUAL
	(
		Src.Contains(TEST_KEY.data(), TEST_KEY.size()),
		Dest.Contains(TEST_KEY.data(), TEST_KEY.size())
	);
}

BOOST_AUTO_TEST_CASE(CopyTest, *boost::unit_test::depends_on("Core/Container/Special/TMovBlobSetTestSuite/TMovBlobSet_InitAddDeleteContains"))
{
	std::string const TEST_PREF_KEY = "ABC";
	std::string const TEST_KEY = "ABCDEFG";

	BOOST_TEST_CHECKPOINT("Initializing source");
	TMovBlobSet Src{ 8 };
	Src.Add(TEST_PREF_KEY.data(), TEST_PREF_KEY.size());
	Src.Add(TEST_KEY.data(), TEST_KEY.size());

	BOOST_TEST_CHECKPOINT("Copying");
	TMovBlobSet Dest{ 8 };
	Dest = Src;
	BOOST_REQUIRE_EQUAL(Src.Count(), Dest.Count());
	BOOST_REQUIRE_EQUAL
	(
		Src.Contains(TEST_PREF_KEY.data(), TEST_PREF_KEY.size()),
		Dest.Contains(TEST_PREF_KEY.data(), TEST_PREF_KEY.size())
	);
	BOOST_REQUIRE_EQUAL
	(
		Src.Contains(TEST_KEY.data(), TEST_KEY.size()),
		Dest.Contains(TEST_KEY.data(), TEST_KEY.size())
	);
}

BOOST_AUTO_TEST_CASE(MoveConstruct, *boost::unit_test::depends_on("Core/Container/Special/TMovBlobSetTestSuite/TMovBlobSet_InitAddDeleteContains"))
{
	std::string const TEST_PREF_KEY = "ABC";
	std::string const TEST_KEY = "ABCDEFG";

	BOOST_TEST_CHECKPOINT("Initializing source");
	TMovBlobSet Src{ 8 };
	Src.Add(TEST_PREF_KEY.data(), TEST_PREF_KEY.size());
	Src.Add(TEST_KEY.data(), TEST_KEY.size());

	BOOST_TEST_CHECKPOINT("Move-constructing");
	TMovBlobSet Dest{ std::move(Src) };

	BOOST_REQUIRE_EQUAL(2, Dest.Count());
	BOOST_REQUIRE(Dest.Contains(TEST_PREF_KEY.data(), TEST_PREF_KEY.size()));
	BOOST_REQUIRE(Dest.Contains(TEST_KEY.data(), TEST_KEY.size()));
}

BOOST_AUTO_TEST_CASE(MoveCopy, *boost::unit_test::depends_on("Core/Container/Special/TMovBlobSetTestSuite/TMovBlobSet_InitAddDeleteContains"))
{
	std::string const TEST_PREF_KEY = "ABC";
	std::string const TEST_KEY = "ABCDEFG";

	BOOST_TEST_CHECKPOINT("Initializing source");
	TMovBlobSet Src{ 8 };
	Src.Add(TEST_PREF_KEY.data(), TEST_PREF_KEY.size());
	Src.Add(TEST_KEY.data(), TEST_KEY.size());

	BOOST_TEST_CHECKPOINT("Move-copying");
	TMovBlobSet Dest{ 8 };
	Dest = std::move(Src);

	BOOST_REQUIRE_EQUAL(2, Dest.Count());
	BOOST_REQUIRE(Dest.Contains(TEST_PREF_KEY.data(), TEST_PREF_KEY.size()));
	BOOST_REQUIRE(Dest.Contains(TEST_KEY.data(), TEST_KEY.size()));
}

BOOST_AUTO_TEST_SUITE_END() // TMovBlobSetTestSuite
BOOST_AUTO_TEST_SUITE_END() // Special
BOOST_AUTO_TEST_SUITE_END() // Container
BOOST_AUTO_TEST_SUITE_END() // Core