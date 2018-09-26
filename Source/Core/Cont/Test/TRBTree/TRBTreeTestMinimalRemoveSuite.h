#pragma once

#include "TRBTreeTestUtils.h"

namespace
{
	BOOST_AUTO_TEST_SUITE(Core)
	BOOST_AUTO_TEST_SUITE(Container)
	BOOST_AUTO_TEST_SUITE(TRBTreeTestSuite)


BOOST_AUTO_TEST_SUITE
(
	ExtraRemoveSuite,
	*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/AddSuite")
	*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/RemoveSuite")
)

BOOST_AUTO_TEST_CASE(GroupKeyRemoveFirstOrAllRangeTest)
{
	using TreeType = TRBTree<KVType<int, std::string>>;

	constexpr int INITIAL_COUNT = 8;

	constexpr int MIN_REMOVE_KEY = 3;
	constexpr int MAXIM_REMOVE_KEY = 6;
	constexpr int NON_EXISTING_KEY = 10000;
	
	const auto P = [MIN_REMOVE_KEY, MAXIM_REMOVE_KEY](const TreeType::KeyType& K) -> bool
	{
		return K >= MIN_REMOVE_KEY && K <= MAXIM_REMOVE_KEY;
	};
	const auto P_NonExistent = [NON_EXISTING_KEY](const TreeType::KeyType& K) -> bool
	{
		return K == NON_EXISTING_KEY;
	};

	BOOST_TEST_CHECKPOINT("Empty");
	TreeType T;
	BOOST_REQUIRE( ! T.RemoveFirstKeyPredicate_InRange(P, T.Iterator(), T.EndIterator()) );
	BOOST_REQUIRE( ! T.RemoveFirstKeyPredicate_InRange(P_NonExistent, T.Iterator(), T.EndIterator()) );
	BOOST_REQUIRE( ! T.RemoveAllKeyPredicate_InRange(P, T.Iterator(), T.EndIterator()));
	BOOST_REQUIRE( ! T.RemoveAllKeyPredicate_InRange(P_NonExistent, T.Iterator(), T.EndIterator()));

	BOOST_TEST_CHECKPOINT("Filling");
	T.Add(1, std::string("test"));
	T.Add(2, std::string("test"));
	T.Add(3, std::string("test"));
	T.Add(4, std::string("test"));
	T.Add(5, std::string("test"));
	T.Add(6, std::string("test"));
	T.Add(7, std::string("test"));
	T.Add(8, std::string("test"));
	TreeType::IteratorType const ItFirst = T.FindIteratorFor(1);
	TreeType::IteratorType const ItSecond = T.FindIteratorFor(2);
	TreeType::IteratorType const ItThird = T.FindIteratorFor(3);
	TreeType::IteratorType const ItFourth = T.FindIteratorFor(4);
	TreeType::IteratorType const ItSixth = T.FindIteratorFor(6);

	TreeType::IteratorType const ItNarrowRemoveRangeStart = ItFourth;
	TreeType::IteratorType const ItNarrowRemoveRangeEnd = ItSixth;

	BOOST_REQUIRE_EQUAL(ItFirst.GetKey(), 1);
	BOOST_REQUIRE_EQUAL(ItSecond.GetKey(), 2);
	BOOST_REQUIRE_EQUAL(ItThird.GetKey(), 3);
	BOOST_REQUIRE_EQUAL(ItFourth.GetKey(), 4);
	BOOST_REQUIRE_EQUAL(ItSixth.GetKey(), 6);
	BOOST_REQUIRE_EQUAL(T.Num(), INITIAL_COUNT);

	BOOST_TEST_CHECKPOINT("Removing first and all out of range");
	BOOST_REQUIRE( ! T.RemoveAllKeyPredicate_InRange(P, ItFirst, ItThird) );
	BOOST_REQUIRE( ! T.RemoveFirstKeyPredicate_InRange(P, ItFirst, ItThird) );

	BOOST_TEST_CHECKPOINT("Removing first in range");
	BOOST_REQUIRE(T.RemoveFirstKeyPredicate_InRange(P, ItNarrowRemoveRangeStart, ItNarrowRemoveRangeEnd));
	BOOST_REQUIRE(T.ContainsKeyPredicate(P));
	

	BOOST_TEST_CHECKPOINT("Removing first in range");
	BOOST_REQUIRE(T.RemoveFirstKeyPredicate_InRange(P, T.Iterator(), T.EndIterator()));
	BOOST_REQUIRE( T.ContainsKeyPredicate(P) );

	BOOST_TEST_CHECKPOINT("Removing all in range");
	BOOST_REQUIRE( T.RemoveAllKeyPredicate_InRange(P, T.Iterator(), T.EndIterator()) );
	BOOST_REQUIRE( ! T.ContainsKeyPredicate(P) );
}

BOOST_AUTO_TEST_CASE(GroupKeyRemoveFirstOrAllTest)
{
	using TreeType = TRBTree<KVType<std::string, int>>;

	constexpr int INITIAL_COUNT = 8;
	constexpr int KEY_TO_REMOVE_LENGTH = 3;
	constexpr int NON_EXISTING_LENGTH = 10000;

	const auto P = [KEY_TO_REMOVE_LENGTH](const TreeType::KeyType& K) -> bool
	{
		return K.size() == KEY_TO_REMOVE_LENGTH;
	};
	const auto P_NonExistent = [NON_EXISTING_LENGTH](const TreeType::KeyType& K) -> bool
	{
		return K.size() == NON_EXISTING_LENGTH;
	};

	BOOST_TEST_CHECKPOINT("Remove from empty");
	TreeType T;
	BOOST_REQUIRE( ! T.RemoveFirstKeyPredicate(P) );
	BOOST_REQUIRE( ! T.RemoveFirstKeyPredicate(P_NonExistent) );

	BOOST_TEST_CHECKPOINT("Fill");
	T.Add(std::string("asdasdgsdfsd"), 1);
	T.Add(std::string("abc"), 2);
	T.Add(std::string("def"), 3);
	T.Add(std::string("deg"), 4);
	T.Add(std::string("asdgasdasdfasasfd"), 5);
	T.Add(std::string("ten"), 6);
	T.Add(std::string("xdfdfhdf"), 7);
	T.Add(std::string("zen"), 8);
	BOOST_REQUIRE_EQUAL(T.Num(), INITIAL_COUNT);

	BOOST_TEST_CHECKPOINT("Remove first");
	BOOST_REQUIRE( ! T.RemoveFirstKeyPredicate(P_NonExistent) );
	BOOST_REQUIRE(T.RemoveFirstKeyPredicate(P) );
	BOOST_REQUIRE_EQUAL(T.Num(), (INITIAL_COUNT - 1));

	BOOST_TEST_CHECKPOINT("Remove all");
	BOOST_REQUIRE( ! T.RemoveAllKeyPredicate(P_NonExistent) );
	BOOST_REQUIRE( T.RemoveAllKeyPredicate(P) );
	BOOST_REQUIRE( ! T.ContainsKeyPredicate(P) );
}

BOOST_AUTO_TEST_CASE(GroupRemoveTest)
{
	constexpr int INITIAL_COUNT = 8;
	std::string const VALUE_TO_REMOVE { "REM" };
	std::string const OTHER_VAL { "OTHER_VAL" };
	std::string const NON_EXISTING_VAL { "NON_EXISTING_VAL" };

	const auto P = [VALUE_TO_REMOVE](const IntStringRBTree::KeyValueType& InKV) -> bool
	{
		return InKV.Value == VALUE_TO_REMOVE;
	};
	const auto P_NonExistent = [NON_EXISTING_VAL](const IntStringRBTree::KeyValueType& InKV) -> bool
	{
		return InKV.Value == NON_EXISTING_VAL;
	};

	BOOST_TEST_CHECKPOINT("Remove from empty");
	IntStringRBTree T;

	BOOST_REQUIRE
	(
		! T.RemoveFirstPredicate_InRange(P, T.Iterator(), T.EndIterator() )
	);
	BOOST_REQUIRE( ! T.RemoveAllPredicate(P));
	BOOST_REQUIRE( 0 == T.RemoveAllPredicate_InRange(P, T.Iterator(), T.EndIterator()));

	BOOST_TEST_CHECKPOINT("Fill");
	BOOST_REQUIRE(T.Add(1, std::string("ONE")));
	BOOST_REQUIRE(T.Add(2, VALUE_TO_REMOVE));
	BOOST_REQUIRE(T.Add(3, VALUE_TO_REMOVE));
	BOOST_REQUIRE(T.Add(4, OTHER_VAL));
	BOOST_REQUIRE(T.Add(5, OTHER_VAL));
	BOOST_REQUIRE(T.Add(6, VALUE_TO_REMOVE));
	BOOST_REQUIRE(T.Add(7, OTHER_VAL));
	BOOST_REQUIRE(T.Add(8, VALUE_TO_REMOVE));

	IntStringRBTree::IteratorType const It_OtherZone_First = GetAdvancedIt(T.Iterator(), 3);
	IntStringRBTree::IteratorType const It_OtherZone_Last = GetNextIt(GetNextIt(It_OtherZone_First));
	IntStringRBTree::IteratorType const It_Last = GetAdvancedIt(T.Iterator(), 7);

	BOOST_REQUIRE_EQUAL(T.Num(), INITIAL_COUNT);

	BOOST_TEST_CHECKPOINT("Remove non-existent");
	BOOST_REQUIRE( ! T.RemoveFirstPredicate(P_NonExistent) );
	BOOST_REQUIRE( ! T.RemoveFirstPredicate_InRange(P_NonExistent, T.Iterator(), T.EndIterator() ) );
	BOOST_REQUIRE( 0 == T.RemoveAllPredicate(P_NonExistent));
	BOOST_REQUIRE( 0 == T.RemoveAllPredicate_InRange(P_NonExistent, T.Iterator(), T.EndIterator()) );

	BOOST_TEST_CHECKPOINT("Remove out of range");
	BOOST_REQUIRE( ! T.RemoveFirstPredicate_InRange(P, It_OtherZone_First, It_OtherZone_Last) );
	BOOST_REQUIRE( 0 == T.RemoveAllPredicate_InRange(P, It_OtherZone_First, It_OtherZone_Last) );

	BOOST_TEST_CHECKPOINT("Remove first");
	BOOST_REQUIRE( T.RemoveFirstPredicate(P) );
	BOOST_REQUIRE_EQUAL( T.Num(), (INITIAL_COUNT - 1) );

	BOOST_TEST_CHECKPOINT("Remove last and check that remove first returns end");
	IntStringRBTree::IteratorType ItAfterLast { T.Iterator() };

	BOOST_REQUIRE( T.RemoveFirstPredicate_InRange(P, It_Last, T.EndIterator(), /*out*/ItAfterLast) );

	BOOST_REQUIRE_EQUAL( T.Num(), (INITIAL_COUNT - 2) );
	BOOST_REQUIRE( ItAfterLast.IsEnd() );

	BOOST_TEST_CHECKPOINT("Remove all and check that no more elements with the given value contained");
	BOOST_REQUIRE( T.RemoveAllPredicate(P) > 0 );
	BOOST_REQUIRE( ! T.ContainsPredicate(P) );
}

BOOST_AUTO_TEST_CASE(RemoveAtTest)
{
	constexpr int INITIAL_COUNT = 3;

	IntRBTree T;
	BOOST_REQUIRE(T.Add(2, NoValue{}));
	BOOST_REQUIRE(T.Add(1, NoValue{}));
	BOOST_REQUIRE(T.Add(3, NoValue{}));

	T.RemoveAt(T.Iterator());

	BOOST_REQUIRE_EQUAL(T.Num(), (INITIAL_COUNT - 1));
	BOOST_REQUIRE( ! T.Contains(1) );
	BOOST_REQUIRE(T.Contains(2));
	BOOST_REQUIRE(T.Contains(3));
}
BOOST_AUTO_TEST_SUITE_END() // ExtraRemoveSuite


	BOOST_AUTO_TEST_SUITE(Minimal)

		BOOST_AUTO_TEST_SUITE
		(
			ComplexRemoveTestSuite,
			*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/RemoveSuite")
		)

		BOOST_AUTO_TEST_CASE(Remove_CountTen_RemoveFour)
	{
		//
		// WARNING!!! Very important: this is particular RARE case of removing!
		//
		constexpr int INITIAL_COUNT = 10;
		constexpr int KEY_TO_REMOVE = 4;

		BOOST_TEST_CHECKPOINT("Making initial");
		IntRBTree T;
		for (int i = 1; i <= INITIAL_COUNT; i++)
		{
			BOOST_REQUIRE(T.Add(i, NoValue{}));
		}

		BOOST_TEST_CHECKPOINT("Removing");
		T.Remove(KEY_TO_REMOVE);

		BOOST_TEST_CHECKPOINT("Checking");
		BOOST_REQUIRE_EQUAL(T.Num(), (INITIAL_COUNT - 1));
		for (int i = 1; i <= INITIAL_COUNT; i++)
		{
			if (i != KEY_TO_REMOVE)
			{
				BOOST_REQUIRE(T.Contains(i));
			}
		}
	}

	BOOST_AUTO_TEST_CASE
	(
		Remove_CountTen_RemoveFive,
		*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/ComplexRemoveTestSuite/Remove_CountTen_RemoveFour")
	)
	{

		//
		// WARNING!!! Very important: this is particular RARE case of removing!
		//
		constexpr int INITIAL_COUNT = 10;
		constexpr int KEY_TO_REMOVE = 5;

		BOOST_TEST_CHECKPOINT("Making initial");
		IntRBTree T;
		for (int i = 1; i <= INITIAL_COUNT; i++)
		{
			BOOST_REQUIRE(T.Add(i, NoValue{}));
		}

		BOOST_TEST_CHECKPOINT("Removing");
		T.Remove(KEY_TO_REMOVE);

		BOOST_TEST_CHECKPOINT("Checking");
		BOOST_REQUIRE_EQUAL(T.Num(), (INITIAL_COUNT - 1));
		for (int i = 1; i <= INITIAL_COUNT; i++)
		{
			if (i != KEY_TO_REMOVE)
			{
				BOOST_REQUIRE(T.Contains(i));
			}
		}
	}

	BOOST_AUTO_TEST_CASE
	(
		Remove_CountTen_RemoveSixth,
		*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/ComplexRemoveTestSuite/Remove_CountTen_RemoveFive")
	)
	{

		//
		// WARNING!!! Very important: this is particular RARE case of removing!
		//
		constexpr int INITIAL_COUNT = 10;
		constexpr int KEY_TO_REMOVE = 6;

		BOOST_TEST_CHECKPOINT("Making initial");
		IntRBTree T;
		for (int i = 1; i <= INITIAL_COUNT; i++)
		{
			BOOST_REQUIRE(T.Add(i, NoValue{}));
		}

		BOOST_TEST_CHECKPOINT("Removing");
		T.Remove(KEY_TO_REMOVE);

		BOOST_TEST_CHECKPOINT("Checking");
		BOOST_REQUIRE_EQUAL(T.Num(), (INITIAL_COUNT - 1));
		for (int i = 1; i <= INITIAL_COUNT; i++)
		{
			if (i != KEY_TO_REMOVE)
			{
				BOOST_REQUIRE(T.Contains(i));
			}
		}
	}

	BOOST_AUTO_TEST_CASE
	(
		Remove_Count30_Remove25,
		*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/ComplexRemoveTestSuite/Remove_CountTen_RemoveSixth")
	)
	{
		//
		// WARNING!!! Very important: this is particular RARE case of removing!
		//
		constexpr int INITIAL_COUNT = 30;
		constexpr int KEY_TO_REMOVE = 25;

		BOOST_TEST_CHECKPOINT("Making initial");
		IntRBTree T;
		for (int i = 1; i <= INITIAL_COUNT; i++)
		{
			BOOST_REQUIRE(T.Add(i, NoValue{}));
		}

		BOOST_TEST_CHECKPOINT("Removing");
		T.Remove(KEY_TO_REMOVE);

		BOOST_TEST_CHECKPOINT("Checking");
		BOOST_REQUIRE_EQUAL(T.Num(), (INITIAL_COUNT - 1));
		for (int i = 1; i <= INITIAL_COUNT; i++)
		{
			if (i != KEY_TO_REMOVE)
			{
				BOOST_REQUIRE(T.Contains(i));
			}
		}
	}

	BOOST_AUTO_TEST_SUITE_END() // ComplexRemoveTestSuite

BOOST_AUTO_TEST_SUITE
(
	RemoveSuite,
	*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/AddSuite")
)
BOOST_AUTO_TEST_CASE(RemoveFromEmpty)
{
	constexpr int KEY_ONE = 1;
	constexpr int KEY_TWO = 2;

	IntRBTree T;
	BOOST_REQUIRE(!T.Remove(KEY_ONE));
	BOOST_REQUIRE(!T.Remove(KEY_TWO));
}

BOOST_AUTO_TEST_CASE(RemoveFromRoot_TreeWithOneElem)
{
	constexpr int KEY_EXISTING = 1;
	constexpr int KEY_NOT_EXISTING = 2;

	BOOST_TEST_CHECKPOINT("Preparing");
	IntRBTree T;
	T.Add(KEY_EXISTING, NoValue{});
	BOOST_REQUIRE_EQUAL(T.Num(), 1);

	BOOST_TEST_CHECKPOINT("Removing non-existing key");
	BOOST_REQUIRE(!T.Remove(KEY_NOT_EXISTING));
	BOOST_REQUIRE(!T.Empty());

	BOOST_TEST_CHECKPOINT("Removing");
	BOOST_REQUIRE(T.Remove(KEY_EXISTING));
	BOOST_REQUIRE(T.Empty());
}

BOOST_AUTO_TEST_CASE
(
	RemoveFromRoot_ThatHasOnlyLeftChild_OnlyTwoElements,
	*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/RemoveSuite/RemoveFromRoot_TreeWithOneElem")
)
{
	constexpr int KEY_ROOT = 2;
	constexpr int KEY_LEFT_CHILD = 1;
	constexpr int KEY_NOT_EXISTING = 9;

	BOOST_TEST_CHECKPOINT("Preparing");
	IntRBTree T;
	T.Add(KEY_ROOT, NoValue{});
	T.Add(KEY_LEFT_CHILD, NoValue{});
	BOOST_REQUIRE_EQUAL(T.Num(), 2);

	BOOST_TEST_CHECKPOINT("Removing non-existing key");
	BOOST_REQUIRE(!T.Remove(KEY_NOT_EXISTING));
	BOOST_REQUIRE(!T.Empty());

	BOOST_TEST_CHECKPOINT("Removing");
	BOOST_REQUIRE(T.Remove(KEY_ROOT));
	BOOST_REQUIRE_EQUAL(T.Num(), 1);
	BOOST_REQUIRE_EQUAL(T.Min().Key, KEY_LEFT_CHILD);
	BOOST_REQUIRE_EQUAL(T.Max().Key, KEY_LEFT_CHILD);
}

BOOST_AUTO_TEST_CASE
(
	RemoveFromRoot_ThatHasOnlyRightChild_OnlyTwoElements,
	*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/RemoveSuite/RemoveFromRoot_TreeWithOneElem")
)
{
	constexpr int KEY_ROOT = 1;
	constexpr int KEY_RIGHT_CHILD = 2;
	constexpr int KEY_NOT_EXISTING = 9;

	BOOST_TEST_CHECKPOINT("Preparing");
	IntRBTree T;
	T.Add(KEY_ROOT, NoValue{});
	T.Add(KEY_RIGHT_CHILD, NoValue{});
	BOOST_REQUIRE_EQUAL(T.Num(), 2);

	BOOST_TEST_CHECKPOINT("Removing non-existing key");
	BOOST_REQUIRE(!T.Remove(KEY_NOT_EXISTING));
	BOOST_REQUIRE(!T.Empty());

	BOOST_TEST_CHECKPOINT("Removing");
	BOOST_REQUIRE(T.Remove(KEY_ROOT));
	BOOST_REQUIRE_EQUAL(T.Num(), 1);
	BOOST_REQUIRE_EQUAL(T.Min().Key, KEY_RIGHT_CHILD);
	BOOST_REQUIRE_EQUAL(T.Max().Key, KEY_RIGHT_CHILD);
}

BOOST_AUTO_TEST_CASE
(
	RemoveLeftBlackChildLeaf_OfRoot,
	*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/RemoveSuite/RemoveFromRoot_ThatHasOnlyLeftChild_OnlyTwoElements")
	*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/RemoveSuite/RemoveFromRoot_ThatHasOnlyRightChild_OnlyTwoElements")
)
{
	BOOST_TEST_CHECKPOINT("Preparing");
	IntRBTree T;
	for (int i = 1; i <= 4; i++)
	{
		T.Add(i, NoValue{});
	}
	BOOST_REQUIRE_EQUAL(T.Num(), 4);

	BOOST_TEST_CHECKPOINT("Removing");
	BOOST_REQUIRE(T.Remove(1));
	BOOST_REQUIRE_EQUAL(T.Num(), 3);

	IntRBTree::KeyValueType ordered[3];
	T.CopyTo(ordered);
	BOOST_REQUIRE_EQUAL(ordered[0].Key, 2);
	BOOST_REQUIRE_EQUAL(ordered[1].Key, 3);
	BOOST_REQUIRE_EQUAL(ordered[2].Key, 4);
}

BOOST_AUTO_TEST_CASE
(
	RemoveRoot_BothChildrenOfBrotherRed,
	*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/RemoveSuite/RemoveFromRoot_ThatHasOnlyLeftChild_OnlyTwoElements")
	*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/RemoveSuite/RemoveFromRoot_ThatHasOnlyRightChild_OnlyTwoElements")
)
{
	BOOST_TEST_CHECKPOINT("Preparing");
	IntRBTree T;
	for (int i = 1; i <= 5; i++)
	{
		T.Add(i, NoValue{});
	}
	BOOST_REQUIRE_EQUAL(T.Num(), 5);

	BOOST_TEST_CHECKPOINT("Removing");
	BOOST_REQUIRE(T.Remove(2));
	BOOST_REQUIRE_EQUAL(T.Num(), 4);

	IntRBTree::KeyValueType ordered[4];
	T.CopyTo(ordered);
	BOOST_REQUIRE_EQUAL(ordered[0].Key, 1);
	BOOST_REQUIRE_EQUAL(ordered[1].Key, 3);
	BOOST_REQUIRE_EQUAL(ordered[2].Key, 4);
	BOOST_REQUIRE_EQUAL(ordered[3].Key, 5);
}

BOOST_AUTO_TEST_CASE
(
	RemoveRoot_BrotherHasLeftRedChild,
	*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/RemoveSuite/RemoveFromRoot_ThatHasOnlyLeftChild_OnlyTwoElements")
	*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/RemoveSuite/RemoveFromRoot_ThatHasOnlyRightChild_OnlyTwoElements")
)
{
	BOOST_TEST_CHECKPOINT("Preparing");
	IntRBTree T;
	T.Add(2, NoValue{});
	T.Add(1, NoValue{});
	T.Add(4, NoValue{});
	T.Add(3, NoValue{});
	BOOST_REQUIRE_EQUAL(T.Num(), 4);

	BOOST_TEST_CHECKPOINT("Removing");
	BOOST_REQUIRE(T.Remove(2));
	BOOST_REQUIRE_EQUAL(T.Num(), 3);

	IntRBTree::KeyValueType ordered[3];
	T.CopyTo(ordered);
	BOOST_REQUIRE_EQUAL(ordered[0].Key, 1);
	BOOST_REQUIRE_EQUAL(ordered[1].Key, 3);
	BOOST_REQUIRE_EQUAL(ordered[2].Key, 4);
}

BOOST_AUTO_TEST_CASE
(
	RemoveRoot_BrotherHasRightRedChild,
	*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/RemoveSuite/RemoveFromRoot_ThatHasOnlyLeftChild_OnlyTwoElements")
	*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/RemoveSuite/RemoveFromRoot_ThatHasOnlyRightChild_OnlyTwoElements")
)
{
	BOOST_TEST_CHECKPOINT("Preparing");
	IntRBTree T;
	for (int i = 1; i <= 4; i++)
	{
		T.Add(i, NoValue{});
	}
	BOOST_REQUIRE_EQUAL(T.Num(), 4);

	BOOST_TEST_CHECKPOINT("Removing");
	BOOST_REQUIRE(T.Remove(2));
	BOOST_REQUIRE_EQUAL(T.Num(), 3);

	IntRBTree::KeyValueType ordered[3];
	T.CopyTo(ordered);
	BOOST_REQUIRE_EQUAL(ordered[0].Key, 1);
	BOOST_REQUIRE_EQUAL(ordered[1].Key, 3);
	BOOST_REQUIRE_EQUAL(ordered[2].Key, 4);
}

BOOST_AUTO_TEST_CASE
(
	RemoveFromRoot_ThatHasTwoChildren,
	*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/RemoveSuite/RemoveFromRoot_ThatHasOnlyLeftChild_OnlyTwoElements")
	*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/RemoveSuite/RemoveFromRoot_ThatHasOnlyRightChild_OnlyTwoElements")
)
{
	constexpr int KEY_ROOT = 2;
	constexpr int KEY_LEFT_CHILD = 1;
	constexpr int KEY_RIGHT_CHILD = 3;
	constexpr int KEY_NOT_EXISTING = 9;

	BOOST_TEST_CHECKPOINT("Preparing");
	IntRBTree T;
	T.Add(KEY_ROOT, NoValue{});
	T.Add(KEY_LEFT_CHILD, NoValue{});
	T.Add(KEY_RIGHT_CHILD, NoValue{});
	BOOST_REQUIRE_EQUAL(T.Num(), 3);

	BOOST_TEST_CHECKPOINT("Removing non-existing key");
	BOOST_REQUIRE(!T.Remove(KEY_NOT_EXISTING));
	BOOST_REQUIRE(!T.Empty());

	BOOST_TEST_CHECKPOINT("Removing");
	BOOST_REQUIRE(T.Remove(KEY_ROOT));
	BOOST_REQUIRE_EQUAL(T.Num(), 2);
	BOOST_REQUIRE_EQUAL(T.Min().Key, KEY_LEFT_CHILD);
	BOOST_REQUIRE_EQUAL(T.Max().Key, KEY_RIGHT_CHILD);
}

BOOST_AUTO_TEST_CASE(RemoveLeftChildOfRoot_OnlyTwoElements)
{
	constexpr int KEY_ROOT = 2;
	constexpr int KEY_LEFT_CHILD = 1;

	BOOST_TEST_CHECKPOINT("Preparing");
	IntRBTree T;
	T.Add(KEY_ROOT, NoValue{});
	T.Add(KEY_LEFT_CHILD, NoValue{});
	BOOST_REQUIRE_EQUAL(T.Num(), 2);

	BOOST_TEST_CHECKPOINT("Removing");
	BOOST_REQUIRE(T.Remove(KEY_LEFT_CHILD));
	BOOST_REQUIRE_EQUAL(T.Num(), 1);
	BOOST_REQUIRE_EQUAL(T.Min().Key, KEY_ROOT);
	BOOST_REQUIRE_EQUAL(T.Max().Key, KEY_ROOT);
}

BOOST_AUTO_TEST_CASE(RemoveLeftChildOfRoot_LeftChildHasTwoChildren)
{
	constexpr int KEY_ROOT = 5;
	constexpr int KEY_LEFT_CHILD = 3;
	constexpr int KEY_RIGHT_CHILD = 7;
	constexpr int KEY_LEFT_LEFT_CHILD_CHILD = 1;
	constexpr int KEY_LEFT_RIGHT_CHILD_CHILD = 2;
	constexpr int INITIAL_COUNT = 5;

	BOOST_TEST_CHECKPOINT("Preparing");
	IntRBTree T;
	BOOST_REQUIRE(T.Add(KEY_ROOT, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_LEFT_CHILD, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_RIGHT_CHILD, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_LEFT_LEFT_CHILD_CHILD, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_LEFT_RIGHT_CHILD_CHILD, NoValue{}));
	BOOST_REQUIRE_EQUAL(T.Num(), INITIAL_COUNT);

	BOOST_TEST_CHECKPOINT("Removing");
	BOOST_REQUIRE(T.Remove(KEY_LEFT_CHILD));
	BOOST_REQUIRE_EQUAL(T.Num(), (INITIAL_COUNT - 1));
	IntRBTree::KeyValueType ordered[(INITIAL_COUNT - 1)];
	T.CopyTo(ordered);
	BOOST_REQUIRE_EQUAL(ordered[0].Key, KEY_LEFT_LEFT_CHILD_CHILD);
	BOOST_REQUIRE_EQUAL(ordered[1].Key, KEY_LEFT_RIGHT_CHILD_CHILD);
	BOOST_REQUIRE_EQUAL(ordered[2].Key, KEY_ROOT);
	BOOST_REQUIRE_EQUAL(ordered[3].Key, KEY_RIGHT_CHILD);
}

BOOST_AUTO_TEST_CASE(RemoveLeftChildOfRoot_DeletableHasLeftChild_BrotherHasOneRedChild)
{
	constexpr int KEY_ROOT = 5;
	constexpr int KEY_LEFT_CHILD = 3;
	constexpr int KEY_RIGHT_CHILD = 7;
	constexpr int KEY_LEFT_LEFT_CHILD_CHILD = 1;
	constexpr int KEY_BROTHER_RIGHT_CHILD = 8;
	constexpr int INITIAL_COUNT = 5;

	BOOST_TEST_CHECKPOINT("Preparing");
	IntRBTree T;
	BOOST_REQUIRE(T.Add(KEY_ROOT, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_LEFT_CHILD, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_RIGHT_CHILD, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_LEFT_LEFT_CHILD_CHILD, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_BROTHER_RIGHT_CHILD, NoValue{}));
	BOOST_REQUIRE_EQUAL(T.Num(), INITIAL_COUNT);

	BOOST_TEST_CHECKPOINT("Removing");
	BOOST_REQUIRE(T.Remove(KEY_LEFT_CHILD));
	BOOST_REQUIRE_EQUAL(T.Num(), (INITIAL_COUNT - 1));
	IntRBTree::KeyValueType ordered[(INITIAL_COUNT - 1)];
	T.CopyTo(ordered);
	BOOST_REQUIRE_EQUAL(ordered[0].Key, KEY_LEFT_LEFT_CHILD_CHILD);
	BOOST_REQUIRE_EQUAL(ordered[1].Key, KEY_ROOT);
	BOOST_REQUIRE_EQUAL(ordered[2].Key, KEY_RIGHT_CHILD);
	BOOST_REQUIRE_EQUAL(ordered[3].Key, KEY_BROTHER_RIGHT_CHILD);
}



BOOST_AUTO_TEST_CASE(RemoveRightChildOfLeftChildOfRoot_LeftChildOfRootHasTwoChildren)
{
	constexpr int KEY_ROOT = 5;
	constexpr int KEY_LEFT_CHILD = 3;
	constexpr int KEY_RIGHT_CHILD = 7;
	constexpr int KEY_LEFT_LEFT_CHILD_CHILD = 1;
	constexpr int KEY_LEFT_RIGHT_CHILD_CHILD = 2;
	constexpr int INITIAL_COUNT = 5;

	BOOST_TEST_CHECKPOINT("Preparing");
	IntRBTree T;
	BOOST_REQUIRE(T.Add(KEY_ROOT, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_LEFT_CHILD, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_RIGHT_CHILD, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_LEFT_LEFT_CHILD_CHILD, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_LEFT_RIGHT_CHILD_CHILD, NoValue{}));
	BOOST_REQUIRE_EQUAL(T.Num(), INITIAL_COUNT);

	BOOST_TEST_CHECKPOINT("Removing");
	// WARNING!!! It's correct, Right child of left child of root after balancing is old KEY_LEFT_CHILD)
	BOOST_REQUIRE(T.Remove(KEY_LEFT_RIGHT_CHILD_CHILD));
	BOOST_REQUIRE_EQUAL(T.Num(), (INITIAL_COUNT - 1));
	IntRBTree::KeyValueType ordered[(INITIAL_COUNT - 1)];
	T.CopyTo(ordered);
	BOOST_REQUIRE_EQUAL(ordered[0].Key, KEY_LEFT_LEFT_CHILD_CHILD);
	BOOST_REQUIRE_EQUAL(ordered[1].Key, KEY_LEFT_CHILD);
	BOOST_REQUIRE_EQUAL(ordered[2].Key, KEY_ROOT);
	BOOST_REQUIRE_EQUAL(ordered[3].Key, KEY_RIGHT_CHILD);
}

BOOST_AUTO_TEST_CASE(RemoveRightChildOfRoot_ChildHasTwoChildren)
{
	constexpr int KEY_ROOT = 5;
	constexpr int KEY_LEFT_CHILD = 3;
	constexpr int KEY_RIGHT_CHILD = 7;
	constexpr int KEY_RIGHT_LEFT_CHILD_CHILD = 6;
	constexpr int KEY_RIGHT_RIGHT_CHILD_CHILD = 8;
	constexpr int INITIAL_COUNT = 5;

	BOOST_TEST_CHECKPOINT("Preparing");
	IntRBTree T;
	BOOST_REQUIRE(T.Add(KEY_ROOT, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_LEFT_CHILD, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_RIGHT_CHILD, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_RIGHT_LEFT_CHILD_CHILD, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_RIGHT_RIGHT_CHILD_CHILD, NoValue{}));
	BOOST_REQUIRE_EQUAL(T.Num(), INITIAL_COUNT);

	BOOST_TEST_CHECKPOINT("Removing");
	BOOST_REQUIRE(T.Remove(KEY_RIGHT_CHILD));
	BOOST_REQUIRE_EQUAL(T.Num(), (INITIAL_COUNT - 1));
	IntRBTree::KeyValueType ordered[(INITIAL_COUNT - 1)];
	T.CopyTo(ordered);
	BOOST_REQUIRE_EQUAL(ordered[0].Key, KEY_LEFT_CHILD);
	BOOST_REQUIRE_EQUAL(ordered[1].Key, KEY_ROOT);
	BOOST_REQUIRE_EQUAL(ordered[2].Key, KEY_RIGHT_LEFT_CHILD_CHILD);
	BOOST_REQUIRE_EQUAL(ordered[3].Key, KEY_RIGHT_RIGHT_CHILD_CHILD);
}

BOOST_AUTO_TEST_CASE
(
	RemoveRoot_TwoTireTree,
	*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/RemoveSuite/RemoveFromRoot_ThatHasTwoChildren")
)
{
	constexpr int KEY_ROOT = 5;
	constexpr int KEY_LEFT_CHILD = 3;
	constexpr int KEY_RIGHT_CHILD = 7;
	constexpr int KEY_LEFT_LEFT_CHILD_CHILD = 1;
	constexpr int KEY_LEFT_RIGHT_CHILD_CHILD = 2;
	constexpr int INITIAL_COUNT = 5;

	BOOST_TEST_CHECKPOINT("Preparing");
	IntRBTree T;
	BOOST_REQUIRE(T.Add(KEY_ROOT, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_LEFT_CHILD, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_RIGHT_CHILD, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_LEFT_LEFT_CHILD_CHILD, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_LEFT_RIGHT_CHILD_CHILD, NoValue{}));
	BOOST_REQUIRE_EQUAL(T.Num(), INITIAL_COUNT);

	BOOST_TEST_CHECKPOINT("Removing");
	BOOST_REQUIRE(T.Remove(KEY_ROOT));
	BOOST_REQUIRE_EQUAL(T.Num(), (INITIAL_COUNT - 1));

	BOOST_TEST_CHECKPOINT("Checking");
	IntRBTree::KeyValueType ordered[(INITIAL_COUNT - 1)];
	T.CopyTo(ordered);
	BOOST_REQUIRE_EQUAL(ordered[0].Key, KEY_LEFT_LEFT_CHILD_CHILD);
	BOOST_REQUIRE_EQUAL(ordered[1].Key, KEY_LEFT_RIGHT_CHILD_CHILD);
	BOOST_REQUIRE_EQUAL(ordered[2].Key, KEY_LEFT_CHILD);
	BOOST_REQUIRE_EQUAL(ordered[3].Key, KEY_RIGHT_CHILD);
}

BOOST_AUTO_TEST_CASE(RemoveRightChildOfRoot_OnlyTwoElements)
{
	constexpr int KEY_ROOT = 1;
	constexpr int KEY_RIGHT_CHILD = 2;

	BOOST_TEST_CHECKPOINT("Preparing");
	IntRBTree T;
	T.Add(KEY_ROOT, NoValue{});
	T.Add(KEY_RIGHT_CHILD, NoValue{});
	BOOST_REQUIRE_EQUAL(T.Num(), 2);

	BOOST_TEST_CHECKPOINT("Removing");
	BOOST_REQUIRE(T.Remove(KEY_RIGHT_CHILD));
	BOOST_REQUIRE_EQUAL(T.Num(), 1);
	BOOST_REQUIRE_EQUAL(T.Min().Key, KEY_ROOT);
	BOOST_REQUIRE_EQUAL(T.Max().Key, KEY_ROOT);
}

BOOST_AUTO_TEST_CASE(RemoveLeftLeaf_TwoTire)
{
	constexpr int KEY_FIRST = 3;
	constexpr int KEY_SECOND = 1;
	constexpr int KEY_THIRD = 4;
	constexpr int KEY_FOURTH = 0;
	constexpr int INITIAL_COUNT = 4;

	BOOST_TEST_CHECKPOINT("Preparing");
	IntRBTree T;
	T.Add(KEY_FIRST, NoValue{});
	T.Add(KEY_SECOND, NoValue{});
	T.Add(KEY_THIRD, NoValue{});
	T.Add(KEY_FOURTH, NoValue{});
	BOOST_REQUIRE_EQUAL(T.Num(), INITIAL_COUNT);

	BOOST_TEST_CHECKPOINT("Removing");
	BOOST_REQUIRE(T.Remove(KEY_FOURTH));
	BOOST_REQUIRE_EQUAL(T.Num(), (INITIAL_COUNT - 1));

	BOOST_TEST_CHECKPOINT("Checking");
	IntRBTree::KeyValueType ordered[(INITIAL_COUNT - 1)];
	T.CopyTo(ordered);
	BOOST_REQUIRE_EQUAL(ordered[0].Key, KEY_SECOND);
	BOOST_REQUIRE_EQUAL(ordered[1].Key, KEY_FIRST);
	BOOST_REQUIRE_EQUAL(ordered[2].Key, KEY_THIRD);
}

BOOST_AUTO_TEST_CASE(RemoveRightLeaf_TwoTire)
{
	constexpr int KEY_FIRST = 3;
	constexpr int KEY_SECOND = 1;
	constexpr int KEY_THIRD = 4;
	constexpr int KEY_FOURTH = 2;
	constexpr int INITIAL_COUNT = 4;

	BOOST_TEST_CHECKPOINT("Preparing");
	IntRBTree T;
	T.Add(KEY_FIRST, NoValue{});
	T.Add(KEY_SECOND, NoValue{});
	T.Add(KEY_THIRD, NoValue{});
	T.Add(KEY_FOURTH, NoValue{});
	BOOST_REQUIRE_EQUAL(T.Num(), INITIAL_COUNT);

	BOOST_TEST_CHECKPOINT("Removing");
	BOOST_REQUIRE(T.Remove(KEY_FOURTH));
	BOOST_REQUIRE_EQUAL(T.Num(), (INITIAL_COUNT - 1));

	BOOST_TEST_CHECKPOINT("Checking");
	IntRBTree::KeyValueType ordered[(INITIAL_COUNT - 1)];
	T.CopyTo(ordered);
	BOOST_REQUIRE_EQUAL(ordered[0].Key, KEY_SECOND);
	BOOST_REQUIRE_EQUAL(ordered[1].Key, KEY_FIRST);
	BOOST_REQUIRE_EQUAL(ordered[2].Key, KEY_THIRD);
}

BOOST_AUTO_TEST_CASE(RemoveInternalNode_WithOnlyLeftRedChild)
{
	constexpr int KEY_FIRST = 3;
	constexpr int KEY_SECOND = 1;
	constexpr int KEY_THIRD = 4;
	constexpr int KEY_FOURTH = 0;
	constexpr int INITIAL_COUNT = 4;

	BOOST_TEST_CHECKPOINT("Preparing");
	IntRBTree T;
	T.Add(KEY_FIRST, NoValue{});
	T.Add(KEY_SECOND, NoValue{});
	T.Add(KEY_THIRD, NoValue{});
	T.Add(KEY_FOURTH, NoValue{});
	BOOST_REQUIRE_EQUAL(T.Num(), INITIAL_COUNT);

	BOOST_TEST_CHECKPOINT("Removing");
	BOOST_REQUIRE(T.Remove(KEY_SECOND));
	BOOST_REQUIRE_EQUAL(T.Num(), (INITIAL_COUNT - 1));

	BOOST_TEST_CHECKPOINT("Checking");
	IntRBTree::KeyValueType ordered[(INITIAL_COUNT - 1)];
	T.CopyTo(ordered);
	BOOST_REQUIRE_EQUAL(ordered[0].Key, KEY_FOURTH);
	BOOST_REQUIRE_EQUAL(ordered[1].Key, KEY_FIRST);
	BOOST_REQUIRE_EQUAL(ordered[2].Key, KEY_THIRD);
}

BOOST_AUTO_TEST_CASE(RemoveInternalNode_WithOnlyRightRedChild)
{
	constexpr int KEY_FIRST = 3;
	constexpr int KEY_SECOND = 1;
	constexpr int KEY_THIRD = 4;
	constexpr int KEY_FOURTH = 2;
	constexpr int INITIAL_COUNT = 4;

	BOOST_TEST_CHECKPOINT("Preparing");
	IntRBTree T;
	T.Add(KEY_FIRST, NoValue{});
	T.Add(KEY_SECOND, NoValue{});
	T.Add(KEY_THIRD, NoValue{});
	T.Add(KEY_FOURTH, NoValue{});
	BOOST_REQUIRE_EQUAL(T.Num(), INITIAL_COUNT);

	BOOST_TEST_CHECKPOINT("Removing");
	BOOST_REQUIRE(T.Remove(KEY_SECOND));
	BOOST_REQUIRE_EQUAL(T.Num(), (INITIAL_COUNT - 1));

	BOOST_TEST_CHECKPOINT("Checking");
	IntRBTree::KeyValueType ordered[(INITIAL_COUNT - 1)];
	T.CopyTo(ordered);
	BOOST_REQUIRE_EQUAL(ordered[0].Key, KEY_FOURTH);
	BOOST_REQUIRE_EQUAL(ordered[1].Key, KEY_FIRST);
	BOOST_REQUIRE_EQUAL(ordered[2].Key, KEY_THIRD);
}

BOOST_AUTO_TEST_CASE(RemoveInternalNode_WithOnlyRightRedChild_OnlyLeftRedChildOfBrother)
{
	constexpr int KEY_FIRST = 4;
	constexpr int KEY_SECOND = 1;
	constexpr int KEY_THIRD = 6;
	constexpr int KEY_FOURTH = 3;
	constexpr int KEY_FIVE = 5;
	constexpr int INITIAL_COUNT = 5;


	BOOST_TEST_CHECKPOINT("Preparing");
	IntRBTree T;
	T.Add(KEY_FIRST, NoValue{});
	T.Add(KEY_SECOND, NoValue{});
	T.Add(KEY_THIRD, NoValue{});
	T.Add(KEY_FOURTH, NoValue{});
	T.Add(KEY_FIVE, NoValue{});
	BOOST_REQUIRE_EQUAL(T.Num(), INITIAL_COUNT);

	BOOST_TEST_CHECKPOINT("Removing");
	BOOST_REQUIRE(T.Remove(KEY_SECOND));
	BOOST_REQUIRE_EQUAL(T.Num(), (INITIAL_COUNT - 1));

	BOOST_TEST_CHECKPOINT("Checking");
	IntRBTree::KeyValueType ordered[(INITIAL_COUNT - 1)];
	T.CopyTo(ordered);
	BOOST_REQUIRE_EQUAL(ordered[0].Key, KEY_FOURTH);
	BOOST_REQUIRE_EQUAL(ordered[1].Key, KEY_FIRST);
	BOOST_REQUIRE_EQUAL(ordered[2].Key, KEY_FIVE);
	BOOST_REQUIRE_EQUAL(ordered[3].Key, KEY_THIRD);
}

BOOST_AUTO_TEST_CASE(RemoveInternalNode_WithOnlyRightRedChild_OnlyRightRedChildOfBrother)
{
	constexpr int KEY_FIRST = 4;
	constexpr int KEY_SECOND = 1;
	constexpr int KEY_THIRD = 6;
	constexpr int KEY_FOURTH = 3;
	constexpr int KEY_FIVE = 7;
	constexpr int INITIAL_COUNT = 5;


	BOOST_TEST_CHECKPOINT("Preparing");
	IntRBTree T;
	T.Add(KEY_FIRST, NoValue{});
	T.Add(KEY_SECOND, NoValue{});
	T.Add(KEY_THIRD, NoValue{});
	T.Add(KEY_FOURTH, NoValue{});
	T.Add(KEY_FIVE, NoValue{});
	BOOST_REQUIRE_EQUAL(T.Num(), INITIAL_COUNT);

	BOOST_TEST_CHECKPOINT("Removing");
	BOOST_REQUIRE(T.Remove(KEY_SECOND));
	BOOST_REQUIRE_EQUAL(T.Num(), (INITIAL_COUNT - 1));

	BOOST_TEST_CHECKPOINT("Checking");
	IntRBTree::KeyValueType ordered[(INITIAL_COUNT - 1)];
	T.CopyTo(ordered);
	BOOST_REQUIRE_EQUAL(ordered[0].Key, KEY_FOURTH);
	BOOST_REQUIRE_EQUAL(ordered[1].Key, KEY_FIRST);
	BOOST_REQUIRE_EQUAL(ordered[2].Key, KEY_THIRD);
	BOOST_REQUIRE_EQUAL(ordered[3].Key, KEY_FIVE);
}

BOOST_AUTO_TEST_CASE(RemoveInternalNode_WithOnlyRightRedChild_BothRedChildrenOfBlackBrother)
{
	constexpr int KEY_FIRST = 4;
	constexpr int KEY_SECOND = 1;
	constexpr int KEY_THIRD = 6;
	constexpr int KEY_FOURTH = 3;
	constexpr int KEY_FIVE = 7;
	constexpr int KEY_SIX = 5;
	constexpr int INITIAL_COUNT = 6;


	BOOST_TEST_CHECKPOINT("Preparing");
	IntRBTree T;
	T.Add(KEY_FIRST, NoValue{});
	T.Add(KEY_SECOND, NoValue{});
	T.Add(KEY_THIRD, NoValue{});
	T.Add(KEY_FOURTH, NoValue{});
	T.Add(KEY_FIVE, NoValue{});
	T.Add(KEY_SIX, NoValue{});
	BOOST_REQUIRE_EQUAL(T.Num(), INITIAL_COUNT);

	BOOST_TEST_CHECKPOINT("Removing");
	BOOST_REQUIRE(T.Remove(KEY_SECOND));
	BOOST_REQUIRE_EQUAL(T.Num(), (INITIAL_COUNT - 1));

	BOOST_TEST_CHECKPOINT("Checking");
	IntRBTree::KeyValueType ordered[(INITIAL_COUNT - 1)];
	T.CopyTo(ordered);
	BOOST_REQUIRE_EQUAL(ordered[0].Key, KEY_FOURTH);
	BOOST_REQUIRE_EQUAL(ordered[1].Key, KEY_FIRST);
	BOOST_REQUIRE_EQUAL(ordered[2].Key, KEY_SIX);
	BOOST_REQUIRE_EQUAL(ordered[3].Key, KEY_THIRD);
	BOOST_REQUIRE_EQUAL(ordered[4].Key, KEY_FIVE);
}

BOOST_AUTO_TEST_CASE(RemoveInternalNode_FullTwoTireTree)
{
	constexpr int KEY_FIRST = 4;
	constexpr int KEY_SECOND = 1;
	constexpr int KEY_THIRD = 6;
	constexpr int KEY_FOURTH = 3;
	constexpr int KEY_FIVE = 7;
	constexpr int KEY_SIX = 5;
	constexpr int KEY_SEVEN = 0;
	constexpr int INITIAL_COUNT = 7;


	BOOST_TEST_CHECKPOINT("Preparing");
	IntRBTree T;
	T.Add(KEY_FIRST, NoValue{});
	T.Add(KEY_SECOND, NoValue{});
	T.Add(KEY_THIRD, NoValue{});
	T.Add(KEY_FOURTH, NoValue{});
	T.Add(KEY_FIVE, NoValue{});
	T.Add(KEY_SIX, NoValue{});
	T.Add(KEY_SEVEN, NoValue{});
	BOOST_REQUIRE_EQUAL(T.Num(), INITIAL_COUNT);

	BOOST_TEST_CHECKPOINT("Removing");
	BOOST_REQUIRE(T.Remove(KEY_SECOND));
	BOOST_REQUIRE_EQUAL(T.Num(), (INITIAL_COUNT - 1));

	BOOST_TEST_CHECKPOINT("Checking");
	IntRBTree::KeyValueType ordered[(INITIAL_COUNT - 1)];
	T.CopyTo(ordered);
	BOOST_REQUIRE_EQUAL(ordered[0].Key, KEY_SEVEN);
	BOOST_REQUIRE_EQUAL(ordered[1].Key, KEY_FOURTH);
	BOOST_REQUIRE_EQUAL(ordered[2].Key, KEY_FIRST);
	BOOST_REQUIRE_EQUAL(ordered[3].Key, KEY_SIX);
	BOOST_REQUIRE_EQUAL(ordered[4].Key, KEY_THIRD);
	BOOST_REQUIRE_EQUAL(ordered[5].Key, KEY_FIVE);
}

BOOST_AUTO_TEST_CASE(RemoveInternalNode_WithOnlyLeftRedChild_RedBrother)
{
	constexpr int KEY_FIRST = 2;
	constexpr int KEY_SECOND = 1;
	constexpr int KEY_THIRD = 6;
	constexpr int KEY_FOURTH = 0;
	constexpr int KEY_FIVE = 3;
	constexpr int KEY_SIX = 7;
	constexpr int KEY_SEVEN = 4;
	constexpr int INITIAL_COUNT = 7;

	BOOST_TEST_CHECKPOINT("Preparing");
	IntRBTree T;
	T.Add(KEY_FIRST, NoValue{});
	T.Add(KEY_SECOND, NoValue{});
	T.Add(KEY_THIRD, NoValue{});
	T.Add(KEY_FOURTH, NoValue{});
	T.Add(KEY_FIVE, NoValue{});
	T.Add(KEY_SIX, NoValue{});
	T.Add(KEY_SEVEN, NoValue{});
	BOOST_REQUIRE_EQUAL(T.Num(), INITIAL_COUNT);

	BOOST_TEST_CHECKPOINT("Removing");
	BOOST_REQUIRE(T.Remove(KEY_SECOND));
	BOOST_REQUIRE_EQUAL(T.Num(), (INITIAL_COUNT - 1));

	BOOST_TEST_CHECKPOINT("Checking");
	IntRBTree::KeyValueType ordered[(INITIAL_COUNT - 1)];
	T.CopyTo(ordered);
	BOOST_REQUIRE_EQUAL(ordered[0].Key, KEY_FOURTH);
	BOOST_REQUIRE_EQUAL(ordered[1].Key, KEY_FIRST);
	BOOST_REQUIRE_EQUAL(ordered[2].Key, KEY_FIVE);
	BOOST_REQUIRE_EQUAL(ordered[3].Key, KEY_SEVEN);
	BOOST_REQUIRE_EQUAL(ordered[4].Key, KEY_THIRD);
	BOOST_REQUIRE_EQUAL(ordered[5].Key, KEY_SIX);
}

BOOST_AUTO_TEST_CASE(RemoveInternalBlackNode_WithTwoBlackChildren_BrotherWith_RightRedLeftBlackChildren)
{
	constexpr int INITIAL_COUNT = 10;

	BOOST_TEST_CHECKPOINT("Preparing");
	IntRBTree T;
	for (int i = 1; i <= INITIAL_COUNT; i++)
	{
		BOOST_REQUIRE(T.Add(i, NoValue{}));
	}
	BOOST_REQUIRE_EQUAL(T.Num(), INITIAL_COUNT);

	BOOST_TEST_CHECKPOINT("Removing");
	BOOST_REQUIRE(T.Remove(2));
	BOOST_REQUIRE_EQUAL(T.Num(), (INITIAL_COUNT - 1));

	BOOST_TEST_CHECKPOINT("Checking");
	IntRBTree::KeyValueType ordered[(INITIAL_COUNT - 1)];
	T.CopyTo(ordered);
	int curr_index = 0;
	for (int ref_key = 1; ref_key <= INITIAL_COUNT; ref_key++)
	{
		if (ref_key != 2)
		{
			BOOST_REQUIRE_EQUAL(ordered[curr_index].Key, ref_key);
			curr_index++;
		}
	}
}

BOOST_AUTO_TEST_CASE(RemoveInternalBlackNode_WithTwoBlackChildren_BrotherWith_RightBlackLeftRedChildren)
{
	constexpr int INITIAL_COUNT = 10;

	BOOST_TEST_CHECKPOINT("Preparing");
	IntRBTree T;
	for (int i = INITIAL_COUNT; i >= 1; i--)
	{
		BOOST_REQUIRE(T.Add(i, NoValue{}));
	}
	BOOST_REQUIRE_EQUAL(T.Num(), INITIAL_COUNT);

	BOOST_TEST_CHECKPOINT("Removing");
	BOOST_REQUIRE(T.Remove(9));
	BOOST_REQUIRE_EQUAL(T.Num(), (INITIAL_COUNT - 1));

	BOOST_TEST_CHECKPOINT("Checking");
	IntRBTree::KeyValueType ordered[(INITIAL_COUNT - 1)];
	T.CopyTo(ordered);
	int curr_index = 0;
	for (int ref_key = 1; ref_key <= INITIAL_COUNT; ref_key++)
	{
		if (ref_key != 9)
		{
			BOOST_REQUIRE_EQUAL(ordered[curr_index].Key, ref_key);
			curr_index++;
		}
	}
}

BOOST_AUTO_TEST_CASE(RemoveInternalBlackNode_RedNode_WithTwoBlackChildren)
{
	constexpr int INITIAL_COUNT = 6;

	BOOST_TEST_CHECKPOINT("Preparing");
	IntRBTree T;
	for (int i = 1; i <= INITIAL_COUNT; i++)
	{
		BOOST_REQUIRE(T.Add(i, NoValue{}));
	}
	BOOST_REQUIRE_EQUAL(T.Num(), INITIAL_COUNT);

	BOOST_TEST_CHECKPOINT("Removing");
	BOOST_REQUIRE(T.Remove(4));
	BOOST_REQUIRE_EQUAL(T.Num(), (INITIAL_COUNT - 1));

	BOOST_TEST_CHECKPOINT("Checking");
	IntRBTree::KeyValueType ordered[(INITIAL_COUNT - 1)];
	T.CopyTo(ordered);
	int curr_index = 0;
	for (int ref_key = 1; ref_key <= INITIAL_COUNT; ref_key++)
	{
		if (ref_key != 4)
		{
			BOOST_REQUIRE_EQUAL(ordered[curr_index].Key, ref_key);
			curr_index++;
		}
	}
}


BOOST_AUTO_TEST_SUITE_END() // RemoveSuite


	BOOST_AUTO_TEST_SUITE_END() // Minimal
	BOOST_AUTO_TEST_SUITE_END() // TRBTreeTestSuite
	BOOST_AUTO_TEST_SUITE_END() // Container
	BOOST_AUTO_TEST_SUITE_END() // Core
} // anonymous