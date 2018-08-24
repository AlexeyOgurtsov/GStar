#pragma once

#include "Core/Cont/TVector.h"
#include <boost/test/included/unit_test.hpp>
#include <memory> // std::unique_ptr
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <string>

namespace
{

BOOST_AUTO_TEST_SUITE(Core)
BOOST_AUTO_TEST_SUITE(Container)
BOOST_AUTO_TEST_SUITE(TVectorTestSuite)

BOOST_AUTO_TEST_SUITE(IterationSuite)

BOOST_AUTO_TEST_CASE
(
	ReverseIteratorMath,
	*boost::unit_test::depends_on("Core/Container/TVectorTestSuite/IterationSuite/ReverseIterationTest")
)
{
	BOOST_TEST_CHECKPOINT("Initialization");
	TVector<int> V;
	for (int i = 0; i < 10; i++)
	{
		V.Add(i);
	}

	BOOST_TEST_CHECKPOINT("Addition test");
	TVector<int>::BackwardIteratorType const ItFirstElement = V.BackwardIterator() + (V.Len() - 1);
	BOOST_REQUIRE(ItFirstElement == V.Iterator());

	BOOST_REQUIRE_EQUAL(V.BackwardIterator() + 0, V.BackwardIterator());
	BOOST_REQUIRE_EQUAL(*(V.BackwardIterator() + 2), *(V.Iterator() + (V.Len() - 1) - 2));
	BOOST_REQUIRE_EQUAL(*(V.BackwardIterator() + 3), *(V.Iterator() + (V.Len() - 1) - 3));

	BOOST_TEST_CHECKPOINT("Addition end test");
	BOOST_REQUIRE( ! (V.BackwardIterator() + V.Len()) );
	BOOST_REQUIRE( ! ((V.BackwardIterator() + V.Len()) + 0));

	BOOST_TEST_CHECKPOINT("Subtraction test");
	BOOST_REQUIRE_EQUAL(ItFirstElement - 0, ItFirstElement);
	BOOST_REQUIRE_EQUAL(V.BackwardIterator() - 0, V.BackwardIterator());
	BOOST_REQUIRE_EQUAL(*(ItFirstElement - 2), *(V.Iterator() + 2));
	BOOST_REQUIRE_EQUAL(*(ItFirstElement - 3), *(V.Iterator() + 3));
}

BOOST_AUTO_TEST_CASE
(
	ReverseIterationDecrementTest,
	*boost::unit_test::depends_on("Core/Container/TVectorTestSuite/IterationSuite/ReverseIterationTest")
)
{
	BOOST_TEST_CHECKPOINT("Initialization");
	TVector<int> V;
	for (int i = 0; i < 10; i++)
	{
		V.Add(i);
	}

	BOOST_TEST_CHECKPOINT("Iteration");
	{
		int RefIndex = 0;

		TVector<int>::BackwardIteratorType ItInitial = V.BackwardIterator() + (V.Len() - 1);

		TVector<int>::BackwardIteratorType It = ItInitial;
		for (; ; --It)
		{
			BOOST_REQUIRE(!It.IsEnd());
			BOOST_REQUIRE(!!It);
			BOOST_REQUIRE_EQUAL(It.ToBaseIterator().Index(), RefIndex);
			BOOST_REQUIRE_EQUAL(It.Get(), V[RefIndex]);
			BOOST_REQUIRE_EQUAL(*It, V[RefIndex]);
			BOOST_REQUIRE(nullptr != It.GetPtr());
			BOOST_REQUIRE_EQUAL(*It.GetPtr(), V[RefIndex]);

			RefIndex++;

			if (It == V.BackwardIterator())
			{
				break;
			}
		}
		BOOST_REQUIRE(It == V.BackwardIterator());
	}
}

BOOST_AUTO_TEST_CASE
(
	ReverseIterationTest,
	*boost::unit_test::depends_on("Core/Container/TVectorTestSuite/IterationSuite/SimpleIterationTest")
)
{
	BOOST_TEST_CHECKPOINT("Initialization");
	TVector<int> V;
	for (int i = 0; i < 10; i++)
	{
		V.Add(i);
	}

	BOOST_TEST_CHECKPOINT("Iteration");
	{
		int RefIndex = V.LastIndex();
		for (TVector<int>::BackwardIteratorType It = V.BackwardIterator(); It; ++It)
		{
			BOOST_REQUIRE(!It.IsEnd());
			BOOST_REQUIRE(!!It);
			BOOST_REQUIRE_EQUAL(It.ToBaseIterator().Index(), RefIndex);
			BOOST_REQUIRE_EQUAL(It.Get(), V[RefIndex]);
			BOOST_REQUIRE_EQUAL(*It, V[RefIndex]);
			BOOST_REQUIRE(nullptr != It.GetPtr());
			BOOST_REQUIRE_EQUAL(*It.GetPtr(), V[RefIndex]);

			RefIndex--;
		}
	}
}

BOOST_AUTO_TEST_CASE(SimpleIterationTest)
{
	BOOST_TEST_CHECKPOINT("Initialization");
	TVector<int> V;
	for (int i = 0; i < 10; i++)
	{
		V.Add(i);
	}

	BOOST_TEST_CHECKPOINT("Iteration");
	{
		int RefIndex = 0;
		for (TVector<int>::IteratorType It = V.Iterator(); It; ++It)
		{
			BOOST_REQUIRE(!It.IsEnd());
			BOOST_REQUIRE(!!It);
			BOOST_REQUIRE_EQUAL(It.Index(), RefIndex);
			BOOST_REQUIRE_EQUAL(It.Get(), V[RefIndex]);
			BOOST_REQUIRE_EQUAL(*It, V[RefIndex]);
			BOOST_REQUIRE(nullptr != It.GetPtr());
			BOOST_REQUIRE_EQUAL(*It.GetPtr(), V[RefIndex]);

			RefIndex++;
		}
	}

	BOOST_TEST_CHECKPOINT("Equality");
	{
		BOOST_REQUIRE(V.Iterator() == V.Iterator());
		TVector<int>::IteratorType ItCurr = V.Iterator();
		++ItCurr;
		BOOST_REQUIRE(V.Iterator() != ItCurr);

		TVector<int>::IteratorType ItEnd = V.Iterator();
		while (++ItEnd) {}
		BOOST_REQUIRE(ItEnd == ItEnd);
		BOOST_REQUIRE(ItCurr != ItEnd);
	}

	BOOST_TEST_CHECKPOINT("Copy-construction");
	TVector<int>::IteratorType ItCopy = V.Iterator();
	BOOST_REQUIRE(ItCopy == V.Iterator());

	BOOST_TEST_CHECKPOINT("Copy-assignment");
	ItCopy = V.Iterator();
	BOOST_REQUIRE(ItCopy == V.Iterator());

	BOOST_TEST_CHECKPOINT("Move-construction");
	TVector<int>::IteratorType ItMovedCopy = std::move(V.Iterator());
	BOOST_REQUIRE(ItMovedCopy == V.Iterator());

	BOOST_TEST_CHECKPOINT("Move-assignment");
	ItMovedCopy = std::move(V.Iterator());
	BOOST_REQUIRE(ItMovedCopy == V.Iterator());

	BOOST_TEST_CHECKPOINT("Add");
	TVector<int>::IteratorType const AdvancedIt = V.Iterator() + V.Len();
	TVector<int>::IteratorType FowardedIt = V.Iterator();

	FowardedIt.Forward(0);
	BOOST_REQUIRE(FowardedIt == V.Iterator());

	FowardedIt.Forward(1);
	BOOST_REQUIRE_EQUAL(*FowardedIt, V[1]);

	FowardedIt.Forward(V.Len() - 2);
	BOOST_REQUIRE_EQUAL(*FowardedIt, V[V.LastIndex()]);
	FowardedIt.Forward(1);
	BOOST_REQUIRE( ! FowardedIt );

	BOOST_REQUIRE( ! AdvancedIt );
	BOOST_REQUIRE( V.Iterator() + 0 == V.Iterator());
	BOOST_REQUIRE_EQUAL(*(V.Iterator() + 1), V[1]);
	BOOST_REQUIRE_EQUAL(*(V.Iterator() + 2), V[2]);

	BOOST_TEST_CHECKPOINT("Subtract");
	BOOST_REQUIRE(V.Iterator() - 0 == V.Iterator());
	BOOST_REQUIRE(AdvancedIt - 0 == AdvancedIt);
	BOOST_REQUIRE(*(AdvancedIt - V.Len()) == V.First());
	BOOST_REQUIRE(*(AdvancedIt - 1) == V.Last(0));

	BOOST_REQUIRE((AdvancedIt - 1).AtLast());

	BOOST_TEST_CHECKPOINT("BackwardIteration");
	{
		int RefIndex = V.LastIndex();
		for (TVector<int>::IteratorType It = V.Iterator() + RefIndex; ; It--)
		{			
			BOOST_REQUIRE(!It.IsEnd());
			BOOST_REQUIRE(!!It);
			BOOST_REQUIRE_EQUAL(It.Index(), RefIndex);
			BOOST_REQUIRE_EQUAL(It.Get(), V[RefIndex]);
			BOOST_REQUIRE_EQUAL(*It, V[RefIndex]);
			BOOST_REQUIRE(nullptr != It.GetPtr());
			BOOST_REQUIRE_EQUAL(*It.GetPtr(), V[RefIndex]);

			if (It.AtStart())
			{
				break;
			}

			RefIndex--;
		}
	}

	BOOST_TEST_CHECKPOINT("ConstIteration");
	{
		int RefIndex = 0;
		for (TVector<int>::ConstIteratorType It = V.ConstIterator(); It; ++It)
		{
			BOOST_REQUIRE(!It.IsEnd());
			BOOST_REQUIRE(!!It);
			BOOST_REQUIRE_EQUAL(It.Index(), RefIndex);
			BOOST_REQUIRE_EQUAL(It.Get(), V[RefIndex]);
			BOOST_REQUIRE_EQUAL(*It, V[RefIndex]);
			BOOST_REQUIRE(nullptr != It.GetPtr());
			BOOST_REQUIRE_EQUAL(*It.GetPtr(), V[RefIndex]);

			RefIndex++;
		}
	}

	BOOST_TEST_CHECKPOINT("Updating by iterator");
}

BOOST_AUTO_TEST_SUITE_END() // IterationSuite

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
BOOST_AUTO_TEST_CASE(SimplePod_SmallEqualityTest, *boost::unit_test::depends_on("Core/Container/TVectorTestSuite/SimplePod_SmallMainTest"))
{
	TVector<int> V_left;
	TVector<int> V_right;
	BOOST_TEST_CHECKPOINT("EqualityTest_Empty");
	BOOST_REQUIRE(V_left == V_right);

	BOOST_TEST_CHECKPOINT("NonEqualityTest_1_and_empty");
	V_left.Add(1);
	BOOST_REQUIRE(V_left != V_right);
	BOOST_REQUIRE(V_right != V_left);

	BOOST_TEST_CHECKPOINT("EqualityTest_1_and_1");
	V_right.Add(1);
	BOOST_REQUIRE(V_left == V_right);
	BOOST_REQUIRE(V_right == V_left);

	BOOST_TEST_CHECKPOINT("NonEqualityTest_1_and_1_2");
	V_right.Add(2);
	BOOST_REQUIRE(V_left != V_right);
	BOOST_REQUIRE(V_right != V_left);

	BOOST_TEST_CHECKPOINT("EqualityTest_1_2_and_1_2");
	V_left.Add(2);
	BOOST_REQUIRE(V_left == V_right);
	BOOST_REQUIRE(V_right == V_left);
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

BOOST_AUTO_TEST_CASE(SimplePod_RemoveAtSwap, *boost::unit_test::depends_on("Core/Container/TVectorTestSuite/SimplePod_SmallMainTest"))
{
	BOOST_TEST_CHECKPOINT("Remove single last element");
	TVector<int> V_1;
	V_1.Add(1);
	V_1.RemoveAtSwap(0);
	BOOST_REQUIRE_EQUAL(V_1.Len(), 0);

	BOOST_TEST_CHECKPOINT("Remove last element");
	TVector<int> V_12_remove_last;
	V_12_remove_last.Add(1);
	V_12_remove_last.Add(2);
	V_12_remove_last.RemoveAtSwap(1);
	BOOST_REQUIRE_EQUAL(V_12_remove_last.Len(), 1);
	BOOST_REQUIRE_EQUAL(V_12_remove_last[0], 1);

	BOOST_TEST_CHECKPOINT("Remove first element of two");
	TVector<int> V_12_remove_first;
	V_12_remove_first.Add(1);
	V_12_remove_first.Add(2);
	V_12_remove_first.RemoveAtSwap(0);
	BOOST_REQUIRE_EQUAL(V_12_remove_first.Len(), 1);
	BOOST_REQUIRE(V_12_remove_first.Contains(2));

	BOOST_TEST_CHECKPOINT("Remove middle element of five");
	TVector<int> V_12345_remove_middle;
	V_12345_remove_middle.Add(1);
	V_12345_remove_middle.Add(2);
	V_12345_remove_middle.Add(3);
	V_12345_remove_middle.Add(4);
	V_12345_remove_middle.Add(5);
	V_12345_remove_middle.RemoveAtSwap(2);
	BOOST_REQUIRE_EQUAL(V_12345_remove_middle.Len(), 4);
	BOOST_REQUIRE(V_12345_remove_middle.Contains(1));
	BOOST_REQUIRE(V_12345_remove_middle.Contains(2));
	BOOST_REQUIRE(V_12345_remove_middle.Contains(4));
	BOOST_REQUIRE(V_12345_remove_middle.Contains(5));

	BOOST_TEST_CHECKPOINT("Remove middle 3 elements");
	TVector<int> V_123456789_remove_middle3;
	for (int i = 0; i < 9; i++) { V_123456789_remove_middle3.Add(i + 1); }
	V_123456789_remove_middle3.RemoveAtSwap(3, 3);
	BOOST_REQUIRE_EQUAL(V_123456789_remove_middle3.Len(), 6);
	BOOST_REQUIRE(V_123456789_remove_middle3.Contains(1));
	BOOST_REQUIRE(V_123456789_remove_middle3.Contains(2));
	BOOST_REQUIRE(V_123456789_remove_middle3.Contains(3));
	BOOST_REQUIRE(V_123456789_remove_middle3.Contains(7));
	BOOST_REQUIRE(V_123456789_remove_middle3.Contains(8));
	BOOST_REQUIRE(V_123456789_remove_middle3.Contains(9));

	BOOST_TEST_CHECKPOINT("Remove end 3 elements");
	TVector<int> V_123456_remove_last3;
	for (int i = 0; i < 6; i++) { V_123456_remove_last3.Add(i + 1); }
	V_123456_remove_last3.RemoveAtSwap(3, 3);
	BOOST_REQUIRE_EQUAL(V_123456_remove_last3.Len(), 3);
	BOOST_REQUIRE(V_123456_remove_last3.Contains(1));
	BOOST_REQUIRE(V_123456_remove_last3.Contains(2));
	BOOST_REQUIRE(V_123456_remove_last3.Contains(3));
}

BOOST_AUTO_TEST_CASE(SimplePod_Remove, *boost::unit_test::depends_on("Core/Container/TVectorTestSuite/SimplePod_SmallMainTest"))
{
	BOOST_TEST_CHECKPOINT("Remove single last element");
	TVector<int> V_1;
	V_1.Add(1);
	V_1.RemoveAt(0);
	BOOST_REQUIRE_EQUAL(V_1.Len(), 0);

	BOOST_TEST_CHECKPOINT("Remove last element");
	TVector<int> V_12_remove_last;
	V_12_remove_last.Add(1);
	V_12_remove_last.Add(2);
	V_12_remove_last.RemoveAt(1);
	BOOST_REQUIRE_EQUAL(V_12_remove_last.Len(), 1);
	BOOST_REQUIRE_EQUAL(V_12_remove_last[0], 1);

	BOOST_TEST_CHECKPOINT("Remove first element of two");
	TVector<int> V_12_remove_first;
	V_12_remove_first.Add(1);
	V_12_remove_first.Add(2);
	V_12_remove_first.RemoveAt(0);
	BOOST_REQUIRE_EQUAL(V_12_remove_first.Len(), 1);
	BOOST_REQUIRE(V_12_remove_first.Contains(2));

	BOOST_TEST_CHECKPOINT("Remove middle 3 elements");
	TVector<int> V_123456789_remove_middle3;
	for (int i = 0; i < 9; i++) { V_123456789_remove_middle3.Add(i + 1); }
	V_123456789_remove_middle3.RemoveAt(3, 3);
	BOOST_REQUIRE_EQUAL(V_123456789_remove_middle3.Len(), 6);
	BOOST_REQUIRE_EQUAL(V_123456789_remove_middle3[0], 1);
	BOOST_REQUIRE_EQUAL(V_123456789_remove_middle3[1], 2);
	BOOST_REQUIRE_EQUAL(V_123456789_remove_middle3[2], 3);
	BOOST_REQUIRE_EQUAL(V_123456789_remove_middle3[3], 7);
	BOOST_REQUIRE_EQUAL(V_123456789_remove_middle3[4], 8);
	BOOST_REQUIRE_EQUAL(V_123456789_remove_middle3[5], 9);
}

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

BOOST_AUTO_TEST_CASE(SimplePod_ForEach, *boost::unit_test::depends_on("Core/Container/TVectorTestSuite/SimplePod_SmallMainTest"))
{
	BOOST_TEST_CHECKPOINT("ForEach const empty");
	const TVector<int> V_empty;
	int numItems_empty = 0;
	for (const auto& elem : V_empty) { numItems_empty++; }
	BOOST_REQUIRE_EQUAL(numItems_empty, 0);

	BOOST_TEST_CHECKPOINT("ForEach Non-empty");
	TVector<int> V{ 1, 2, 3 };
	int numItems = 0;
	for (auto& elem : V) { numItems++; }
	BOOST_REQUIRE_EQUAL(numItems, 3);
}

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

BOOST_AUTO_TEST_CASE(SimplePod_RemoveAll, *boost::unit_test::depends_on("Core/Container/TVectorTestSuite/SimplePod_SmallMainTest"))
{
	BOOST_TEST_CHECKPOINT("Empty");
	TVector<int> V_empty;
	int32_t count_fromEmpty = V_empty.RemoveAll(0);
	BOOST_REQUIRE_EQUAL(count_fromEmpty, 0);
	BOOST_REQUIRE_EQUAL(V_empty.Len(), 0);

	BOOST_TEST_CHECKPOINT("OneFound");
	TVector<int> V_oneFound;
	V_oneFound.Add(0);
	int32_t count_fromOneFound = V_oneFound.RemoveAll(0);
	BOOST_REQUIRE_EQUAL(count_fromOneFound, 1);
	BOOST_REQUIRE_EQUAL(V_oneFound.Len(), 0);

	BOOST_TEST_CHECKPOINT("OneNotFound");
	TVector<int> V_oneNotFound;
	V_oneNotFound.Add(1);
	int32_t count_fromOneNotFound = V_oneNotFound.RemoveAll(0);
	BOOST_REQUIRE_EQUAL(count_fromOneNotFound, 0);
	BOOST_REQUIRE_EQUAL(V_oneNotFound.Len(), 1);
	BOOST_REQUIRE_EQUAL(V_oneNotFound[0], 1);

	BOOST_TEST_CHECKPOINT("0102000");
	TVector<int> V_0102000;
	V_0102000.Add(0);
	V_0102000.Add(1);
	V_0102000.Add(0);
	V_0102000.Add(2);
	V_0102000.Add(0);
	V_0102000.Add(0);
	V_0102000.Add(0);
	int32_t count_0102000 = V_0102000.RemoveAll(0);
	BOOST_REQUIRE_EQUAL(count_0102000, 5);
	BOOST_REQUIRE_EQUAL(V_0102000.Len(), 2);
	BOOST_REQUIRE_EQUAL(V_0102000[0], 1);
	BOOST_REQUIRE_EQUAL(V_0102000[1], 2);

	BOOST_TEST_CHECKPOINT("123");
	TVector<int> V_123;
	for (int i = 0; i < 3; i++) { V_123.Add(i + 1); }
	int32_t count_123 = V_123.RemoveAll(0);
	BOOST_REQUIRE_EQUAL(count_123, 0);
	BOOST_REQUIRE_EQUAL(V_123.Len(), 3);
	BOOST_REQUIRE_EQUAL(V_123[0], 1);
	BOOST_REQUIRE_EQUAL(V_123[1], 2);
	BOOST_REQUIRE_EQUAL(V_123[2], 3);

	BOOST_TEST_CHECKPOINT("010002045006007");
	TVector<int> V_010002045006007;
	V_010002045006007.Add(0);
	V_010002045006007.Add(1);
	V_010002045006007.Add(0);
	V_010002045006007.Add(0);
	V_010002045006007.Add(0);
	V_010002045006007.Add(2);
	V_010002045006007.Add(0);
	V_010002045006007.Add(4);
	V_010002045006007.Add(5);
	V_010002045006007.Add(0);
	V_010002045006007.Add(0);
	V_010002045006007.Add(6);
	V_010002045006007.Add(0);
	V_010002045006007.Add(0);
	V_010002045006007.Add(7);
	int32_t count_010002045006007 = V_010002045006007.RemoveAll(0);
	BOOST_REQUIRE_EQUAL(count_010002045006007, 9);
	BOOST_REQUIRE_EQUAL(V_010002045006007.Len(), 6);
	BOOST_REQUIRE_EQUAL(V_010002045006007[0], 1);
	BOOST_REQUIRE_EQUAL(V_010002045006007[1], 2);
	BOOST_REQUIRE_EQUAL(V_010002045006007[2], 4);
	BOOST_REQUIRE_EQUAL(V_010002045006007[3], 5);
	BOOST_REQUIRE_EQUAL(V_010002045006007[4], 6);
	BOOST_REQUIRE_EQUAL(V_010002045006007[5], 7);

	BOOST_TEST_CHECKPOINT("12300405780");
	// 1. We have zeroes at the end;
	// 2. We start with non-zero
	TVector<int> V_12300405780;
	V_12300405780.Add(1);
	V_12300405780.Add(2);
	V_12300405780.Add(3);
	V_12300405780.Add(0);
	V_12300405780.Add(0);
	V_12300405780.Add(4);
	V_12300405780.Add(0);
	V_12300405780.Add(5);
	V_12300405780.Add(7);
	V_12300405780.Add(8);
	V_12300405780.Add(0);
	int32_t count_12300405780 = V_12300405780.RemoveAll(0);
	BOOST_REQUIRE_EQUAL(count_12300405780, 4);
	BOOST_REQUIRE_EQUAL(V_12300405780.Len(), 7);
	BOOST_REQUIRE_EQUAL(V_12300405780[0], 1);
	BOOST_REQUIRE_EQUAL(V_12300405780[1], 2);
	BOOST_REQUIRE_EQUAL(V_12300405780[2], 3);
	BOOST_REQUIRE_EQUAL(V_12300405780[3], 4);
	BOOST_REQUIRE_EQUAL(V_12300405780[4], 5);
	BOOST_REQUIRE_EQUAL(V_12300405780[5], 7);
	BOOST_REQUIRE_EQUAL(V_12300405780[6], 8);
}

BOOST_AUTO_TEST_CASE(SimplePod_RemoveAllPredicate, *boost::unit_test::depends_on("Core/Container/TVectorTestSuite/SimplePod_SmallMainTest"))
{
	BOOST_TEST_CHECKPOINT("Empty");
	auto pred = [](const int& val) { return val == 0; };
	TVector<int> V_empty;
	int32_t count_fromEmpty = V_empty.RemoveAllPredicate(pred);
	BOOST_REQUIRE_EQUAL(count_fromEmpty, 0);
	BOOST_REQUIRE_EQUAL(V_empty.Len(), 0);

	BOOST_TEST_CHECKPOINT("OneFound");
	TVector<int> V_oneFound;
	V_oneFound.Add(0);
	int32_t count_fromOneFound = V_oneFound.RemoveAllPredicate(pred);
	BOOST_REQUIRE_EQUAL(count_fromOneFound, 1);
	BOOST_REQUIRE_EQUAL(V_oneFound.Len(), 0);

	BOOST_TEST_CHECKPOINT("OneNotFound");
	TVector<int> V_oneNotFound;
	V_oneNotFound.Add(1);
	int32_t count_fromOneNotFound = V_oneNotFound.RemoveAllPredicate(pred);
	BOOST_REQUIRE_EQUAL(count_fromOneNotFound, 0);
	BOOST_REQUIRE_EQUAL(V_oneNotFound.Len(), 1);
	BOOST_REQUIRE_EQUAL(V_oneNotFound[0], 1);

	BOOST_TEST_CHECKPOINT("0102000");
	TVector<int> V_0102000;
	V_0102000.Add(0);
	V_0102000.Add(1);
	V_0102000.Add(0);
	V_0102000.Add(2);
	V_0102000.Add(0);
	V_0102000.Add(0);
	V_0102000.Add(0);
	int32_t count_0102000 = V_0102000.RemoveAllPredicate(pred);
	BOOST_REQUIRE_EQUAL(count_0102000, 5);
	BOOST_REQUIRE_EQUAL(V_0102000.Len(), 2);
	BOOST_REQUIRE_EQUAL(V_0102000[0], 1);
	BOOST_REQUIRE_EQUAL(V_0102000[1], 2);

	BOOST_TEST_CHECKPOINT("123");
	TVector<int> V_123;
	for (int i = 0; i < 3; i++) { V_123.Add(i + 1); }
	int32_t count_123 = V_123.RemoveAllPredicate(pred);
	BOOST_REQUIRE_EQUAL(count_123, 0);
	BOOST_REQUIRE_EQUAL(V_123.Len(), 3);
	BOOST_REQUIRE_EQUAL(V_123[0], 1);
	BOOST_REQUIRE_EQUAL(V_123[1], 2);
	BOOST_REQUIRE_EQUAL(V_123[2], 3);

	BOOST_TEST_CHECKPOINT("010002045006007");
	TVector<int> V_010002045006007;
	V_010002045006007.Add(0);
	V_010002045006007.Add(1);
	V_010002045006007.Add(0);
	V_010002045006007.Add(0);
	V_010002045006007.Add(0);
	V_010002045006007.Add(2);
	V_010002045006007.Add(0);
	V_010002045006007.Add(4);
	V_010002045006007.Add(5);
	V_010002045006007.Add(0);
	V_010002045006007.Add(0);
	V_010002045006007.Add(6);
	V_010002045006007.Add(0);
	V_010002045006007.Add(0);
	V_010002045006007.Add(7);
	int32_t count_010002045006007 = V_010002045006007.RemoveAllPredicate(pred);
	BOOST_REQUIRE_EQUAL(count_010002045006007, 9);
	BOOST_REQUIRE_EQUAL(V_010002045006007.Len(), 6);
	BOOST_REQUIRE_EQUAL(V_010002045006007[0], 1);
	BOOST_REQUIRE_EQUAL(V_010002045006007[1], 2);
	BOOST_REQUIRE_EQUAL(V_010002045006007[2], 4);
	BOOST_REQUIRE_EQUAL(V_010002045006007[3], 5);
	BOOST_REQUIRE_EQUAL(V_010002045006007[4], 6);
	BOOST_REQUIRE_EQUAL(V_010002045006007[5], 7);

	BOOST_TEST_CHECKPOINT("12300405780");
	// 1. We have zeroes at the end;
	// 2. We start with non-zero
	TVector<int> V_12300405780;
	V_12300405780.Add(1);
	V_12300405780.Add(2);
	V_12300405780.Add(3);
	V_12300405780.Add(0);
	V_12300405780.Add(0);
	V_12300405780.Add(4);
	V_12300405780.Add(0);
	V_12300405780.Add(5);
	V_12300405780.Add(7);
	V_12300405780.Add(8);
	V_12300405780.Add(0);
	int32_t count_12300405780 = V_12300405780.RemoveAllPredicate(pred);
	BOOST_REQUIRE_EQUAL(count_12300405780, 4);
	BOOST_REQUIRE_EQUAL(V_12300405780.Len(), 7);
	BOOST_REQUIRE_EQUAL(V_12300405780[0], 1);
	BOOST_REQUIRE_EQUAL(V_12300405780[1], 2);
	BOOST_REQUIRE_EQUAL(V_12300405780[2], 3);
	BOOST_REQUIRE_EQUAL(V_12300405780[3], 4);
	BOOST_REQUIRE_EQUAL(V_12300405780[4], 5);
	BOOST_REQUIRE_EQUAL(V_12300405780[5], 7);
	BOOST_REQUIRE_EQUAL(V_12300405780[6], 8);
}

BOOST_AUTO_TEST_CASE(SimplePod_RemoveAllAtSwap, *boost::unit_test::depends_on("Core/Container/TVectorTestSuite/SimplePod_SmallMainTest"))
{
	BOOST_TEST_CHECKPOINT("Empty");
	TVector<int> V_empty;
	int32_t count_fromEmpty = V_empty.RemoveAllSwap(0);
	BOOST_REQUIRE_EQUAL(count_fromEmpty, 0);
	BOOST_REQUIRE_EQUAL(V_empty.Len(), 0);

	BOOST_TEST_CHECKPOINT("OneFound");
	TVector<int> V_oneFound;
	V_oneFound.Add(0);
	int32_t count_fromOneFound = V_oneFound.RemoveAllSwap(0);
	BOOST_REQUIRE_EQUAL(count_fromOneFound, 1);
	BOOST_REQUIRE_EQUAL(V_oneFound.Len(), 0);

	BOOST_TEST_CHECKPOINT("OneNotFound");
	TVector<int> V_oneNotFound;
	V_oneNotFound.Add(1);
	int32_t count_fromOneNotFound = V_oneNotFound.RemoveAllSwap(0);
	BOOST_REQUIRE_EQUAL(count_fromOneNotFound, 0);
	BOOST_REQUIRE_EQUAL(V_oneNotFound.Len(), 1);
	BOOST_REQUIRE_EQUAL(V_oneNotFound[0], 1);

	BOOST_TEST_CHECKPOINT("00");
	TVector<int> V_00;
	V_00.Add(0);
	V_00.Add(0);
	int32_t count_from00 = V_00.RemoveAllSwap(0);
	BOOST_REQUIRE_EQUAL(count_from00, 2);
	BOOST_REQUIRE_EQUAL(V_00.Len(), 0);

	BOOST_TEST_CHECKPOINT("11");
	TVector<int> V_11;
	V_11.Add(1);
	V_11.Add(1);
	int32_t count_from11 = V_11.RemoveAllSwap(0);
	BOOST_REQUIRE_EQUAL(count_from11, 0);
	BOOST_REQUIRE_EQUAL(V_11.Len(), 2);
	BOOST_REQUIRE(V_11.Contains(1));

	BOOST_TEST_CHECKPOINT("01");
	TVector<int> V_01;
	V_01.Add(0);
	V_01.Add(1);
	int32_t count_from01 = V_01.RemoveAllSwap(0);
	BOOST_REQUIRE_EQUAL(count_from01, 1);
	BOOST_REQUIRE_EQUAL(V_01.Len(), 1);
	BOOST_REQUIRE(V_01.Contains(1));

	BOOST_TEST_CHECKPOINT("012");
	TVector<int> V_012;
	V_012.Add(0);
	V_012.Add(1);
	V_012.Add(2);
	int32_t count_from012 = V_012.RemoveAllSwap(0);
	BOOST_REQUIRE_EQUAL(count_from012, 1);
	BOOST_REQUIRE_EQUAL(V_012.Len(), 2);
	BOOST_REQUIRE(V_012.Contains(1));
	BOOST_REQUIRE(V_012.Contains(2));

	BOOST_TEST_CHECKPOINT("0012");
	TVector<int> V_0012;
	V_0012.Add(0);
	V_0012.Add(0);
	V_0012.Add(1);
	V_0012.Add(2);
	int32_t count_from0012 = V_0012.RemoveAllSwap(0);
	BOOST_REQUIRE_EQUAL(count_from0012, 2);
	BOOST_REQUIRE_EQUAL(V_0012.Len(), 2);
	BOOST_REQUIRE(V_0012.Contains(1));
	BOOST_REQUIRE(V_0012.Contains(2));

	BOOST_TEST_CHECKPOINT("12034");
	TVector<int> V_12034;
	V_12034.Add(1);
	V_12034.Add(2);
	V_12034.Add(0);
	V_12034.Add(3);
	V_12034.Add(4);
	int32_t count_from12034 = V_12034.RemoveAllSwap(0);
	BOOST_REQUIRE_EQUAL(count_from12034, 1);
	BOOST_REQUIRE_EQUAL(V_12034.Len(), 4);
	BOOST_REQUIRE(V_12034.Contains(1));
	BOOST_REQUIRE(V_12034.Contains(2));
	BOOST_REQUIRE(V_12034.Contains(3));
	BOOST_REQUIRE(V_12034.Contains(4));

	BOOST_TEST_CHECKPOINT("120034");
	TVector<int> V_120034;
	V_120034.Add(1);
	V_120034.Add(2);
	V_120034.Add(0);
	V_120034.Add(0);
	V_120034.Add(3);
	V_120034.Add(4);
	int32_t count_from120034 = V_120034.RemoveAllSwap(0);
	BOOST_REQUIRE_EQUAL(count_from120034, 2);
	BOOST_REQUIRE_EQUAL(V_120034.Len(), 4);
	BOOST_REQUIRE(V_120034.Contains(1));
	BOOST_REQUIRE(V_120034.Contains(2));
	BOOST_REQUIRE(V_120034.Contains(3));
	BOOST_REQUIRE(V_120034.Contains(4));

	BOOST_TEST_CHECKPOINT("0102000");
	TVector<int> V_0102000;
	V_0102000.Add(0);
	V_0102000.Add(1);
	V_0102000.Add(0);
	V_0102000.Add(2);
	V_0102000.Add(0);
	V_0102000.Add(0);
	V_0102000.Add(0);
	int32_t count_0102000 = V_0102000.RemoveAllSwap(0);
	BOOST_REQUIRE_EQUAL(count_0102000, 5);
	BOOST_REQUIRE_EQUAL(V_0102000.Len(), 2);
	BOOST_REQUIRE(V_0102000.Contains(1));
	BOOST_REQUIRE(V_0102000.Contains(2));

	BOOST_TEST_CHECKPOINT("010002045006007");
	TVector<int> V_010002045006007;
	V_010002045006007.Add(0);
	V_010002045006007.Add(1);
	V_010002045006007.Add(0);
	V_010002045006007.Add(0);
	V_010002045006007.Add(0);
	V_010002045006007.Add(2);
	V_010002045006007.Add(0);
	V_010002045006007.Add(4);
	V_010002045006007.Add(5);
	V_010002045006007.Add(0);
	V_010002045006007.Add(0);
	V_010002045006007.Add(6);
	V_010002045006007.Add(0);
	V_010002045006007.Add(0);
	V_010002045006007.Add(7);
	int32_t count_010002045006007 = V_010002045006007.RemoveAllSwap(0);
	BOOST_REQUIRE_EQUAL(count_010002045006007, 9);
	BOOST_REQUIRE_EQUAL(V_010002045006007.Len(), 6);
	BOOST_REQUIRE(V_010002045006007.Contains(1));
	BOOST_REQUIRE(V_010002045006007.Contains(2));
	BOOST_REQUIRE(V_010002045006007.Contains(4));
	BOOST_REQUIRE(V_010002045006007.Contains(5));
	BOOST_REQUIRE(V_010002045006007.Contains(6));
	BOOST_REQUIRE(V_010002045006007.Contains(7));

	BOOST_TEST_CHECKPOINT("SuperTest");
	TVector<int> V_super;
	V_super.Add(1);
	V_super.Add(0);
	V_super.Add(0);
	V_super.Add(0);
	V_super.Add(2);
	V_super.Add(3);
	V_super.Add(0);
	V_super.Add(4);
	V_super.Add(0);
	V_super.Add(0);
	V_super.Add(5);
	V_super.Add(6);
	V_super.Add(7);
	V_super.Add(0);
	V_super.Add(8);
	V_super.Add(9);
	const int32_t INITIAL_SUPER_LEN = V_super.Len();
	const int32_t INITIAL_SUPER_NUM_ZEROES = V_super.CountValues(0);
	const int32_t SUPER_NUM_TO_REMOVE = INITIAL_SUPER_LEN - INITIAL_SUPER_NUM_ZEROES;
	const int32_t count_superTest = V_super.RemoveAllSwap(0);
	BOOST_REQUIRE_EQUAL(count_superTest, INITIAL_SUPER_NUM_ZEROES);
	BOOST_REQUIRE_EQUAL(V_super.Len(), SUPER_NUM_TO_REMOVE);
	for (int i = 0; i < SUPER_NUM_TO_REMOVE; i++)
	{
		BOOST_REQUIRE(V_super.Contains(i + 1));
	}
}

BOOST_AUTO_TEST_CASE(SimplePod_RemoveAllSwapPredicate, *boost::unit_test::depends_on("Core/Container/TVectorTestSuite/SimplePod_SmallMainTest"))
{
	BOOST_TEST_CHECKPOINT("Empty");
	const auto pred = [](const int& val) { return val == 0; };
	TVector<int> V_empty;
	int32_t count_fromEmpty = V_empty.RemoveAllSwapPredicate(pred);
	BOOST_REQUIRE_EQUAL(count_fromEmpty, 0);
	BOOST_REQUIRE_EQUAL(V_empty.Len(), 0);

	BOOST_TEST_CHECKPOINT("OneFound");
	TVector<int> V_oneFound;
	V_oneFound.Add(0);
	int32_t count_fromOneFound = V_oneFound.RemoveAllSwapPredicate(pred);
	BOOST_REQUIRE_EQUAL(count_fromOneFound, 1);
	BOOST_REQUIRE_EQUAL(V_oneFound.Len(), 0);

	BOOST_TEST_CHECKPOINT("OneNotFound");
	TVector<int> V_oneNotFound;
	V_oneNotFound.Add(1);
	int32_t count_fromOneNotFound = V_oneNotFound.RemoveAllSwapPredicate(pred);
	BOOST_REQUIRE_EQUAL(count_fromOneNotFound, 0);
	BOOST_REQUIRE_EQUAL(V_oneNotFound.Len(), 1);
	BOOST_REQUIRE_EQUAL(V_oneNotFound[0], 1);

	BOOST_TEST_CHECKPOINT("0102000");
	TVector<int> V_0102000;
	V_0102000.Add(0);
	V_0102000.Add(1);
	V_0102000.Add(0);
	V_0102000.Add(2);
	V_0102000.Add(0);
	V_0102000.Add(0);
	V_0102000.Add(0);
	int32_t count_0102000 = V_0102000.RemoveAllSwapPredicate(pred);
	BOOST_REQUIRE_EQUAL(count_0102000, 5);
	BOOST_REQUIRE_EQUAL(V_0102000.Len(), 2);
	BOOST_REQUIRE(V_0102000.Contains(1));
	BOOST_REQUIRE(V_0102000.Contains(2));

	BOOST_TEST_CHECKPOINT("010002045006007");
	TVector<int> V_010002045006007;
	V_010002045006007.Add(0);
	V_010002045006007.Add(1);
	V_010002045006007.Add(0);
	V_010002045006007.Add(0);
	V_010002045006007.Add(0);
	V_010002045006007.Add(2);
	V_010002045006007.Add(0);
	V_010002045006007.Add(4);
	V_010002045006007.Add(5);
	V_010002045006007.Add(0);
	V_010002045006007.Add(0);
	V_010002045006007.Add(6);
	V_010002045006007.Add(0);
	V_010002045006007.Add(0);
	V_010002045006007.Add(7);
	int32_t count_010002045006007 = V_010002045006007.RemoveAllSwapPredicate(pred);
	BOOST_REQUIRE_EQUAL(count_010002045006007, 9);
	BOOST_REQUIRE_EQUAL(V_010002045006007.Len(), 6);
	BOOST_REQUIRE(V_010002045006007.Contains(1));
	BOOST_REQUIRE(V_010002045006007.Contains(2));
	BOOST_REQUIRE(V_010002045006007.Contains(4));
	BOOST_REQUIRE(V_010002045006007.Contains(5));
	BOOST_REQUIRE(V_010002045006007.Contains(6));
	BOOST_REQUIRE(V_010002045006007.Contains(7));
}

BOOST_AUTO_TEST_SUITE_END() // Container
BOOST_AUTO_TEST_SUITE_END() // TVectorTestSuite
BOOST_AUTO_TEST_SUITE_END() // Core

} // anonymous