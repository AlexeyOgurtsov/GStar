#pragma once

#include "TVectorTestUtils.h"

namespace
{

BOOST_AUTO_TEST_SUITE(Core)
BOOST_AUTO_TEST_SUITE(Container)
BOOST_AUTO_TEST_SUITE(TVectorTestSuite)


BOOST_AUTO_TEST_CASE(SimplePod_SmallConstructInitialSize, *boost::unit_test::depends_on("Core/Container/TVectorTestSuite/SimplePod_SmallMainTest"))
{
	TVector<int> V(5);
	BOOST_REQUIRE_EQUAL(V.Len(), 5);
	const int AssignValue = 3;
	const int AssignIndex = 4;
	V[AssignIndex] = AssignValue;
	BOOST_REQUIRE_EQUAL(V[AssignIndex], AssignValue);
	// WARNING!!! We should NOT check here, that the elements are initialized,
	// because the construct should NOT initialize elements.
}

BOOST_AUTO_TEST_CASE(SimplePod_SmallConstructInitialSize_ForceInit, *boost::unit_test::depends_on("Core/Container/TVectorTestSuite/SimplePod_SmallMainTest"))
{
	TVector<int> V(5);
	BOOST_REQUIRE_EQUAL(V.Len(), 5);
	BOOST_REQUIRE_EQUAL(V[0], 0);
	BOOST_REQUIRE_EQUAL(V[1], 0);
	BOOST_REQUIRE_EQUAL(V[2], 0);
	BOOST_REQUIRE_EQUAL(V[3], 0);
	BOOST_REQUIRE_EQUAL(V[4], 0);
}

BOOST_AUTO_TEST_CASE(SimplePod_SmallConstructTempl, *boost::unit_test::depends_on("Core/Container/TVectorTestSuite/SimplePod_SmallMainTest"))
{
	const int templValue = 7;
	TVector<int> V(5, templValue);
	BOOST_REQUIRE_EQUAL(V.Len(), 5);
	BOOST_REQUIRE_EQUAL(V[0], templValue);
	BOOST_REQUIRE_EQUAL(V[1], templValue);
	BOOST_REQUIRE_EQUAL(V[2], templValue);
	BOOST_REQUIRE_EQUAL(V[3], templValue);
	BOOST_REQUIRE_EQUAL(V[4], templValue);
}

BOOST_AUTO_TEST_CASE(SimplePod_SmallConstructInitializerList, *boost::unit_test::depends_on("Core/Container/TVectorTestSuite/SimplePod_SmallMainTest"))
{
	TVector<int> V{ 1, 2, 3, 4, 5 };
	BOOST_REQUIRE_EQUAL(V.Len(), 5);
	BOOST_REQUIRE_EQUAL(V[0], 1);
	BOOST_REQUIRE_EQUAL(V[1], 2);
	BOOST_REQUIRE_EQUAL(V[2], 3);
	BOOST_REQUIRE_EQUAL(V[3], 4);
	BOOST_REQUIRE_EQUAL(V[4], 5);
}

BOOST_AUTO_TEST_CASE(SimplePod_SmallConstructFromArray, *boost::unit_test::depends_on("Core/Container/TVectorTestSuite/SimplePod_SmallMainTest"))
{
	constexpr int THE_ARRAY_SIZE = 5;
	const int THE_ARRAY[THE_ARRAY_SIZE]{ 1, 2, 3, 4, 5 };
	TVector<int> V(THE_ARRAY, THE_ARRAY_SIZE);
	BOOST_REQUIRE_EQUAL(V.Len(), 5);
	BOOST_REQUIRE_EQUAL(V[0], 1);
	BOOST_REQUIRE_EQUAL(V[1], 2);
	BOOST_REQUIRE_EQUAL(V[2], 3);
	BOOST_REQUIRE_EQUAL(V[3], 4);
	BOOST_REQUIRE_EQUAL(V[4], 5);
}


BOOST_AUTO_TEST_CASE
(
	SimplePod_SmallCopyConstruction,
	*boost::unit_test::depends_on("Core/Container/TVectorTestSuite/SimplePod_SmallMainTest")
	*boost::unit_test::depends_on("Core/Container/TVectorTestSuite/SimplePod_SmallEqualityTest")
)
{
	BOOST_TEST_CHECKPOINT("Source array");
	TVector<int> V_source{ 1, 2, 3, 4, 5 };

	BOOST_TEST_CHECKPOINT("Destination array");
	TVector<int> V(V_source);
	BOOST_REQUIRE_EQUAL(V.Len(), V_source.Len());
	BOOST_REQUIRE_EQUAL(V_source, V);
}

BOOST_AUTO_TEST_CASE
(
	SimplePod_SmallMoveConstruction,
	*boost::unit_test::depends_on("Core/Container/TVectorTestSuite/SimplePod_SmallMainTest")
	*boost::unit_test::depends_on("Core/Container/TVectorTestSuite/SimplePod_SmallEqualityTest")
)
{
	BOOST_TEST_CHECKPOINT("Source array");
	TVector<int> V_source1{ 1, 2, 3, 4, 5 };
	TVector<int> V_source2{ 1, 2, 3, 4, 5 };

	BOOST_TEST_CHECKPOINT("Destination array");
	TVector<int> V(std::move(V_source1));
	BOOST_REQUIRE_EQUAL(V.Len(), V_source2.Len());
	BOOST_REQUIRE_EQUAL(V_source2, V);
}

BOOST_AUTO_TEST_CASE
(
	SimplePod_SmallCopyAssign,
	*boost::unit_test::depends_on("Core/Container/TVectorTestSuite/SimplePod_SmallMainTest")
	*boost::unit_test::depends_on("Core/Container/TVectorTestSuite/SimplePod_SmallEqualityTest")
)
{
	BOOST_TEST_CHECKPOINT("Source array");
	TVector<int> V_source{ 1, 2, 3, 4, 5 };

	BOOST_TEST_CHECKPOINT("Copy to empty vector");
	TVector<int> V_assign_to_empty;
	V_assign_to_empty = V_source;
	BOOST_REQUIRE_EQUAL(V_assign_to_empty, V_source);

	BOOST_TEST_CHECKPOINT("Copy to lesser vector");
	TVector<int> V_assign_to_3length{ 9, 6, 8 };
	V_assign_to_3length = V_source;
	BOOST_REQUIRE_EQUAL(V_assign_to_3length, V_source);

	BOOST_TEST_CHECKPOINT("Copy to greater vector");
	TVector<int> V_assign_to_7length{ 9, 6, 8, 7, 6, 5, 4 };
	V_assign_to_7length = V_source;
	BOOST_REQUIRE_EQUAL(V_assign_to_7length, V_source);
}

BOOST_AUTO_TEST_CASE
(
	SimplePod_MoveAssign_FromBigDynamic,
	*boost::unit_test::depends_on("Core/Container/TVectorTestSuite/SimplePod_SmallMainTest")
	*boost::unit_test::depends_on("Core/Container/TVectorTestSuite/SimplePod_SmallEqualityTest")
)
{
	BOOST_TEST_CHECKPOINT("Source arrays");
	constexpr int32_t COUNT_SOURCE_ARRAYS = 10;
	constexpr int32_t SOURCE_ARRAY_LEN = 100;
	TVector<int> referenceArray;
	TVector<int> sourceArrays[COUNT_SOURCE_ARRAYS];
	for (int sourceLen = 0; sourceLen < SOURCE_ARRAY_LEN; sourceLen++)
	{
		referenceArray.Add(sourceLen + 1);
	}
	for (int sourceIndex = 0; sourceIndex < COUNT_SOURCE_ARRAYS; sourceIndex++)
	{
		for (int sourceLen = 0; sourceLen < SOURCE_ARRAY_LEN; sourceLen++)
		{
			sourceArrays[sourceIndex].Add(sourceLen + 1);
		}
	}

	BOOST_TEST_CHECKPOINT("Move to empty");
	TVector<int> V_assign_to_empty;
	V_assign_to_empty = std::move(sourceArrays[0]);
	BOOST_REQUIRE_EQUAL(V_assign_to_empty.Len(), SOURCE_ARRAY_LEN);
	BOOST_REQUIRE_EQUAL(referenceArray, V_assign_to_empty);
	BOOST_REQUIRE(false == sourceArrays[0].IsValid());

	BOOST_TEST_CHECKPOINT("Move to small vector");
	TVector<int> V_assign_to_small{ 1, 2, 3 };
	V_assign_to_small = std::move(sourceArrays[1]);
	BOOST_REQUIRE_EQUAL(V_assign_to_small.Len(), SOURCE_ARRAY_LEN);
	BOOST_REQUIRE_EQUAL(referenceArray, V_assign_to_small);
	BOOST_REQUIRE(false == sourceArrays[1].IsValid());

	BOOST_TEST_CHECKPOINT("Move to dynamic vector");
	TVector<int> V_assign_to_dynamic_lesser;
	for (int i = 0; i < 60; i++)
	{
		V_assign_to_dynamic_lesser.Add(9);
	}
	V_assign_to_dynamic_lesser = std::move(sourceArrays[2]);
	BOOST_REQUIRE_EQUAL(V_assign_to_dynamic_lesser.Len(), SOURCE_ARRAY_LEN);
	BOOST_REQUIRE_EQUAL(referenceArray, V_assign_to_dynamic_lesser);
	BOOST_REQUIRE(false == sourceArrays[2].IsValid());
}

BOOST_AUTO_TEST_CASE
(
	SimplePod_SmallMoveAssign,
	*boost::unit_test::depends_on("Core/Container/TVectorTestSuite/SimplePod_SmallMainTest")
	*boost::unit_test::depends_on("Core/Container/TVectorTestSuite/SimplePod_SmallEqualityTest")
)
{
	// WARNING!!! We should NOT check that the vector is invalidated after move, because
	// it's not always the case (not a requirement).

	BOOST_TEST_CHECKPOINT("Source arrays");
	TVector<int> V_source1{ 1, 2, 3, 4, 5 };
	TVector<int> V_source2{ 1, 2, 3, 4, 5 };
	TVector<int> V_source3{ 1, 2, 3, 4, 5 };
	TVector<int> V_source4{ 1, 2, 3, 4, 5 };
	TVector<int> V_source5{ 1, 2, 3, 4, 5 };

	BOOST_TEST_CHECKPOINT("Copy to empty vector");
	TVector<int> V_assign_to_empty;
	V_assign_to_empty = std::move(V_source1);
	BOOST_REQUIRE_EQUAL(V_assign_to_empty, V_source2);

	BOOST_TEST_CHECKPOINT("Copy to lesser vector");
	TVector<int> V_assign_to_3length{ 9, 6, 8 };
	V_assign_to_3length = std::move(V_source2);
	BOOST_REQUIRE_EQUAL(V_assign_to_3length, V_source3);

	BOOST_TEST_CHECKPOINT("Copy to greater vector");
	TVector<int> V_assign_to_7length{ 9, 6, 8, 7, 6, 5, 4 };
	V_assign_to_7length = std::move(V_source3);
	BOOST_REQUIRE_EQUAL(V_assign_to_7length, V_source4);

	BOOST_TEST_CHECKPOINT("Copy to dynamic vector");
	TVector<int> V_assign_to_big_dynamic;
	V_assign_to_big_dynamic.AddZeroed(100);
	V_assign_to_big_dynamic = std::move(V_source4);
	BOOST_REQUIRE_EQUAL(V_assign_to_big_dynamic, V_source5);
}

BOOST_AUTO_TEST_SUITE_END() // TVectorTestSuite
BOOST_AUTO_TEST_SUITE_END() // Container
BOOST_AUTO_TEST_SUITE_END() // Core

} 