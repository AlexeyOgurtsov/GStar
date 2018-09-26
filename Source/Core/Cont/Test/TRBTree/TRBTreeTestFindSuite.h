#pragma once

#include "TRBTreeTestUtils.h"

namespace
{
	BOOST_AUTO_TEST_SUITE(Core)
	BOOST_AUTO_TEST_SUITE(Container)
	BOOST_AUTO_TEST_SUITE(TRBTreeTestSuite)

BOOST_AUTO_TEST_SUITE
(
	ExtraFindSuite,
	*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/AddSuite")
	*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/RemoveSuite")
	*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/IterationSuite")
)

BOOST_AUTO_TEST_CASE
(
	FindValueTest,
	*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/ExtraFindSuite/FindIterator")
)
{
	std::string const VALUE_TO_FIND { "FIND" };
	std::string const OTHER_VALUE { "OTHER_VALUE" };

	BOOST_TEST_CHECKPOINT("Empty");
	IntStringRBTree T;
	const IntStringRBTree* pCT = &T;
	BOOST_REQUIRE( ! pCT->ContainsValue(0) );
	BOOST_REQUIRE( ! pCT->ContainsOtherValue(0) );

	BOOST_REQUIRE( ! pCT->ContainsValue_InRange(0, pCT->Iterator(), pCT->EndIterator() ) );
	BOOST_REQUIRE( ! pCT->ContainsOtherValue_InRange(0, pCT->Iterator(), pCT->EndIterator() ) );

	IntStringRBTree::IteratorType It = T.FindIteratorByValue(0);
	BOOST_REQUIRE( It.IsEnd() );
	IntStringRBTree::ConstIteratorType ConstIt = pCT->FindIteratorByValue(0);
	BOOST_REQUIRE( ConstIt.IsEnd() );

	IntStringRBTree::IteratorType ItNot = T.FindIteratorByNotValue(0);
	BOOST_REQUIRE(ItNot.IsEnd());
	IntStringRBTree::ConstIteratorType ConstItNot = pCT->FindIteratorByNotValue(0);
	BOOST_REQUIRE(ConstItNot.IsEnd());

	IntStringRBTree::IteratorType It_InRange = T.FindIteratorByValue_InRange(0, T.Iterator(), T.EndIterator());
	BOOST_REQUIRE(It_InRange.IsEnd());
	IntStringRBTree::ConstIteratorType ConstIt_InRange = pCT->FindIteratorByValue_InRange(0, pCT->Iterator(), pCT->EndIterator());
	BOOST_REQUIRE(ConstIt_InRange.IsEnd());

	IntStringRBTree::IteratorType ItNotValue_InRange = T.FindIteratorByNotValue_InRange(0, T.Iterator(), T.EndIterator());
	BOOST_REQUIRE(ItNotValue_InRange.IsEnd());
	IntStringRBTree::ConstIteratorType ConstItNotValue_InRange = pCT->FindIteratorByNotValue_InRange(0, pCT->Iterator(), pCT->EndIterator());
	BOOST_REQUIRE(ConstItNotValue_InRange.IsEnd());

	BOOST_TEST_CHECKPOINT("Filling");
	IntStringRBTree::IteratorType const RefIt_First = T.GetOrAdd(1, OTHER_VALUE);
	IntStringRBTree::IteratorType const RefIt_FirstOtherValue = T.GetOrAdd(2, OTHER_VALUE);
	IntStringRBTree::IteratorType const RefIt_Value = T.GetOrAdd(3, VALUE_TO_FIND);
	IntStringRBTree::IteratorType const RefIt_SecondOtherValue = T.GetOrAdd(4, OTHER_VALUE);
	IntStringRBTree::IteratorType const RefIt_OtherValue = T.GetOrAdd(5, VALUE_TO_FIND);



	BOOST_TEST_CHECKPOINT("Find");
	It = T.FindIteratorByValue(VALUE_TO_FIND);
	BOOST_REQUIRE(It);
	BOOST_REQUIRE_EQUAL(It.GetValue(), VALUE_TO_FIND);
	BOOST_REQUIRE(It == RefIt_Value);

	BOOST_TEST_CHECKPOINT("Find In Range");
	It_InRange = T.FindIteratorByValue_InRange(VALUE_TO_FIND, RefIt_SecondOtherValue, T.EndIterator());
	BOOST_REQUIRE(It_InRange);
	BOOST_REQUIRE(It_InRange == RefIt_OtherValue);
}

BOOST_AUTO_TEST_CASE
(
	FindByPredicateTest,
	*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/ExtraFindSuite/FindIterator")
)
{
	constexpr int INITIAL_COUNT = 10;
	constexpr int KEY_TO_SEARCH = 5;
	constexpr int KEY_FIRST = 1;
	constexpr int KEY_AFTER_FIRST = 2;

	BOOST_TEST_CHECKPOINT("Empty");
	IntRBTree T;
	const IntRBTree* pCT = &T;
	IntRBTree::ConstIteratorType It = pCT->FindIteratorByPredicate
	(
		[KEY_TO_SEARCH](const IntRBTree::KeyValueType& KV)->bool { return KV.Key == KEY_TO_SEARCH; }
	);
	IntRBTree::ConstIteratorType ItKey = pCT->FindIteratorByKeyPredicate
	(
		[KEY_TO_SEARCH](const IntRBTree::KeyType& K)->bool { return K == KEY_TO_SEARCH; }
	);
	IntRBTree::IteratorType ItMutable = T.FindIteratorByPredicate
	(
		[KEY_TO_SEARCH](const IntRBTree::KeyValueType& KV)->bool { return KV.Key == KEY_TO_SEARCH; }
	);
	IntRBTree::IteratorType ItKeyMutable = T.FindIteratorByKeyPredicate
	(
		[KEY_TO_SEARCH](const IntRBTree::KeyType& K)->bool { return K == KEY_TO_SEARCH; }
	);
	IntRBTree::ConstIteratorType ItNotFirst = pCT->FindIteratorByNotPredicate
	(
		[KEY_FIRST](const IntRBTree::KeyValueType& KV)->bool { return KV.Key == KEY_FIRST; }
	);
	IntRBTree::ConstIteratorType ItNotKeyFirst = pCT->FindIteratorByNotKeyPredicate
	(
		[KEY_FIRST](const IntRBTree::KeyType& K)->bool { return K == KEY_FIRST; }
	);
	BOOST_REQUIRE(It.IsEnd());
	BOOST_REQUIRE(ItKey.IsEnd());
	BOOST_REQUIRE(ItMutable.IsEnd());
	BOOST_REQUIRE(ItKeyMutable.IsEnd());
	BOOST_REQUIRE(ItNotFirst.IsEnd());
	BOOST_REQUIRE(ItNotKeyFirst.IsEnd());
	BOOST_REQUIRE
	(
		! T.ContainsPredicate([KEY_TO_SEARCH](const IntRBTree::KeyValueType& KV)->bool { return KV.Key == KEY_TO_SEARCH; })
	);
	BOOST_REQUIRE
	(
		! T.ContainsKeyPredicate([KEY_TO_SEARCH](const IntRBTree::KeyType& K)->bool { return K == KEY_TO_SEARCH; })
	);
	BOOST_REQUIRE
	(
		! pCT->ContainsPredicate([KEY_TO_SEARCH](const IntRBTree::KeyValueType& KV)->bool { return KV.Key == KEY_TO_SEARCH; })
	);
	BOOST_REQUIRE
	(
		! pCT->ContainsKeyPredicate([KEY_TO_SEARCH](const IntRBTree::KeyType& K)->bool { return K == KEY_TO_SEARCH; })
	);
	BOOST_REQUIRE
	(
		! pCT->ContainsNotPredicate([KEY_TO_SEARCH](const IntRBTree::KeyValueType& KV)->bool { return KV.Key == KEY_TO_SEARCH; })
	);
	BOOST_REQUIRE
	(
		! pCT->ContainsNotKeyPredicate([KEY_TO_SEARCH](const IntRBTree::KeyType& K)->bool { return K == KEY_TO_SEARCH; })
	);

	BOOST_TEST_CHECKPOINT("Prepare");
	for (int i = 1; i <= INITIAL_COUNT; i++)
	{
		T.Add(i, NoValue{});
	}
	BOOST_REQUIRE_EQUAL(T.Num(), INITIAL_COUNT);

	BOOST_TEST_CHECKPOINT("Check");
	It = pCT->FindIteratorByPredicate
	(
		[KEY_TO_SEARCH](const IntRBTree::KeyValueType& KV)->bool { return KV.Key == KEY_TO_SEARCH; }
	);
	ItKey = pCT->FindIteratorByKeyPredicate
	(
		[KEY_TO_SEARCH](const IntRBTree::KeyType& K)->bool { return K == KEY_TO_SEARCH; }
	);
	ItNotFirst = pCT->FindIteratorByNotPredicate
	(
		[KEY_FIRST](const IntRBTree::KeyValueType& KV)->bool { return KV.Key == KEY_FIRST; }
	);
	ItNotKeyFirst = pCT->FindIteratorByNotKeyPredicate
	(
		[KEY_FIRST](const IntRBTree::KeyType& K)->bool { return K == KEY_FIRST; }
	);
	BOOST_REQUIRE(It);
	BOOST_REQUIRE_EQUAL(It.GetKey(), KEY_TO_SEARCH);
	BOOST_REQUIRE(ItKey);
	BOOST_REQUIRE_EQUAL(ItKey.GetKey(), KEY_TO_SEARCH);
	BOOST_REQUIRE(ItNotFirst);
	BOOST_REQUIRE_EQUAL(ItNotFirst.GetKey(), KEY_AFTER_FIRST);
	BOOST_REQUIRE(ItNotKeyFirst);
	BOOST_REQUIRE_EQUAL(ItNotKeyFirst.GetKey(), KEY_AFTER_FIRST);
	BOOST_REQUIRE
	(
		T.ContainsPredicate([KEY_TO_SEARCH](const IntRBTree::KeyValueType& KV)->bool { return KV.Key == KEY_TO_SEARCH; })
	);
	BOOST_REQUIRE
	(
		pCT->ContainsPredicate([KEY_TO_SEARCH](const IntRBTree::KeyValueType& KV)->bool { return KV.Key == KEY_TO_SEARCH; })
	);
	BOOST_REQUIRE
	(
		pCT->ContainsNotPredicate([KEY_FIRST](const IntRBTree::KeyValueType& KV)->bool { return KV.Key == KEY_FIRST; })
	);
}

BOOST_AUTO_TEST_CASE
(
	FindByPredicateRangeTest,
	*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/ExtraFindSuite/FindIterator")
)
{
	constexpr int INITIAL_COUNT = 10;
	constexpr int KEY_FIRST = 1;
	constexpr int KEY_TO_SEARCH = 5;
	constexpr int KEY_OUT_OF_RANGE = 8;
	constexpr int KEY_FIRST_IN_RANGE = 3;
	constexpr int KEY_AFTER_FIRST_IN_RANGE = 4;
	constexpr int KEY_RANGE_END = 7;

	BOOST_TEST_CHECKPOINT("Empty");
	IntRBTree T;
	const IntRBTree* pCT = &T;
	IntRBTree::ConstIteratorType It = pCT->FindIteratorByPredicate_InRange
	(
		[KEY_TO_SEARCH](const IntRBTree::KeyValueType& KV)->bool { return KV.Key == KEY_TO_SEARCH; },
		T.ConstIterator(), T.ConstEndIterator()
	);
	IntRBTree::ConstIteratorType ItKey = pCT->FindIteratorByKeyPredicate_InRange
	(
		[KEY_TO_SEARCH](const IntRBTree::KeyType& K)->bool { return K == KEY_TO_SEARCH; },
		T.ConstIterator(), T.ConstEndIterator()
	);
	IntRBTree::IteratorType ItMutable = T.FindIteratorByPredicate_InRange
	(
		[KEY_TO_SEARCH](const IntRBTree::KeyValueType& KV)->bool { return KV.Key == KEY_TO_SEARCH; },
		T.Iterator(), T.EndIterator()
	);
	IntRBTree::IteratorType ItKeyMutable = T.FindIteratorByKeyPredicate_InRange
	(
		[KEY_TO_SEARCH](const IntRBTree::KeyType& K)->bool { return K == KEY_TO_SEARCH; },
		T.Iterator(), T.EndIterator()
	);
	IntRBTree::ConstIteratorType ItNotFirst = pCT->FindIteratorByNotPredicate_InRange
	(
		[KEY_FIRST_IN_RANGE](const IntRBTree::KeyValueType& KV)->bool { return KV.Key == KEY_FIRST_IN_RANGE; },
		T.ConstIterator(), T.ConstEndIterator()
	);
	IntRBTree::ConstIteratorType ItNotKeyFirst = pCT->FindIteratorByNotKeyPredicate_InRange
	(
		[KEY_FIRST_IN_RANGE](const IntRBTree::KeyType& K)->bool { return K == KEY_FIRST_IN_RANGE; },
		T.ConstIterator(), T.ConstEndIterator()
	);
	BOOST_REQUIRE(It.IsEnd());
	BOOST_REQUIRE(ItKey.IsEnd());
	BOOST_REQUIRE(ItMutable.IsEnd());
	BOOST_REQUIRE(ItKeyMutable.IsEnd());
	BOOST_REQUIRE(ItNotFirst.IsEnd());
	BOOST_REQUIRE(ItNotKeyFirst.IsEnd());
	BOOST_REQUIRE
	(
		! T.ContainsPredicate_InRange
		(
			[KEY_TO_SEARCH](const IntRBTree::KeyValueType& KV)->bool { return KV.Key == KEY_TO_SEARCH; },
			T.ConstIterator(), T.ConstEndIterator()
		)
	);
	BOOST_REQUIRE
	(
		! T.ContainsKeyPredicate_InRange
		(
			[KEY_TO_SEARCH](const IntRBTree::KeyType& K)->bool { return K == KEY_TO_SEARCH; },
			T.ConstIterator(), T.ConstEndIterator()
		)
	);
	BOOST_REQUIRE
	(
		! pCT->ContainsPredicate_InRange
		(
			[KEY_TO_SEARCH](const IntRBTree::KeyValueType& KV)->bool { return KV.Key == KEY_TO_SEARCH; },
			T.ConstIterator(), T.ConstEndIterator()
		)
	);
	BOOST_REQUIRE
	(
		! pCT->ContainsKeyPredicate_InRange
		(
			[KEY_TO_SEARCH](const IntRBTree::KeyType& K)->bool { return K == KEY_TO_SEARCH; },
			T.ConstIterator(), T.ConstEndIterator()
		)
	);
	BOOST_REQUIRE
	(
		! pCT->ContainsNotPredicate_InRange
		(
			[KEY_TO_SEARCH](const IntRBTree::KeyValueType& KV)->bool { return KV.Key == KEY_TO_SEARCH; },
			T.ConstIterator(), T.ConstEndIterator()
		)
	);
	BOOST_REQUIRE
	(
		! pCT->ContainsNotKeyPredicate_InRange
		(
			[KEY_TO_SEARCH](const IntRBTree::KeyType& K)->bool { return K == KEY_TO_SEARCH; },
			T.ConstIterator(), T.ConstEndIterator()
		)
	);

	BOOST_TEST_CHECKPOINT("Prepare");
	for (int i = 1; i <= INITIAL_COUNT; i++)
	{
		T.Add(i, NoValue{});
	}
	BOOST_REQUIRE_EQUAL(T.Num(), INITIAL_COUNT);

	BOOST_TEST_CHECKPOINT("Check");
	IntRBTree::IteratorType const RangeFirstIt = T.FindIteratorFor(KEY_FIRST_IN_RANGE);
	IntRBTree::IteratorType const RangeLastIt = T.FindIteratorFor(KEY_RANGE_END);
	IntRBTree::ConstIteratorType const RangeFirstConstIt = pCT->FindIteratorFor(KEY_FIRST_IN_RANGE);
	IntRBTree::ConstIteratorType const RangeLastConstIt = pCT->FindIteratorFor(KEY_RANGE_END);
	BOOST_REQUIRE(RangeFirstIt);
	BOOST_REQUIRE(RangeLastIt);
	BOOST_REQUIRE(RangeFirstConstIt);
	BOOST_REQUIRE(RangeLastConstIt);

	It = pCT->FindIteratorByPredicate_InRange
	(
		[KEY_TO_SEARCH](const IntRBTree::KeyValueType& KV)->bool { return KV.Key == KEY_TO_SEARCH; },
		RangeFirstConstIt, RangeLastConstIt
	);
	ItKey = pCT->FindIteratorByKeyPredicate_InRange
	(
		[KEY_TO_SEARCH](const IntRBTree::KeyType& K)->bool { return K == KEY_TO_SEARCH; },
		RangeFirstConstIt, RangeLastConstIt
	);
	ItNotFirst = pCT->FindIteratorByNotPredicate_InRange
	(
		[KEY_FIRST_IN_RANGE](const IntRBTree::KeyValueType& KV)->bool { return KV.Key == KEY_FIRST_IN_RANGE; },
		RangeFirstConstIt, RangeLastConstIt
	);
	ItNotKeyFirst = pCT->FindIteratorByNotKeyPredicate_InRange
	(
		[KEY_FIRST_IN_RANGE](const IntRBTree::KeyType& K)->bool { return K == KEY_FIRST_IN_RANGE; },
		RangeFirstConstIt, RangeLastConstIt
	);

	IntRBTree::ConstIteratorType ItOutOfRange = pCT->FindIteratorByPredicate_InRange
	(
		[KEY_OUT_OF_RANGE](const IntRBTree::KeyValueType& KV)->bool { return KV.Key == KEY_OUT_OF_RANGE; },
		RangeFirstConstIt, RangeLastConstIt
	);
	IntRBTree::ConstIteratorType ItKeyOutOfRange = pCT->FindIteratorByKeyPredicate_InRange
	(
		[KEY_OUT_OF_RANGE](const IntRBTree::KeyType& K)->bool { return K == KEY_OUT_OF_RANGE; },
		RangeFirstConstIt, RangeLastConstIt
	);

	IntRBTree::ConstIteratorType ItZeroInterval = pCT->FindIteratorByPredicate_InRange
	(
		[KEY_FIRST](const IntRBTree::KeyValueType& KV)->bool { return KV.Key == KEY_FIRST; },
		pCT->Iterator(), pCT->Iterator()
	);
	IntRBTree::ConstIteratorType ItKeyZeroInterval = pCT->FindIteratorByKeyPredicate_InRange
	(
		[KEY_FIRST](const IntRBTree::KeyType& K)->bool { return K == KEY_FIRST; },
		pCT->Iterator(), pCT->Iterator()
	);

	IntRBTree::ConstIteratorType ItZeroIntervalNot = pCT->FindIteratorByNotPredicate_InRange
	(
		[KEY_FIRST](const IntRBTree::KeyValueType& KV)->bool { return KV.Key == KEY_FIRST; },
		pCT->Iterator(), pCT->Iterator()
	);
	IntRBTree::ConstIteratorType ItKeyZeroIntervalNot = pCT->FindIteratorByNotKeyPredicate_InRange
	(
		[KEY_FIRST](const IntRBTree::KeyType& K)->bool { return K == KEY_FIRST; },
		pCT->Iterator(), pCT->Iterator()
	);

	BOOST_REQUIRE( ItOutOfRange.IsEnd() );
	BOOST_REQUIRE( ItKeyOutOfRange.IsEnd() );
	BOOST_REQUIRE( ItZeroInterval.IsEnd() );
	BOOST_REQUIRE( ItKeyZeroInterval.IsEnd() );
	BOOST_REQUIRE( ItZeroIntervalNot.IsEnd() );
	BOOST_REQUIRE( ItKeyZeroIntervalNot.IsEnd() );

	BOOST_REQUIRE(It);
	BOOST_REQUIRE_EQUAL(It.GetKey(), KEY_TO_SEARCH);

	BOOST_REQUIRE(ItKey);
	BOOST_REQUIRE_EQUAL(ItKey.GetKey(), KEY_TO_SEARCH);

	BOOST_REQUIRE(ItNotFirst);
	BOOST_REQUIRE_EQUAL(ItNotFirst.GetKey(), KEY_AFTER_FIRST_IN_RANGE);

	BOOST_REQUIRE(ItNotKeyFirst);
	BOOST_REQUIRE_EQUAL(ItNotKeyFirst.GetKey(), KEY_AFTER_FIRST_IN_RANGE);

	BOOST_REQUIRE
	(
		T.ContainsPredicate_InRange
		(
			[KEY_TO_SEARCH](const IntRBTree::KeyValueType& KV)->bool { return KV.Key == KEY_TO_SEARCH; },
			RangeFirstConstIt, RangeLastConstIt
		)
	);
	BOOST_REQUIRE
	(
		pCT->ContainsPredicate_InRange
		(
			[KEY_TO_SEARCH](const IntRBTree::KeyValueType& KV)->bool { return KV.Key == KEY_TO_SEARCH; },
			RangeFirstConstIt, RangeLastConstIt
		)
	);
	BOOST_REQUIRE
	(
		pCT->ContainsNotPredicate_InRange
		(
			[KEY_FIRST_IN_RANGE](const IntRBTree::KeyValueType& KV)->bool { return KV.Key == KEY_FIRST_IN_RANGE; },
			RangeFirstConstIt, RangeLastConstIt
		)
	);
}

BOOST_AUTO_TEST_CASE(FindIterator)
{
	IntRBTree T;
	BOOST_REQUIRE(T.Add(2, NoValue{}));
	BOOST_REQUIRE(T.Add(1, NoValue{}));
	BOOST_REQUIRE(T.Add(3, NoValue{}));

	BOOST_REQUIRE( ! T.FindIteratorFor(4) );
	BOOST_REQUIRE( T.FindIteratorFor(1) );
	BOOST_REQUIRE_EQUAL(T.FindIteratorFor(1).GetKey(), 1 );
	BOOST_REQUIRE(T.FindIteratorFor(2));
	BOOST_REQUIRE_EQUAL(T.FindIteratorFor(2).GetKey(), 2);
	BOOST_REQUIRE(T.FindIteratorFor(3));
	BOOST_REQUIRE_EQUAL(T.FindIteratorFor(3).GetKey(), 3);
}

BOOST_AUTO_TEST_CASE(FindIteratorInConst)
{
	IntRBTree T;
	BOOST_REQUIRE(T.Add(2, NoValue{}));
	BOOST_REQUIRE(T.Add(1, NoValue{}));
	BOOST_REQUIRE(T.Add(3, NoValue{}));

	const IntRBTree* const pConstT = &T;
	BOOST_REQUIRE( ! pConstT->FindIteratorFor(4) );
	BOOST_REQUIRE(pConstT->FindIteratorFor(1));
	BOOST_REQUIRE_EQUAL(pConstT->FindIteratorFor(1).GetKey(), 1);
	BOOST_REQUIRE(pConstT->FindIteratorFor(2));
	BOOST_REQUIRE_EQUAL(pConstT->FindIteratorFor(2).GetKey(), 2);
	BOOST_REQUIRE(pConstT->FindIteratorFor(3));
	BOOST_REQUIRE_EQUAL(pConstT->FindIteratorFor(3).GetKey(), 3);
}

BOOST_AUTO_TEST_SUITE_END() // ExtraFindSuite

	BOOST_AUTO_TEST_SUITE(Minimal)

		BOOST_AUTO_TEST_SUITE
		(
			FindSuite,
			*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/AddSuite/AddComplex")
		)

		BOOST_AUTO_TEST_CASE(Corner_FindRoot_InRootOnlyTree)
	{
		constexpr int NUM = 1;
		constexpr int KEY_ONE = 1;

		constexpr int KEY_TO_FIND = KEY_ONE;

		BOOST_TEST_CHECKPOINT("Construction");
		IntRBTree T;
		BOOST_REQUIRE(T.Add(KEY_ONE, NoValue{}));

		BOOST_TEST_CHECKPOINT("Find");
		const IntRBTree::KeyValueType* const pFound = T.Find(KEY_TO_FIND);
		BOOST_REQUIRE(pFound);
		BOOST_REQUIRE_EQUAL(KEY_TO_FIND, pFound->Key);
	}

	BOOST_AUTO_TEST_CASE(Corner_FindRoot)
	{
		constexpr int NUM = 2;
		constexpr int KEY_ONE = 1;
		constexpr int KEY_TWO = 2;

		constexpr int KEY_TO_FIND = KEY_ONE;

		BOOST_TEST_CHECKPOINT("Construction");
		IntRBTree T;
		BOOST_REQUIRE(T.Add(KEY_ONE, NoValue{}));
		BOOST_REQUIRE(T.Add(KEY_TWO, NoValue{}));

		BOOST_TEST_CHECKPOINT("Find");
		const IntRBTree::KeyValueType* const pFound = T.Find(KEY_TO_FIND);
		BOOST_REQUIRE(pFound);
		BOOST_REQUIRE_EQUAL(KEY_TO_FIND, pFound->Key);
	}

	BOOST_AUTO_TEST_CASE(Corner_FindRoot_Inexistent)
	{
		constexpr int NUM = 2;
		constexpr int KEY_ONE = 1;
		constexpr int KEY_TWO = 2;

		constexpr int KEY_TO_FIND = 5;

		BOOST_TEST_CHECKPOINT("Construction");
		IntRBTree T;
		BOOST_REQUIRE(T.Add(KEY_ONE, NoValue{}));
		BOOST_REQUIRE(T.Add(KEY_TWO, NoValue{}));

		BOOST_TEST_CHECKPOINT("Find");
		const IntRBTree::KeyValueType* const pFound = T.Find(KEY_TO_FIND);
		BOOST_REQUIRE(!pFound);
	}

	BOOST_AUTO_TEST_CASE(Corner_FindRoot_InRootOnlyTree_Inexistent)
	{
		constexpr int NUM = 1;
		constexpr int KEY_ONE = 1;

		constexpr int KEY_TO_FIND = 5;

		BOOST_TEST_CHECKPOINT("Construction");
		IntRBTree T;
		BOOST_REQUIRE(T.Add(KEY_ONE, NoValue{}));

		BOOST_TEST_CHECKPOINT("Find");
		const IntRBTree::KeyValueType* const pFound = T.Find(KEY_TO_FIND);
		BOOST_REQUIRE(!pFound);
	}

	BOOST_AUTO_TEST_CASE(FindDeepest)
	{
		constexpr int KEY_TO_FIND = 3;

		constexpr int NUM = 9;

		constexpr int KEY_ONE = 1;
		constexpr int KEY_TWO = 2;
		constexpr int KEY_THREE = 3;
		constexpr int KEY_FOUR = 4;
		constexpr int KEY_FIVE = 5;
		constexpr int KEY_SIX = 6;
		constexpr int KEY_SEVEN = 7;
		constexpr int KEY_EIGHT = 8;
		constexpr int KEY_NINE = 9;

		BOOST_TEST_CHECKPOINT("Construction");
		IntRBTree T;
		BOOST_REQUIRE(T.Add(KEY_SIX, NoValue{}));
		BOOST_REQUIRE(T.Add(KEY_EIGHT, NoValue{}));
		BOOST_REQUIRE(T.Add(KEY_FOUR, NoValue{}));
		BOOST_REQUIRE(T.Add(KEY_SEVEN, NoValue{}));
		BOOST_REQUIRE(T.Add(KEY_NINE, NoValue{}));
		BOOST_REQUIRE(T.Add(KEY_TWO, NoValue{}));
		BOOST_REQUIRE(T.Add(KEY_THREE, NoValue{}));
		BOOST_REQUIRE(T.Add(KEY_FIVE, NoValue{}));
		BOOST_REQUIRE(T.Add(KEY_ONE, NoValue{}));

		BOOST_TEST_CHECKPOINT("Find");
		const IntRBTree::KeyValueType* pFound = T.Find(KEY_TO_FIND);
		BOOST_REQUIRE(pFound);
		BOOST_REQUIRE_EQUAL(KEY_TO_FIND, pFound->Key);
	}

	BOOST_AUTO_TEST_CASE(FindDeepestOne)
	{
		constexpr int KEY_TO_FIND = 7;

		constexpr int NUM = 9;

		constexpr int KEY_ONE = 1;
		constexpr int KEY_TWO = 2;
		constexpr int KEY_THREE = 3;
		constexpr int KEY_FOUR = 4;
		constexpr int KEY_FIVE = 5;
		constexpr int KEY_SIX = 6;
		constexpr int KEY_SEVEN = 7;
		constexpr int KEY_EIGHT = 8;
		constexpr int KEY_NINE = 9;

		BOOST_TEST_CHECKPOINT("Construction");
		IntRBTree T;
		BOOST_REQUIRE(T.Add(KEY_SIX, NoValue{}));
		BOOST_REQUIRE(T.Add(KEY_EIGHT, NoValue{}));
		BOOST_REQUIRE(T.Add(KEY_FOUR, NoValue{}));
		BOOST_REQUIRE(T.Add(KEY_SEVEN, NoValue{}));
		BOOST_REQUIRE(T.Add(KEY_NINE, NoValue{}));
		BOOST_REQUIRE(T.Add(KEY_TWO, NoValue{}));
		BOOST_REQUIRE(T.Add(KEY_THREE, NoValue{}));
		BOOST_REQUIRE(T.Add(KEY_FIVE, NoValue{}));
		BOOST_REQUIRE(T.Add(KEY_ONE, NoValue{}));

		BOOST_TEST_CHECKPOINT("Find");
		const IntRBTree::KeyValueType* pFound = T.Find(KEY_TO_FIND);
		BOOST_REQUIRE(pFound);
		BOOST_REQUIRE_EQUAL(KEY_TO_FIND, pFound->Key);
	}

	BOOST_AUTO_TEST_CASE(FindDeepestInexistent)
	{
		constexpr int KEY_TO_FIND = 10;

		constexpr int NUM = 9;

		constexpr int KEY_ONE = 1;
		constexpr int KEY_TWO = 2;
		constexpr int KEY_THREE = 3;
		constexpr int KEY_FOUR = 4;
		constexpr int KEY_FIVE = 5;
		constexpr int KEY_SIX = 6;
		constexpr int KEY_SEVEN = 7;
		constexpr int KEY_EIGHT = 8;
		constexpr int KEY_NINE = 9;

		BOOST_TEST_CHECKPOINT("Construction");
		IntRBTree T;
		BOOST_REQUIRE(T.Add(KEY_SIX, NoValue{}));
		BOOST_REQUIRE(T.Add(KEY_EIGHT, NoValue{}));
		BOOST_REQUIRE(T.Add(KEY_FOUR, NoValue{}));
		BOOST_REQUIRE(T.Add(KEY_SEVEN, NoValue{}));
		BOOST_REQUIRE(T.Add(KEY_NINE, NoValue{}));
		BOOST_REQUIRE(T.Add(KEY_TWO, NoValue{}));
		BOOST_REQUIRE(T.Add(KEY_THREE, NoValue{}));
		BOOST_REQUIRE(T.Add(KEY_FIVE, NoValue{}));
		BOOST_REQUIRE(T.Add(KEY_ONE, NoValue{}));

		BOOST_TEST_CHECKPOINT("Find");
		const IntRBTree::KeyValueType* pFound = T.Find(KEY_TO_FIND);
		BOOST_REQUIRE(!pFound);
	}

	BOOST_AUTO_TEST_SUITE_END() // FindSuite
	BOOST_AUTO_TEST_SUITE_END() // Minimal	
	BOOST_AUTO_TEST_SUITE_END() // TRBTreeTestSuite
	BOOST_AUTO_TEST_SUITE_END() // Container
	BOOST_AUTO_TEST_SUITE_END() // Core
} // anonymous