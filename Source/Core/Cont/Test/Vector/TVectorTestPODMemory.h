#pragma once

#include "TVectorTestUtils.h"

namespace
{

BOOST_AUTO_TEST_SUITE(Core)
BOOST_AUTO_TEST_SUITE(Container)
BOOST_AUTO_TEST_SUITE(TVectorTestSuite)


BOOST_AUTO_TEST_CASE(SimplePod_SmallReserveGrow_OnEmpty, *boost::unit_test::depends_on("Core/Container/TVectorTestSuite/SimplePod_SmallMainTest"))
{
	TVector<int> V;
	constexpr int RESERVED_CAPACITY = 4;
	V.ReserveGrow(RESERVED_CAPACITY);
	const int* pOldData = V.Data();
	const auto OldMaxLength = V.MaxLen();
	BOOST_REQUIRE(OldMaxLength >= RESERVED_CAPACITY);
	V.Add(1);
	V.Add(2);
	V.Add(3);
	V.Add(4);
	BOOST_REQUIRE_EQUAL(pOldData, V.Data());
	BOOST_REQUIRE_EQUAL(OldMaxLength, V.MaxLen());
}

BOOST_AUTO_TEST_CASE(SimplePod_ReserveGrow_ToBig_OnSmall, *boost::unit_test::depends_on("Core/Container/TVectorTestSuite/SimplePod_SmallMainTest"))
{
	TVector<int> V;
	V.Add(1);
	V.Add(2);
	V.Add(3);

	BOOST_TEST_CHECKPOINT("Reserving");
	constexpr int RESERVE_CAPACITY = 100;
	V.ReserveGrow(RESERVE_CAPACITY);
	BOOST_REQUIRE_EQUAL(V.Len(), 3);
	BOOST_REQUIRE(V.MaxLen() >= V.Len());
	BOOST_REQUIRE(V.Data() != nullptr);
	BOOST_REQUIRE_EQUAL(V[0], 1);
	BOOST_REQUIRE_EQUAL(V[1], 2);
	BOOST_REQUIRE_EQUAL(V[2], 3);

	BOOST_TEST_CHECKPOINT("Checking that we can add without reallocations");
	const int* pOldData = V.Data();
	const auto OldMaxLength = V.MaxLen();
	for (int i = 0; i < RESERVE_CAPACITY; i++) { V.Add(12); }
	BOOST_REQUIRE_EQUAL(pOldData, V.Data());
	BOOST_REQUIRE_EQUAL(OldMaxLength, V.MaxLen());
}

BOOST_AUTO_TEST_CASE(SimplePod_SetLength_ToBig, *boost::unit_test::depends_on("Core/Container/TVectorTestSuite/SimplePod_SmallMainTest"))
{
	TVector<int> V;
	V.Add(1);
	V.Add(2);
	V.Add(3);

	constexpr int BIG_LENGTH = 100;
	V.SetLength(BIG_LENGTH);

	// Checking that the length is set properly.
	BOOST_REQUIRE_EQUAL(V.Len(), BIG_LENGTH);
	BOOST_REQUIRE(V.MaxLen() >= V.Len());
	BOOST_REQUIRE(V.Data() != nullptr);

	// Check that untouched vector part is still the same
	BOOST_REQUIRE_EQUAL(V[0], 1);
	BOOST_REQUIRE_EQUAL(V[1], 2);
	BOOST_REQUIRE_EQUAL(V[2], 3);

	// Check that the reset of the new vector is properly zeroed
	// WARNING!!! We should NOT check that extra elements are zeroes, because int is trivially default-constructible, and will NOT be initialized.
}

BOOST_AUTO_TEST_CASE(SimplePod_SetLength_LesserSmall_FromBig, *boost::unit_test::depends_on("Core/Container/TVectorTestSuite/SimplePod_SmallMainTest"))
{
	// Depends on Add with growing
	TVector<int> V;
	const int BIG_LENGTH = 100;
	// Adding elements (probably will grow)
	for (int i = 0; i < BIG_LENGTH; i++)
	{
		V.Add(i + 1);
	}
	// Check that we properly added elements 
	for (int i = 0; i < V.Len(); i++)
	{
		BOOST_REQUIRE_EQUAL(V[i], i + 1);
	}
	BOOST_REQUIRE_EQUAL(V.Len(), BIG_LENGTH);
	BOOST_REQUIRE(V.MaxLen() >= V.Len());
	BOOST_REQUIRE(V.Data() != nullptr);

	// Small length
	// Trying to set small length
	const int SMALL_LENGTH = 7;
	V.SetLength(SMALL_LENGTH);
	BOOST_REQUIRE_EQUAL(V.Len(), SMALL_LENGTH);
	BOOST_REQUIRE(V.MaxLen() >= V.Len());
	for (int i = 0; i < V.Len(); i++)
	{
		BOOST_REQUIRE_EQUAL(V[i], i + 1);
	}
}

BOOST_AUTO_TEST_CASE(SimplePod_SetLength_LesserBigFromBig, *boost::unit_test::depends_on("Core/Container/TVectorTestSuite/SimplePod_SmallMainTest"))
{
	// Depends on Add with growing
	TVector<int> V;
	const int BIG_LENGTH = 100;
	// Adding elements (probably will grow)
	for (int i = 0; i < BIG_LENGTH; i++) { V.Add(i + 1); }
	// Check that we properly added elements 
	for (int i = 0; i < V.Len(); i++)
	{
		BOOST_REQUIRE_EQUAL(V[i], i + 1);
	}

	BOOST_REQUIRE_EQUAL(V.Len(), BIG_LENGTH);
	BOOST_REQUIRE(V.MaxLen() >= V.Len());
	BOOST_REQUIRE(V.Data() != nullptr);

	// Trying to set new, lesser, but big length
	const int LESSER_BIG_LENGTH = 80;
	V.SetLength(LESSER_BIG_LENGTH);
	BOOST_REQUIRE_EQUAL(V.Len(), LESSER_BIG_LENGTH);
	BOOST_REQUIRE(V.MaxLen() >= V.Len());
	for (int i = 0; i < V.Len(); i++)
	{
		BOOST_REQUIRE_EQUAL(V[i], i + 1);
	}
}

BOOST_AUTO_TEST_CASE(SimplePod_ShrinkToFit_BigToBig, *boost::unit_test::depends_on("Core/Container/TVectorTestSuite/SimplePod_SmallMainTest"))
{
	TVector<int> V;
	const int BIG_LENGTH = 100;
	// Adding elements (probably will grow)
	for (int i = 0; i < BIG_LENGTH; i++) { V.Add(i + 1); }
	// Check that we properly added elements 
	for (int i = 0; i < V.Len(); i++)
	{
		BOOST_REQUIRE_EQUAL(V[i], i + 1);
	}
	BOOST_REQUIRE_EQUAL(V.Len(), BIG_LENGTH);
	BOOST_REQUIRE(V.MaxLen() >= V.Len());
	BOOST_REQUIRE(V.Data() != nullptr);

	constexpr int LESSER_BIG_LENGTH = 40;
	while (V.Len() > LESSER_BIG_LENGTH)
	{
		V.Pop();
	}

	V.ShrinkToFit();
	BOOST_REQUIRE_EQUAL(V.Len(), LESSER_BIG_LENGTH);
	BOOST_REQUIRE(V.MaxLen() >= V.Len());
	BOOST_REQUIRE(V.Data() != nullptr);
	for (int i = 0; i < V.Len(); i++)
	{
		BOOST_REQUIRE_EQUAL(V[i], i + 1);
	}
}

BOOST_AUTO_TEST_CASE(SimplePod_ShrinkToFit_BigToSmall, *boost::unit_test::depends_on("Core/Container/TVectorTestSuite/SimplePod_SmallMainTest"))
{
	TVector<int> V;
	const int BIG_LENGTH = 100;
	// Adding elements (probably will grow)
	for (int i = 0; i < BIG_LENGTH; i++) { V.Add(i + 1); }
	// Check that we properly added elements 
	for (int i = 0; i < V.Len(); i++)
	{
		BOOST_REQUIRE_EQUAL(V[i], i + 1);
	}
	BOOST_REQUIRE_EQUAL(V.Len(), BIG_LENGTH);
	BOOST_REQUIRE(V.MaxLen() >= V.Len());
	BOOST_REQUIRE(V.Data() != nullptr);

	constexpr int LESSER_BIG_LENGTH = 1;
	while (V.Len() > LESSER_BIG_LENGTH)
	{
		V.Pop();
	}

	V.ShrinkToFit();
	BOOST_REQUIRE_EQUAL(V.Len(), LESSER_BIG_LENGTH);
	BOOST_REQUIRE(V.MaxLen() >= V.Len());
	BOOST_REQUIRE(V.Data() != nullptr);
	for (int i = 0; i < V.Len(); i++)
	{
		BOOST_REQUIRE_EQUAL(V[i], i + 1);
	}
}

BOOST_AUTO_TEST_CASE(SimplePod_SmallSetLength, *boost::unit_test::depends_on("Core/Container/TVectorTestSuite/SimplePod_SmallMainTest"))
{
	TVector<int> V;
	constexpr int L = 5;

	BOOST_TEST_CHECKPOINT("SetLength to zero on empty");
	V.SetLength(0);
	BOOST_REQUIRE_EQUAL(V.Len(), 0);
	BOOST_REQUIRE(V.MaxLen() >= 0);

	BOOST_TEST_CHECKPOINT("SetLength on empty");
	V.SetLength(L);
	BOOST_REQUIRE_EQUAL(V.Len(), L);
	BOOST_REQUIRE(V.MaxLen() >= L);
	BOOST_REQUIRE(V.Data() != nullptr);
	for (int i = 0; i < L; i++) { V[i] = i + 1; }

	BOOST_TEST_CHECKPOINT("SetLength greater");
	constexpr int greaterL = 7;
	V.SetLength(greaterL);
	BOOST_REQUIRE_EQUAL(V.Len(), greaterL);
	BOOST_REQUIRE(V.MaxLen() >= greaterL);
	// Check that elements below the range are NOT touched
	for (int i = 0; i < L; i++) { BOOST_REQUIRE_EQUAL(V[i], (i + 1)); }
	// Check that elements over the range are properly initialized with zero
	for (int i = L; i < V.Len(); i++) { BOOST_REQUIRE_EQUAL(V[i], 0); }
	// Fill again: to prepare for the next test
	for (int i = 0; i < V.Len(); i++) { V[i] = i + 1; }

	BOOST_TEST_CHECKPOINT("SetLength less on non-empty");
	const int shorter_L = 3;
	V.SetLength(shorter_L);
	BOOST_REQUIRE_EQUAL(shorter_L, V.Len());
	BOOST_REQUIRE(V.MaxLen() >= V.MaxLen());
	BOOST_REQUIRE(V.Data() != nullptr);
	for (int i = 0; i < shorter_L; i++)
	{
		BOOST_REQUIRE_EQUAL(V[i], (i + 1));
	}
}

BOOST_AUTO_TEST_SUITE_END() // TVectorTestSuite
BOOST_AUTO_TEST_SUITE_END() // Container
BOOST_AUTO_TEST_SUITE_END() // Core

} 