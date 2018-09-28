#pragma once

#include "TVectorTestUtils.h"

namespace
{

BOOST_AUTO_TEST_SUITE(Core)
BOOST_AUTO_TEST_SUITE(Container)
BOOST_AUTO_TEST_SUITE(TVectorTestSuite)

BOOST_AUTO_TEST_CASE(SimplePod_SmallMainTest)
{
	TVector<int> V;
	BOOST_REQUIRE(V.Len() <= V.MaxLen());
	BOOST_REQUIRE_EQUAL(V.Len(), 0);
	BOOST_REQUIRE_EQUAL(V.Empty(), true);

	BOOST_TEST_CHECKPOINT("Add");
	const int added_value = 15;
	const int* ptr = V.Add(added_value);
	BOOST_REQUIRE(ptr != nullptr);
	BOOST_REQUIRE_EQUAL(*ptr, added_value);
	BOOST_REQUIRE_EQUAL(V.Len(), 1);
	BOOST_REQUIRE_EQUAL(V.Empty(), false);
	BOOST_REQUIRE_EQUAL(V[0], added_value);
	BOOST_REQUIRE_EQUAL(V.Last(), added_value);
	BOOST_REQUIRE_EQUAL(V.LastIndex(), 0);
	BOOST_REQUIRE_EQUAL(V.Top(), added_value);
	BOOST_REQUIRE(V.Data() != nullptr);
	BOOST_REQUIRE_EQUAL(*V.Data(), added_value);

	BOOST_TEST_CHECKPOINT("ShrinkToFit");
	V.ShrinkToFit();
	BOOST_REQUIRE_EQUAL(V.Len(), 1);

	BOOST_TEST_CHECKPOINT("Add move");
	const int CONST_added_value_for_move = 7;
	int added_value_for_move = CONST_added_value_for_move;
	const int* ptr2 = V.Add(std::move(added_value_for_move));// WARNING
	BOOST_REQUIRE_EQUAL(V.Len(), 2);
	BOOST_REQUIRE_EQUAL(V.Last(), CONST_added_value_for_move);
	BOOST_REQUIRE_EQUAL(V.LastIndex(), 1);
	BOOST_REQUIRE(ptr2 != nullptr);
	BOOST_REQUIRE_EQUAL(*ptr2, CONST_added_value_for_move);

	BOOST_TEST_CHECKPOINT("Emplace");
	const int CONST_value_for_emplace = 3;
	const int* ptr3 = V.Emplace(CONST_value_for_emplace);
	BOOST_REQUIRE_EQUAL(V.Len(), 3);
	BOOST_REQUIRE(ptr3 != nullptr);
	BOOST_REQUIRE_EQUAL(*ptr3, CONST_value_for_emplace);

	BOOST_TEST_CHECKPOINT("AddUninitialized");
	const int CONST_value_add_uninitialized = 5;
	int* ptr4 = V.AddUninitialized();
	BOOST_REQUIRE_EQUAL(V.Len(), 4);
	BOOST_REQUIRE(ptr4 != nullptr);
	*ptr4 = CONST_value_add_uninitialized;

	BOOST_TEST_CHECKPOINT("Insert");
	const int CONST_value_insert = 8;
	const int CONST_position_insert = 1;
	int* ptr5_inserted = V.Insert(CONST_position_insert, CONST_value_insert);
	BOOST_REQUIRE_EQUAL(V.Len(), 5);
	BOOST_REQUIRE(ptr5_inserted != nullptr);
	BOOST_REQUIRE_EQUAL(*ptr5_inserted, CONST_value_insert);
	// Checking that properly shifted
	BOOST_REQUIRE_EQUAL(V[0], added_value);
	BOOST_REQUIRE_EQUAL(V[CONST_position_insert], CONST_value_insert);
	BOOST_REQUIRE_EQUAL(V[2], CONST_added_value_for_move);
	BOOST_REQUIRE_EQUAL(V[3], CONST_value_for_emplace);
	BOOST_REQUIRE_EQUAL(V[4], CONST_value_add_uninitialized);

	BOOST_TEST_CHECKPOINT("Insert from array");
	const int CONST_insert_arr[2]{ 1, 9 };
	const int CONST_insert_arr_size = 2;
	const int CONST_insert_arr_position = 4;
	int* ptr67_inserted = V.Insert(CONST_insert_arr, CONST_insert_arr_size, CONST_insert_arr_position);
	BOOST_REQUIRE(ptr67_inserted != nullptr);
	BOOST_REQUIRE_EQUAL(ptr67_inserted[0], CONST_insert_arr[0]);
	BOOST_REQUIRE_EQUAL(ptr67_inserted[1], CONST_insert_arr[1]);
	BOOST_REQUIRE_EQUAL(V.Len(), 7);
	BOOST_REQUIRE_EQUAL(V[0], added_value);
	BOOST_REQUIRE_EQUAL(V[CONST_position_insert], CONST_value_insert);
	BOOST_REQUIRE_EQUAL(V[2], CONST_added_value_for_move);
	BOOST_REQUIRE_EQUAL(V[3], CONST_value_for_emplace);
	BOOST_REQUIRE_EQUAL(V[4], CONST_insert_arr[0]);
	BOOST_REQUIRE_EQUAL(V[5], CONST_insert_arr[1]);
	BOOST_REQUIRE_EQUAL(V[6], CONST_value_add_uninitialized);

	BOOST_TEST_CHECKPOINT("RemoveAt");
	const int CONST_remove_position = 4;
	V.RemoveAt(CONST_remove_position);
	BOOST_REQUIRE_EQUAL(V.Len(), 6);
	BOOST_REQUIRE_EQUAL(V[0], added_value);
	BOOST_REQUIRE_EQUAL(V[CONST_position_insert], CONST_value_insert);
	BOOST_REQUIRE_EQUAL(V[2], CONST_added_value_for_move);
	BOOST_REQUIRE_EQUAL(V[3], CONST_value_for_emplace);
	BOOST_REQUIRE_EQUAL(V[4], CONST_insert_arr[1]);
	BOOST_REQUIRE_EQUAL(V[5], CONST_value_add_uninitialized);

	BOOST_TEST_CHECKPOINT("Pop");
	int popped = V.Pop();
	BOOST_REQUIRE_EQUAL(popped, CONST_value_add_uninitialized);
	BOOST_REQUIRE_EQUAL(V.Len(), 5);

	BOOST_TEST_CHECKPOINT("Clear");
	V.Clear();
	BOOST_REQUIRE_EQUAL(V.Empty(), true);
}

BOOST_AUTO_TEST_CASE(SimplePod_Insert, *boost::unit_test::depends_on("Core/Container/TVectorTestSuite/SimplePod_SmallMainTest"))
{
	BOOST_TEST_CHECKPOINT("Insert empty");
	TVector<int> V_empty;
	V_empty.Insert(0, { 1, 2 });
	BOOST_REQUIRE_EQUAL(V_empty.Len(), 2);
	BOOST_REQUIRE_EQUAL(V_empty[0], 1);
	BOOST_REQUIRE_EQUAL(V_empty[1], 2);

	BOOST_TEST_CHECKPOINT("Insert back to one");
	TVector<int> V_one;
	V_one.Add(1);
	V_one.Insert(V_one.Len(), { 2, 3 });
	BOOST_REQUIRE_EQUAL(V_one.Len(), 3);
	BOOST_REQUIRE_EQUAL(V_one[1], 2);
	BOOST_REQUIRE_EQUAL(V_one[2], 3);

	BOOST_TEST_CHECKPOINT("Insert one element back with capacity");
	TVector<int> v_insertGrow;
	v_insertGrow.SetBufferLengthUninitialized(v_insertGrow.MaxLen());
	const int32_t oldLen = v_insertGrow.Len();
	v_insertGrow.Insert(v_insertGrow.Len(), 1);
	BOOST_REQUIRE_EQUAL(v_insertGrow.Len(), oldLen + 1);
	BOOST_REQUIRE_EQUAL(v_insertGrow.Last(), 1);
}

BOOST_AUTO_TEST_SUITE_END() // TVectorTestSuite
BOOST_AUTO_TEST_SUITE_END() // Container
BOOST_AUTO_TEST_SUITE_END() // Core
} 