#pragma once

#include "TVectorTestUtils.h"

namespace
{

BOOST_AUTO_TEST_SUITE(Core)
BOOST_AUTO_TEST_SUITE(Container)
BOOST_AUTO_TEST_SUITE(TVectorTestSuite)

BOOST_AUTO_TEST_CASE(MoveOnly_SmallConstructFromArray, *boost::unit_test::depends_on("Core/Container/TVectorTestSuite/SimplePod_SmallMainTest"))
{
	// @TODO
	constexpr int THE_ARRAY_SIZE = 5;
	UniqueStr THE_ARRAY[THE_ARRAY_SIZE]
	{
		std::make_unique<std::string>("1"), 
		std::make_unique<std::string>("2"),
		std::make_unique<std::string>("3"),
		std::make_unique<std::string>("4"),
		std::make_unique<std::string>("5")
	};
	TVector<UniqueStr> V(THE_ARRAY, THE_ARRAY_SIZE, EForceMove::Value);
	BOOST_REQUIRE_EQUAL(V.Len(), 5);
	BOOST_REQUIRE_EQUAL(*V[0], std::string("1"));
	BOOST_REQUIRE_EQUAL(*V[1], std::string("2"));
	BOOST_REQUIRE_EQUAL(*V[2], std::string("3"));
	BOOST_REQUIRE_EQUAL(*V[3], std::string("4"));
	BOOST_REQUIRE_EQUAL(*V[4], std::string("5"));
}

BOOST_AUTO_TEST_CASE
(
	MoveOnly_BigToSmallMoveConstruction,
	*boost::unit_test::depends_on("Core/Container/TVectorTestSuite/SimplePod_SmallMoveConstruction")
)
{
	constexpr int SOURCE_COUNT = 1000;

	// We need this case, because we need to check that we copied NOT only the SBO, but entire buffer
	BOOST_TEST_CHECKPOINT("Source array");
	TVector<UniqueStr> V_source;
	TVector<UniqueStr> V_source2;
	for (int i = 0; i < SOURCE_COUNT; i++)
	{
		std::string* pS = new std::string(std::to_string(i));
		V_source.Add(UniqueStr(pS));
		std::string* pS2 = new std::string(std::to_string(i));
		V_source2.Add(UniqueStr(pS2));
	}

	BOOST_TEST_CHECKPOINT("Destination array");
	TVector<UniqueStr> V(std::move(V_source));
	BOOST_REQUIRE_EQUAL(V.Len(), V_source2.Len());
	for (int i = 0; i < V.Len(); i++)
	{
		BOOST_REQUIRE(V_source2[i].get());
		BOOST_REQUIRE(V[i].get());
		BOOST_REQUIRE_EQUAL(*V_source2[i].get(), *V[i].get());
	}
}

BOOST_AUTO_TEST_CASE
(
	MoveOnly_BigToSmallMoveConstruction_DifferentResizePolicy,
	*boost::unit_test::depends_on("Core/Container/TVectorTestSuite/MoveOnly_BigToSmallMoveConstruction")
)
{
	constexpr int SOURCE_COUNT = 1000;

	// We need this case, because we need to check that we copied NOT only the SBO, but entire buffer
	BOOST_TEST_CHECKPOINT("Source array");
	TVector<UniqueStr> V_source;
	TVector<UniqueStr> V_source2;
	for (int i = 0; i < SOURCE_COUNT; i++)
	{
		std::string* pS = new std::string(std::to_string(i));
		V_source.Add(UniqueStr(pS));
		std::string* pS2 = new std::string(std::to_string(i));
		V_source2.Add(UniqueStr(pS2));
	}

	BOOST_TEST_CHECKPOINT("Destination array");
	TVector<UniqueStr, TestSmallSBOResizePolicy> V(std::move(V_source));
	BOOST_REQUIRE_EQUAL(V.Len(), V_source2.Len());
	for (int i = 0; i < V.Len(); i++)
	{
		BOOST_REQUIRE(V_source2[i].get());
		BOOST_REQUIRE(V[i].get());
		BOOST_REQUIRE_EQUAL(*V_source2[i].get(), *V[i].get());
	}
}

BOOST_AUTO_TEST_CASE
(
	MoveOnly_BigMoveAssign,
	*boost::unit_test::depends_on("Core/Container/TVectorTestSuite/SimplePod_SmallMoveAssign")
)
{
	// WARNING!!! We should NOT check that the vector is invalidated after move, because
	// it's not always the case (not a requirement).

	BOOST_TEST_CHECKPOINT("Source arrays");
	constexpr int COUNT = 1000;
	TVector<UniqueStr> V_source1;
	TVector<UniqueStr> V_source2;
	TVector<UniqueStr> V_source3;
	TVector<UniqueStr> V_source4;
	for (int i = 0; i < COUNT; i++)
	{
		std::string* pS1 = new std::string(std::to_string(i));
		V_source1.Add(UniqueStr(pS1));

		std::string* pS2 = new std::string(std::to_string(i));
		V_source2.Add(UniqueStr(pS2));

		std::string* pS3 = new std::string(std::to_string(i));
		V_source3.Add(UniqueStr(pS3));

		std::string* pS4 = new std::string(std::to_string(i));
		V_source4.Add(UniqueStr(pS4));
	}

	BOOST_TEST_CHECKPOINT("Copy to empty vector");
	TVector<UniqueStr> V_assign_to_empty;
	V_assign_to_empty = std::move(V_source1);
	BOOST_REQUIRE(V_assign_to_empty.Num() == COUNT);

	BOOST_TEST_CHECKPOINT("Copy to lesser vector");
	TVector<UniqueStr> V_assign_to_3length;
	V_assign_to_3length.Add(std::make_unique<std::string>(std::to_string(9)));
	V_assign_to_3length.Add(std::make_unique<std::string>(std::to_string(6)));
	V_assign_to_3length.Add(std::make_unique<std::string>(std::to_string(8)));

	V_assign_to_3length = std::move(V_source2);
	BOOST_REQUIRE_EQUAL(V_assign_to_3length.Len(), COUNT);
	for (int i = 0; i < V_assign_to_3length.Len(); i++)
	{
		BOOST_REQUIRE(V_assign_to_3length[i].get());
		BOOST_REQUIRE(V_source3[i].get());
		BOOST_REQUIRE(*V_assign_to_3length[i].get() == *V_source3[i].get());
	}

	BOOST_TEST_CHECKPOINT("Copy to greater vector");
	TVector<UniqueStr> V_assign_to_greater;
	for (int i = 0; i < 10000; i++)
	{
		std::string* pS = new std::string(std::to_string(i));
		V_assign_to_greater.Add(UniqueStr(pS));
	}
	V_assign_to_greater = std::move(V_source3);
	BOOST_REQUIRE_EQUAL(V_assign_to_greater.Len(), V_source4.Len());
	for (int i = 0; i < V_assign_to_greater.Len(); i++)
	{
		BOOST_REQUIRE(V_assign_to_greater[i].get());
		BOOST_REQUIRE(V_source4[i].get());
		BOOST_REQUIRE(*V_assign_to_greater[i].get() == *V_source4[i].get());
	}
}

BOOST_AUTO_TEST_CASE
(
	MoveOnly_BigMoveAssign_DifferentResizePolicy,
	*boost::unit_test::depends_on("Core/Container/TVectorTestSuite/MoveOnly_BigMoveAssign")
)
{
	// WARNING!!! We should NOT check that the vector is invalidated after move, because
	// it's not always the case (not a requirement).

	BOOST_TEST_CHECKPOINT("Source arrays");
	constexpr int COUNT = 1000;
	TVector<UniqueStr> V_source1;
	TVector<UniqueStr> V_source2;
	TVector<UniqueStr> V_source3;
	TVector<UniqueStr> V_source4;
	for (int i = 0; i < COUNT; i++)
	{
		std::string* pS1 = new std::string(std::to_string(i));
		V_source1.Add(UniqueStr(pS1));


		std::string* pS2 = new std::string(std::to_string(i));
		V_source2.Add(UniqueStr(pS2));

		std::string* pS3 = new std::string(std::to_string(i));
		V_source3.Add(UniqueStr(pS3));

		std::string* pS4 = new std::string(std::to_string(i));
		V_source4.Add(UniqueStr(pS4));
	}

	BOOST_TEST_CHECKPOINT("Copy to empty vector");
	TVector<UniqueStr, TestSmallSBOResizePolicy> V_assign_to_empty;
	V_assign_to_empty = std::move(V_source1);
	BOOST_REQUIRE(V_assign_to_empty.Num() == COUNT);

	BOOST_TEST_CHECKPOINT("Copy to lesser vector");
	TVector<UniqueStr, TestSmallSBOResizePolicy> V_assign_to_3length;
	V_assign_to_3length.Add(std::make_unique<std::string>(std::to_string(9)));
	V_assign_to_3length.Add(std::make_unique<std::string>(std::to_string(6)));
	V_assign_to_3length.Add(std::make_unique<std::string>(std::to_string(8)));
	V_assign_to_3length = std::move(V_source2);
	BOOST_REQUIRE_EQUAL(V_assign_to_3length.Len(), COUNT);
	for (int i = 0; i < V_assign_to_3length.Len(); i++)
	{
		BOOST_REQUIRE(V_assign_to_3length[i].get());
		BOOST_REQUIRE(V_source3[i].get());
		BOOST_REQUIRE(*V_assign_to_3length[i].get() == *V_source3[i].get());
	}

	BOOST_TEST_CHECKPOINT("Copy to greater vector");
	TVector<UniqueStr, TestSmallSBOResizePolicy> V_assign_to_greater;
	for (int i = 0; i < 10000; i++)
	{
		std::string* pS = new std::string(std::to_string(i));
		V_assign_to_greater.Add(UniqueStr(pS));
	}
	V_assign_to_greater = std::move(V_source3);
	BOOST_REQUIRE_EQUAL(V_assign_to_greater.Len(), COUNT);
	for (int i = 0; i < V_assign_to_greater.Len(); i++)
	{
		BOOST_REQUIRE(V_assign_to_greater[i].get());
		BOOST_REQUIRE(V_source4[i].get());
		BOOST_REQUIRE(*V_assign_to_greater[i].get() == *V_source4[i].get());
	}
}

BOOST_AUTO_TEST_SUITE_END() // TVectorTestSuite
BOOST_AUTO_TEST_SUITE_END() // Container
BOOST_AUTO_TEST_SUITE_END() // Core

} 