#pragma once

#include "TVectorTestUtils.h"

namespace
{

BOOST_AUTO_TEST_SUITE(Core)
BOOST_AUTO_TEST_SUITE(Container)
BOOST_AUTO_TEST_SUITE(TVectorTestSuite)

BOOST_AUTO_TEST_CASE(MoveOnly_MainTest)
{
	TVector<std::unique_ptr<std::string>> V;

	BOOST_TEST_CHECKPOINT("Emplace empty");
	V.Emplace(); // Insert default-constructed
	BOOST_REQUIRE_EQUAL(V.Len(), 1);
	BOOST_REQUIRE_EQUAL(V[0].get(), nullptr);

	BOOST_TEST_CHECKPOINT("Add rvalue");
	auto pValue2 = V.Add(std::make_unique<std::string>("Add"));
	BOOST_REQUIRE_EQUAL(V.Len(), 2);
	BOOST_REQUIRE(V.Last().get() != nullptr);
	BOOST_REQUIRE_EQUAL(V.Last().get(), pValue2->get());

	BOOST_TEST_CHECKPOINT("Insert");
	auto pInsertedAt1 = V.Insert(1, std::make_unique<std::string>("Insert(1, ...)"));
	BOOST_REQUIRE_EQUAL(V[1].get(), pInsertedAt1->get());

	BOOST_TEST_CHECKPOINT("ReserveGrow");
	const int oldLength = V.Len();
	V.ReserveGrow(100);
	BOOST_REQUIRE_EQUAL(V.Len(), oldLength);
	BOOST_REQUIRE(V.MaxLen() >= V.Len());

	BOOST_TEST_CHECKPOINT("Resize");
	const int BIGGER_LENGTH = 200;
	V.ResizeGrow(BIGGER_LENGTH);
	BOOST_REQUIRE_EQUAL(V.Len(), BIGGER_LENGTH);
	BOOST_REQUIRE(V.MaxLen() >= V.Len());

	BOOST_TEST_CHECKPOINT("Push");
	auto pPushed = V.Push(std::make_unique<std::string>("Poppable"));
	BOOST_REQUIRE_EQUAL(*(pPushed->get()), std::string("Poppable"));

	BOOST_TEST_CHECKPOINT("Pop");
	const auto popped = V.Pop();
	BOOST_REQUIRE_EQUAL(*popped, std::string("Poppable"));

	BOOST_TEST_CHECKPOINT("ShrinkToFit");
	V.ShrinkToFit();
}

BOOST_AUTO_TEST_CASE(MoveOnly_AppendSmallVector, *boost::unit_test::depends_on("Core/Container/TVectorTestSuite/MoveOnly_MainTest"))
{
	// Prepare source vectors
	constexpr int32_t NUM_SOURCES = 2;
	constexpr int32_t SOURCE_LEN = 3;
	TVector<std::unique_ptr<std::string>> V_source[NUM_SOURCES];
	for (int sourceIndex = 0; sourceIndex < NUM_SOURCES; sourceIndex++)
	{
		for (int currSourceLen = 0; currSourceLen < SOURCE_LEN; currSourceLen++)
		{
			V_source[sourceIndex].Add(std::make_unique<std::string>(std::to_string(currSourceLen + 1)));
		}
	}

	// Prepare destination vector
	const int32_t INITIALV0_LEN = 2;
	TVector<std::unique_ptr<std::string>> V0;
	V0.Add(std::make_unique<std::string>("A"));
	V0.Add(std::make_unique<std::string>("B"));

	BOOST_TEST_CHECKPOINT("Append small");
	V0.Append(std::move(V_source[0]));
	BOOST_REQUIRE_EQUAL(V0.Len(), SOURCE_LEN + INITIALV0_LEN);
	BOOST_REQUIRE_EQUAL(*V0[0].get(), std::string("A"));
	BOOST_REQUIRE_EQUAL(*V0[1].get(), std::string("B"));
	for (int i = 0; i < SOURCE_LEN; i++)
	{
		BOOST_REQUIRE_EQUAL(*V0[INITIALV0_LEN + i].get(), std::to_string(i + 1));
	}
}

BOOST_AUTO_TEST_SUITE_END() // TVectorTestSuite
BOOST_AUTO_TEST_SUITE_END() // Container
BOOST_AUTO_TEST_SUITE_END() // Core

} 