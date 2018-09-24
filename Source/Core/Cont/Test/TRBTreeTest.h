#pragma once

#include "TRBTree/TRBTreeTestSet.h"
#include "TRBTree/TRBTreeTestExtraOps.h"
#include "TRBTree/TRBTreeTestMemorySuite.h"
#include "TRBTree/TRBTreeTestSerialization.h"
#include "TRBTree/TRBTreeTestMinimalMain.h"
#include "TRBTree/TRBTreeTestMinimalAddSuite.h"
#include "TRBTree/TRBTreeTestMinimalRemoveSuite.h"
#include "TRBTree/TRBTreeTestMinimalIterationSuite.h"
#include "TRBTree/TRBTreeTestFindSuite.h"

namespace
{

BOOST_AUTO_TEST_SUITE(Core)
BOOST_AUTO_TEST_SUITE(Container)
BOOST_AUTO_TEST_SUITE(TRBTreeTestSuite)

BOOST_AUTO_TEST_SUITE
(
	MovingTestSuite,
	*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal")
)

BOOST_AUTO_TEST_CASE(AppendMovedKeyValueTest)
{
	BOOST_TEST_CHECKPOINT("Initialization");
	const int KEY_CONST = 3;
	MoveOnlyTree T;
	BOOST_REQUIRE(T.Add(MoveOnlyTree::KeyValueType{ 1, std::make_unique<std::string>("VALUE") }));
	BOOST_REQUIRE(T.Add(2, std::make_unique<std::string>("VALUE")));
	BOOST_REQUIRE(T.Add(KEY_CONST, std::make_unique<std::string>("VALUE")));
	BOOST_REQUIRE_EQUAL(3, T.Num());
}

BOOST_AUTO_TEST_CASE(AddKeyValueTest_NoMovable)
{
	BOOST_TEST_CHECKPOINT("Initialization");
	const int KEY_CONST = 3;
	const NoValue VALUE_CONST;

	IntRBTree T;
	IntRBTree::KeyValueType const KV{ 1, NoValue{} };
	BOOST_REQUIRE(T.Add(KV));
	BOOST_REQUIRE(T.Add(2, VALUE_CONST));
	BOOST_REQUIRE(T.Add(KEY_CONST, VALUE_CONST));
	BOOST_REQUIRE_EQUAL(3, T.Num());
}

BOOST_AUTO_TEST_SUITE_END() // MovingTestSuite

BOOST_AUTO_TEST_SUITE_END() // ? Container - WRONG!!!


BOOST_AUTO_TEST_SUITE
(
	CopyingSuite,
	*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/AddSuite")
	*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/RemoveSuite")
)



BOOST_AUTO_TEST_CASE(TemplateCopyConstruct)
{
	const IntStringRBTree::KeyValueType KV_1{ 1, std::string("one") };
	const IntStringRBTree::KeyValueType KV_2{ 2, std::string("two") };
	const IntStringRBTree::KeyValueType KV_3{ 3, std::string("three") };

	IntStringRBTree Source;
	Source.Add(KV_1);
	Source.Add(KV_2);
	Source.Add(KV_3);

	TRBTree<KVType<int, std::string>, TemplateCopyTestIntComparer> T{ Source };
	BOOST_REQUIRE_EQUAL(T.Num(), 3);
	BOOST_REQUIRE(T.Contains(KV_1.Key));
	BOOST_REQUIRE(T.Contains(KV_2.Key));
	BOOST_REQUIRE(T.Contains(KV_3.Key));
}

BOOST_AUTO_TEST_CASE(TemplateCopyAssign)
{
	const IntStringRBTree::KeyValueType KV_1{ 1, std::string("one") };
	const IntStringRBTree::KeyValueType KV_2{ 2, std::string("two") };
	const IntStringRBTree::KeyValueType KV_3{ 3, std::string("three") };

	IntStringRBTree Source;
	BOOST_REQUIRE(Source.Add(KV_1));
	BOOST_REQUIRE(Source.Add(KV_2));
	BOOST_REQUIRE(Source.Add(KV_3));

	TRBTree<KVType<int, std::string>, TemplateCopyTestIntComparer> T;
	const IntStringRBTree::KeyValueType OTHER_KV_1{ 11, std::string("eleven") };
	const IntStringRBTree::KeyValueType OTHER_KV_2{ 12, std::string("twelve") };
	BOOST_REQUIRE(T.Add(OTHER_KV_1));
	BOOST_REQUIRE(T.Add(OTHER_KV_2));


	T = Source;
	BOOST_REQUIRE_EQUAL(T.Num(), 3);
	BOOST_REQUIRE(T.Contains(KV_1.Key));
	BOOST_REQUIRE(T.Contains(KV_2.Key));
	BOOST_REQUIRE(T.Contains(KV_3.Key));
}

BOOST_AUTO_TEST_CASE(TemplateMoveConstruct)
{
	const IntStringRBTree::KeyValueType KV_1{ 1, std::string("one") };
	const IntStringRBTree::KeyValueType KV_2{ 2, std::string("two") };
	const IntStringRBTree::KeyValueType KV_3{ 3, std::string("three") };

	IntStringRBTree Source;
	Source.Add(KV_1);
	Source.Add(KV_2);
	Source.Add(KV_3);

	TRBTree<KVType<int, std::string>, TemplateCopyTestIntComparer> T{ std::move(Source) };
	BOOST_REQUIRE_EQUAL(T.Num(), 3);
	BOOST_REQUIRE(T.Contains(KV_1.Key));
	BOOST_REQUIRE(T.Contains(KV_2.Key));
	BOOST_REQUIRE(T.Contains(KV_3.Key));

	BOOST_REQUIRE(Source.Empty());
}

BOOST_AUTO_TEST_CASE(TemplateMoveAssignment)
{
	const IntStringRBTree::KeyValueType KV_1{ 1, std::string("one") };
	const IntStringRBTree::KeyValueType KV_2{ 2, std::string("two") };
	const IntStringRBTree::KeyValueType KV_3{ 3, std::string("three") };

	IntStringRBTree Source;
	BOOST_REQUIRE(Source.Add(KV_1));
	BOOST_REQUIRE(Source.Add(KV_2));
	BOOST_REQUIRE(Source.Add(KV_3));

	TRBTree<KVType<int, std::string>, TemplateCopyTestIntComparer> T;
	const IntStringRBTree::KeyValueType OTHER_KV_1{ 11, std::string("eleven") };
	const IntStringRBTree::KeyValueType OTHER_KV_2{ 12, std::string("twelve") };
	BOOST_REQUIRE(T.Add(OTHER_KV_1));
	BOOST_REQUIRE(T.Add(OTHER_KV_2));

	T = std::move(Source);
	BOOST_REQUIRE_EQUAL(T.Num(), 3);
	BOOST_REQUIRE(T.Contains(KV_1.Key));
	BOOST_REQUIRE(T.Contains(KV_2.Key));
	BOOST_REQUIRE(T.Contains(KV_3.Key));
	BOOST_REQUIRE(!T.Contains(OTHER_KV_1.Key));
	BOOST_REQUIRE(!T.Contains(OTHER_KV_2.Key));

	BOOST_REQUIRE(Source.Empty());
}

BOOST_AUTO_TEST_CASE(MoveConstruct)
{
	const IntStringRBTree::KeyValueType KV_1{ 1, std::string("one") };
	const IntStringRBTree::KeyValueType KV_2{ 2, std::string("two") };
	const IntStringRBTree::KeyValueType KV_3{ 3, std::string("three") };

	IntStringRBTree Source;
	Source.Add(KV_1);
	Source.Add(KV_2);
	Source.Add(KV_3);

	IntStringRBTree T{ std::move(Source) };
	BOOST_REQUIRE_EQUAL(T.Num(), 3);
	BOOST_REQUIRE(T.Contains(KV_1.Key));
	BOOST_REQUIRE(T.Contains(KV_2.Key));
	BOOST_REQUIRE(T.Contains(KV_3.Key));

	BOOST_REQUIRE(Source.Empty());
}

BOOST_AUTO_TEST_CASE(MoveAssignment)
{
	const IntStringRBTree::KeyValueType KV_1{ 1, std::string("one") };
	const IntStringRBTree::KeyValueType KV_2{ 2, std::string("two") };
	const IntStringRBTree::KeyValueType KV_3{ 3, std::string("three") };

	IntStringRBTree Source;
	BOOST_REQUIRE(Source.Add(KV_1));
	BOOST_REQUIRE(Source.Add(KV_2));
	BOOST_REQUIRE(Source.Add(KV_3));

	IntStringRBTree T;
	const IntStringRBTree::KeyValueType OTHER_KV_1{ 11, std::string("eleven") };
	const IntStringRBTree::KeyValueType OTHER_KV_2{ 12, std::string("twelve") };
	BOOST_REQUIRE(T.Add(OTHER_KV_1));
	BOOST_REQUIRE(T.Add(OTHER_KV_2));

	T = std::move(Source);
	BOOST_REQUIRE_EQUAL(T.Num(), 3);
	BOOST_REQUIRE(T.Contains(KV_1.Key));
	BOOST_REQUIRE(T.Contains(KV_2.Key));
	BOOST_REQUIRE(T.Contains(KV_3.Key));
	BOOST_REQUIRE( ! T.Contains(OTHER_KV_1.Key) );
	BOOST_REQUIRE( ! T.Contains(OTHER_KV_2.Key) );

	BOOST_REQUIRE(Source.Empty());
}

BOOST_AUTO_TEST_CASE(SimpleCopyConstruct)
{
	const IntStringRBTree::KeyValueType KV_1 { 1, std::string("one") };
	const IntStringRBTree::KeyValueType KV_2 { 2, std::string("two") };
	const IntStringRBTree::KeyValueType KV_3 { 3, std::string("three") };

	IntStringRBTree Source;
	Source.Add(KV_1);
	Source.Add(KV_2);
	Source.Add(KV_3);
	
	IntStringRBTree T { Source };
	BOOST_REQUIRE_EQUAL(T.Num(), 3);
	BOOST_REQUIRE(T.Contains(KV_1.Key));
	BOOST_REQUIRE(T.Contains(KV_2.Key));
	BOOST_REQUIRE(T.Contains(KV_3.Key));
}

BOOST_AUTO_TEST_CASE(SimpleCopyAssign)
{
	const IntStringRBTree::KeyValueType KV_1{ 1, std::string("one") };
	const IntStringRBTree::KeyValueType KV_2{ 2, std::string("two") };
	const IntStringRBTree::KeyValueType KV_3{ 3, std::string("three") };

	IntStringRBTree Source;
	BOOST_REQUIRE(Source.Add(KV_1));
	BOOST_REQUIRE(Source.Add(KV_2));
	BOOST_REQUIRE(Source.Add(KV_3));

	IntStringRBTree T;
	const IntStringRBTree::KeyValueType OTHER_KV_1{ 11, std::string("eleven") };
	const IntStringRBTree::KeyValueType OTHER_KV_2{ 12, std::string("twelve") };
	BOOST_REQUIRE(T.Add(OTHER_KV_1));
	BOOST_REQUIRE(T.Add(OTHER_KV_2));


	T = Source;
	BOOST_REQUIRE_EQUAL(T.Num(), 3);
	BOOST_REQUIRE(T.Contains(KV_1.Key));
	BOOST_REQUIRE(T.Contains(KV_2.Key));
	BOOST_REQUIRE(T.Contains(KV_3.Key));
	BOOST_REQUIRE( ! T.Contains(OTHER_KV_1.Key) );
	BOOST_REQUIRE( ! T.Contains(OTHER_KV_2.Key) );
}
BOOST_AUTO_TEST_SUITE_END()

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

/**
* Tests a minimal set of operations, to make it possible to write other tests dependent on it.
* - Construction test;
* - Num, Empty tests;
* - Contains minimal test;
* - Add minimal test;
*/
BOOST_AUTO_TEST_SUITE
(
	IterationSuite,
	*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/FirstMinimal")
	*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/MinMax")
)

BOOST_AUTO_TEST_CASE(KeyIteration)
{
	IntStringRBTree T;
	const IntStringRBTree* pCT = &T;
	T.Add(1, std::string("one"));
	T.Add(2, std::string("two"));
	T.Add(3, std::string("three"));

	IntStringRBTree::IteratorType It = T.Iterator();
	IntStringRBTree::KeyIteratorType KeyIt = T.KeyIterator();
	BOOST_REQUIRE(KeyIt.GetBaseIterator() == T.Iterator());
	while (It != T.EndIterator())
	{
		BOOST_REQUIRE_EQUAL(It.GetKey(), *KeyIt);
		BOOST_REQUIRE_EQUAL(It.GetKey(), KeyIt.Get());

		BOOST_REQUIRE(!KeyIt.IsEnd());
		BOOST_REQUIRE(!!KeyIt);
		BOOST_REQUIRE(KeyIt);
		BOOST_REQUIRE(KeyIt.GetPtr() != nullptr);
		BOOST_REQUIRE_EQUAL(It.GetKey(), *KeyIt.GetPtr());

		++KeyIt;
		++It;
	}
	BOOST_REQUIRE(KeyIt.IsEnd());

	BOOST_TEST_CHECKPOINT("Comparison");
	BOOST_REQUIRE(T.KeyIterator() == T.KeyIterator());
	BOOST_REQUIRE(T.KeyIterator() == pCT->KeyIterator());
	BOOST_REQUIRE(T.EndKeyIterator() == T.EndKeyIterator());
	BOOST_REQUIRE(T.EndKeyIterator() == pCT->EndKeyIterator());
	BOOST_REQUIRE(T.EndKeyIterator() != T.KeyIterator());
	BOOST_REQUIRE(pCT->EndKeyIterator() != T.KeyIterator());

	BOOST_TEST_CHECKPOINT("Assignment");
	IntStringRBTree::KeyIteratorType AssignedIt = T.EndKeyIterator();
	AssignedIt = T.KeyIterator();
	BOOST_REQUIRE(AssignedIt);
	BOOST_REQUIRE(AssignedIt == T.KeyIterator());
	AssignedIt = pCT->KeyIterator();
	BOOST_REQUIRE(AssignedIt);
	BOOST_REQUIRE(AssignedIt == T.KeyIterator());
	AssignedIt = pCT->EndKeyIterator();
	BOOST_REQUIRE(AssignedIt == T.EndKeyIterator());
	AssignedIt = T.EndKeyIterator();
	BOOST_REQUIRE(AssignedIt == T.EndKeyIterator());
}

BOOST_AUTO_TEST_CASE(IteratorElementType)
{
	static_assert
		(
			std::is_same
			<
			const TRBTree<KVType<int, std::string>>::IteratorType::ElementType,
			const TKeyValue<KVType<int, std::string>>
			>::value
			);
	static_assert
		(
			std::is_same
			<
			const TRBTree<KVType<int, std::string>>::IteratorType::ElementType,
			const TKeyValue<KVType<int, std::string>>
			>::value
			);
	static_assert
		(
			std::is_same
			<
			const TRBTree<KVType<int, std::string>>::ConstIteratorType::ElementType,
			const TKeyValue<KVType<int, std::string>>
			>::value
			);
	static_assert
		(
			std::is_same
			<
			const TRBTree<KVType<int, std::string>>::ConstIteratorType::ElementType,
			const TKeyValue<KVType<int, std::string>>
			>::value
			);
}


BOOST_AUTO_TEST_CASE(IterationEmpty)
{
	IntRBTree T;
	BOOST_REQUIRE(T.Iterator().IsEnd());
	int numRangeIterated = 0;
	for (const IntRBTree::KeyValueType& KV : T)
	{
		numRangeIterated++;
	}
	BOOST_REQUIRE_EQUAL(0, numRangeIterated);
}
BOOST_AUTO_TEST_CASE(IterationRootOnly)
{
	// WARNING!!! Key values matter!
	constexpr int KEY_ONE = 1;

	BOOST_TEST_CHECKPOINT("Construction");
	IntRBTree T;
	BOOST_REQUIRE(T.Add(KEY_ONE, NoValue{}));

	BOOST_TEST_CHECKPOINT("Iteration");
	IntRBTree::IteratorType It = T.Iterator();
	BOOST_REQUIRE(!It.IsEnd());
	BOOST_REQUIRE(It.GetKeyValue() == IntRBTree::KeyValueType(KEY_ONE, NoValue{}));
	BOOST_REQUIRE(It.GetKey() == IntRBTree::KeyType(KEY_ONE));
	BOOST_REQUIRE(It.GetValue() == IntRBTree::ValueType());
	auto OldIt = It;
	auto OldIt2 = It++;
	// We must test the postfix increment
	BOOST_REQUIRE(OldIt == OldIt2);
	BOOST_REQUIRE(It.IsEnd());

	BOOST_TEST_CHECKPOINT("RangeIteration");
	std::vector<IntRBTree::KeyValueType> RangeIterated;
	for (const IntRBTree::KeyValueType& KV : T)
	{
		RangeIterated.push_back(KV);
	}
	BOOST_REQUIRE_EQUAL(1, RangeIterated.size());
	BOOST_REQUIRE_EQUAL(RangeIterated[0].Key, KEY_ONE);
}

void ConstIteratorTest_ConstContext(const StringToIntRBTree& T)
{
	// WARNING!!! Never check keys or value (it's string - we do not know order of its iteration intuitively)
	StringToIntRBTree::ConstIteratorType cit = T.ConstIterator();
	BOOST_REQUIRE(!cit.IsEnd());
	++cit;
	BOOST_REQUIRE(!cit.IsEnd());
	++cit;
	BOOST_REQUIRE(!cit.IsEnd());
	++cit;
	BOOST_REQUIRE(cit.IsEnd());

	// Checking range loop:
	std::vector<StringToIntRBTree::KeyValueType> IteratedPairs;
	for (const StringToIntRBTree::KeyValueType& KV : T)
	{
		IteratedPairs.push_back(KV);
	}
	BOOST_REQUIRE_EQUAL(IteratedPairs.size(), T.Num());

	// Should not be compilable:
	//cit.SetValue(3);
}

void ConstIteratorTest_Copy(const StringToIntRBTree& TC, StringToIntRBTree& T)
{
	BOOST_REQUIRE(TC.Iterator() == T.ConstIterator());
	StringToIntRBTree::ConstIteratorType cit = TC.Iterator();
	StringToIntRBTree::IteratorType it = T.Iterator();

	BOOST_TEST_CHECKPOINT("Iterator move-construct test");
	StringToIntRBTree::IteratorType it_moved = std::move(T.Iterator());
	BOOST_REQUIRE(it_moved == T.Iterator());

	BOOST_TEST_CHECKPOINT("Iterator move-assignment test");
	it_moved = std::move(T.Iterator());
	BOOST_REQUIRE(it_moved == T.Iterator());

	BOOST_TEST_CHECKPOINT("Iterator copy-construct test");
	StringToIntRBTree::ConstIteratorType new_cit_copied_from_it = it;
	BOOST_REQUIRE(it == new_cit_copied_from_it);

	BOOST_TEST_CHECKPOINT("Iterator copy test");
	++new_cit_copied_from_it;
	new_cit_copied_from_it = it;
	BOOST_REQUIRE(it == new_cit_copied_from_it);
}

void ConstIteratorTest_Equality(const StringToIntRBTree& TC, StringToIntRBTree& T)
{
	StringToIntRBTree::ConstIteratorType cit = TC.Iterator();
	StringToIntRBTree::IteratorType it = T.Iterator();

	BOOST_REQUIRE_EQUAL(cit.GetKeyValue(), it.GetKeyValue());
	BOOST_REQUIRE_EQUAL(cit.GetKey(), it.GetKey());
	BOOST_REQUIRE_EQUAL(cit.GetValue(), it.GetValue());

	BOOST_TEST_CHECKPOINT("Checking const/non-const iterator equality");
	BOOST_REQUIRE(cit == it);
	BOOST_REQUIRE(it == cit);

	BOOST_TEST_CHECKPOINT("Checking const/non-const iterator non-equality");
	++it;
	BOOST_REQUIRE(cit != it);
	BOOST_REQUIRE(it != cit);
}

BOOST_AUTO_TEST_CASE(ConstIteratorTest, *boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/IterationSuite/IterationRootOnly"))
{
	StringToIntRBTree T;

	BOOST_REQUIRE(T.Add(STR_KEY_ONE, 1));
	BOOST_REQUIRE(T.Add(STR_KEY_TWO, 2));
	BOOST_REQUIRE(T.Add(STR_KEY_THREE, 3));

	ConstIteratorTest_ConstContext(T);
	ConstIteratorTest_Equality(T, T);
	ConstIteratorTest_Copy(T, T);
}

BOOST_AUTO_TEST_CASE(IterationEqual)
{
	// WARNING!!! Key values matter!
	constexpr int KEY_ONE = 1;
	constexpr int KEY_TWO = 2;

	BOOST_TEST_CHECKPOINT("Construction");
	IntRBTree T;
	// WARNING!!! Addition order matters!!
	BOOST_REQUIRE(T.Add(KEY_TWO, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_ONE, NoValue{}));

	BOOST_TEST_CHECKPOINT("Iteration");
	IntRBTree::IteratorType It = T.Iterator();
	IntRBTree::IteratorType It_next = T.Iterator();
	BOOST_REQUIRE(It == It_next);

	BOOST_REQUIRE(!It.IsEnd());
	BOOST_REQUIRE(It_next == It_next);
	BOOST_REQUIRE(It == It);
	It_next++;
	BOOST_REQUIRE(!(It == It_next));

	BOOST_REQUIRE(It.GetKeyValue() == IntRBTree::KeyValueType(KEY_ONE, NoValue{}));
	It++;
	BOOST_REQUIRE(It_next == It);
	It_next++;

	BOOST_REQUIRE(It.GetKeyValue() == IntRBTree::KeyValueType(KEY_TWO, NoValue{}));
	It++;
	BOOST_REQUIRE(It.IsEnd());
	BOOST_REQUIRE(It_next == It);
}
BOOST_AUTO_TEST_CASE(IterNonConst, *boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/IterationSuite/IterationEqual"))
{
	StringToIntRBTree T;

	const std::string KEY_ONE = std::string("one");
	const std::string KEY_TWO = std::string("two");
	const std::string KEY_THREE = std::string("three");

	const int VALUE_NEG_ONE = -1;

	BOOST_REQUIRE(T.Add(KEY_ONE, 1));
	BOOST_REQUIRE(T.Add(KEY_TWO, 2));
	BOOST_REQUIRE(T.Add(KEY_THREE, 3));

	const std::string KEY_TO_SEARCH = KEY_TWO;
	const int VALUE_TO_SEARCH = 2;
	auto It = T.Iterator();
	while (It.GetKey() != KEY_TO_SEARCH)
	{
		++It;
	}
	BOOST_REQUIRE(It.GetKey() == KEY_TO_SEARCH);
	BOOST_REQUIRE(It.GetValue() == VALUE_TO_SEARCH);

	BOOST_TEST_CHECKPOINT("Modifying where iterator points to");
	It.SetValue(VALUE_NEG_ONE);
	BOOST_REQUIRE_EQUAL(It.GetValue(), VALUE_NEG_ONE);
}
BOOST_AUTO_TEST_CASE(IterationLeftToParent)
{
	// WARNING!!! Key values matter!
	constexpr int KEY_ONE = 1;
	constexpr int KEY_TWO = 2;

	BOOST_TEST_CHECKPOINT("Construction");
	IntRBTree T;
	// WARNING!!! Addition order matters!!
	BOOST_REQUIRE(T.Add(KEY_TWO, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_ONE, NoValue{}));

	BOOST_TEST_CHECKPOINT("Iteration");
	IntRBTree::IteratorType It = T.Iterator();
	BOOST_REQUIRE(!It.IsEnd());
	BOOST_REQUIRE(It.GetKeyValue() == IntRBTree::KeyValueType(KEY_ONE, NoValue{}));
	It++;
	BOOST_REQUIRE(It.GetKeyValue() == IntRBTree::KeyValueType(KEY_TWO, NoValue{}));
	It++;
	BOOST_REQUIRE(It.IsEnd());

	BOOST_TEST_CHECKPOINT("RangeIteration");
	std::vector<IntRBTree::KeyValueType> RangeIterated;
	for (const IntRBTree::KeyValueType& KV : T)
	{
		RangeIterated.push_back(KV);
	}
	BOOST_REQUIRE_EQUAL(2, RangeIterated.size());
	BOOST_REQUIRE_EQUAL(RangeIterated[0].Key, KEY_ONE);
	BOOST_REQUIRE_EQUAL(RangeIterated[1].Key, KEY_TWO);
}
BOOST_AUTO_TEST_CASE
(
	IterationLeftToParent_ThreeLevels,
	*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/IterationSuite/IterationLeftToParent")
)
{
	// WARNING!!! Key values matter!
	constexpr int KEY_ONE = 1;
	constexpr int KEY_TWO = 2;
	constexpr int KEY_THREE = 3;

	BOOST_TEST_CHECKPOINT("Construction");
	IntRBTree T;
	// WARNING!!! Addition order matters!!
	BOOST_REQUIRE(T.Add(KEY_THREE, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_TWO, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_ONE, NoValue{}));

	BOOST_TEST_CHECKPOINT("Iteration");
	IntRBTree::IteratorType It = T.Iterator();
	BOOST_REQUIRE(!It.IsEnd());
	BOOST_REQUIRE(It.GetKeyValue() == IntRBTree::KeyValueType(KEY_ONE, NoValue{}));
	It++;
	BOOST_REQUIRE(It.GetKeyValue() == IntRBTree::KeyValueType(KEY_TWO, NoValue{}));
	It++;
	BOOST_REQUIRE(It.GetKeyValue() == IntRBTree::KeyValueType(KEY_THREE, NoValue{}));
	It++;
	BOOST_REQUIRE(It.IsEnd());

	BOOST_TEST_CHECKPOINT("RangeIteration");
	std::vector<IntRBTree::KeyValueType> RangeIterated;
	for (const IntRBTree::KeyValueType& KV : T)
	{
		RangeIterated.push_back(KV);
	}
	BOOST_REQUIRE_EQUAL(3, RangeIterated.size());
	BOOST_REQUIRE_EQUAL(RangeIterated[0].Key, KEY_ONE);
	BOOST_REQUIRE_EQUAL(RangeIterated[1].Key, KEY_TWO);
	BOOST_REQUIRE_EQUAL(RangeIterated[2].Key, KEY_THREE);
}
BOOST_AUTO_TEST_CASE
(
	IterationLeftToParent_ManyLevels,
	*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/IterationSuite/IterationLeftToParent_ThreeLevels")
)
{
	// WARNING!!! Key values matter!
	constexpr int KEY_ONE = 1;
	constexpr int KEY_TWO = 2;
	constexpr int KEY_THREE = 3;
	constexpr int KEY_FOUR = 4;

	BOOST_TEST_CHECKPOINT("Construction");
	IntRBTree T;
	// WARNING!!! Addition order matters!!
	BOOST_REQUIRE(T.Add(KEY_FOUR, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_THREE, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_TWO, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_ONE, NoValue{}));

	BOOST_TEST_CHECKPOINT("Iteration");
	IntRBTree::IteratorType It = T.Iterator();
	BOOST_REQUIRE(!It.IsEnd());
	BOOST_REQUIRE(It.GetKeyValue() == IntRBTree::KeyValueType(KEY_ONE, NoValue{}));
	It++;
	BOOST_REQUIRE(It.GetKeyValue() == IntRBTree::KeyValueType(KEY_TWO, NoValue{}));
	It++;
	BOOST_REQUIRE(It.GetKeyValue() == IntRBTree::KeyValueType(KEY_THREE, NoValue{}));
	It++;
	BOOST_REQUIRE(It.GetKeyValue() == IntRBTree::KeyValueType(KEY_FOUR, NoValue{}));
	It++;
	BOOST_REQUIRE(It.IsEnd());
}
BOOST_AUTO_TEST_CASE
(
	IterationRightToParent_ManyLevels,
	*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/IterationSuite/IterationRightToParent_ThreeLevels")
)
{
	// WARNING!!! Key values matter!
	constexpr int KEY_ONE = 1;
	constexpr int KEY_TWO = 2;
	constexpr int KEY_THREE = 3;
	constexpr int KEY_FOUR = 4;

	BOOST_TEST_CHECKPOINT("Construction");
	IntRBTree T;
	// WARNING!!! Addition order matters!!
	BOOST_REQUIRE(T.Add(KEY_ONE, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_TWO, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_THREE, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_FOUR, NoValue{}));

	BOOST_TEST_CHECKPOINT("Iteration");
	IntRBTree::IteratorType It = T.Iterator();
	BOOST_REQUIRE(!It.IsEnd());
	BOOST_REQUIRE(It.GetKeyValue() == IntRBTree::KeyValueType(KEY_ONE, NoValue{}));
	It++;
	BOOST_REQUIRE(It.GetKeyValue() == IntRBTree::KeyValueType(KEY_TWO, NoValue{}));
	It++;
	BOOST_REQUIRE(It.GetKeyValue() == IntRBTree::KeyValueType(KEY_THREE, NoValue{}));
	It++;
	BOOST_REQUIRE(It.GetKeyValue() == IntRBTree::KeyValueType(KEY_FOUR, NoValue{}));
	It++;
	BOOST_REQUIRE(It.IsEnd());
}
BOOST_AUTO_TEST_CASE
(
	IterationRightToParent_ThreeLevels,
	*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/IterationSuite/IterationRightToParent")
)
{
	// WARNING!!! Key values matter!
	constexpr int KEY_ONE = 1;
	constexpr int KEY_TWO = 2;
	constexpr int KEY_THREE = 3;

	BOOST_TEST_CHECKPOINT("Construction");
	IntRBTree T;
	// WARNING!!! Addition order matters!!
	BOOST_REQUIRE(T.Add(KEY_ONE, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_TWO, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_THREE, NoValue{}));

	BOOST_TEST_CHECKPOINT("Iteration");
	IntRBTree::IteratorType It = T.Iterator();
	BOOST_REQUIRE(!It.IsEnd());
	BOOST_REQUIRE(It.GetKeyValue() == IntRBTree::KeyValueType(KEY_ONE, NoValue{}));
	It++;
	BOOST_REQUIRE(It.GetKeyValue() == IntRBTree::KeyValueType(KEY_TWO, NoValue{}));
	It++;
	BOOST_REQUIRE(It.GetKeyValue() == IntRBTree::KeyValueType(KEY_THREE, NoValue{}));
	It++;
	BOOST_REQUIRE(It.IsEnd());
}
BOOST_AUTO_TEST_CASE(IterationRightToParent)
{
	// WARNING!!! Key values matter!
	constexpr int KEY_ONE = 1;
	constexpr int KEY_TWO = 2;

	BOOST_TEST_CHECKPOINT("Construction");
	IntRBTree T;
	// WARNING!!! Addition order matters!!
	BOOST_REQUIRE(T.Add(KEY_ONE, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_TWO, NoValue{}));

	BOOST_TEST_CHECKPOINT("Iteration");
	IntRBTree::IteratorType It = T.Iterator();
	BOOST_REQUIRE(!It.IsEnd());
	BOOST_REQUIRE(It.GetKeyValue() == IntRBTree::KeyValueType(KEY_ONE, NoValue{}));
	It++;
	BOOST_REQUIRE(It.GetKeyValue() == IntRBTree::KeyValueType(KEY_TWO, NoValue{}));
	It++;
	BOOST_REQUIRE(It.IsEnd());
}

BOOST_AUTO_TEST_CASE(IterationBothLeftAndRight)
{
	// WARNING!!! Key values matter!!
	constexpr int KEY_ONE = 1;
	constexpr int KEY_TWO = 2;
	constexpr int KEY_THREE = 3;

	BOOST_TEST_CHECKPOINT("Construction");
	IntRBTree T;
	// WARNING!!! Addition order matters!!
	BOOST_REQUIRE(T.Add(KEY_TWO, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_ONE, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_THREE, NoValue{}));

	BOOST_TEST_CHECKPOINT("Iteration");
	IntRBTree::IteratorType It = T.Iterator();
	BOOST_REQUIRE(!It.IsEnd());
	BOOST_REQUIRE(It.GetKeyValue() == IntRBTree::KeyValueType(KEY_ONE, NoValue{}));
	It++;
	BOOST_REQUIRE(It.GetKeyValue() == IntRBTree::KeyValueType(KEY_TWO, NoValue{}));
	It++;
	BOOST_REQUIRE(It.GetKeyValue() == IntRBTree::KeyValueType(KEY_THREE, NoValue{}));
	It++;
	BOOST_REQUIRE(It.IsEnd());
}

BOOST_AUTO_TEST_CASE(IterationLevelTwo)
{
	// WARNING!!! Key values matter!!
	constexpr int KEY_ONE = 1;
	constexpr int KEY_TWO = 2;
	constexpr int KEY_THREE = 3;
	constexpr int KEY_FOUR = 4;
	constexpr int KEY_FIVE = 5;
	constexpr int KEY_SIX = 6;
	constexpr int KEY_SEVEN = 7;

	BOOST_TEST_CHECKPOINT("Construction");
	IntRBTree T;
	// WARNING!!! Addition order matters!!
	BOOST_REQUIRE(T.Add(KEY_FOUR, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_TWO, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_SIX, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_ONE, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_THREE, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_FIVE, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_SEVEN, NoValue{}));

	BOOST_TEST_CHECKPOINT("Iteration");
	IntRBTree::IteratorType It = T.Iterator();
	BOOST_REQUIRE(!It.IsEnd());
	BOOST_REQUIRE(It.GetKeyValue() == IntRBTree::KeyValueType(KEY_ONE, NoValue{}));
	It++;
	BOOST_REQUIRE(It.GetKeyValue() == IntRBTree::KeyValueType(KEY_TWO, NoValue{}));
	It++;
	BOOST_REQUIRE(It.GetKeyValue() == IntRBTree::KeyValueType(KEY_THREE, NoValue{}));
	It++;
	BOOST_REQUIRE(It.GetKeyValue() == IntRBTree::KeyValueType(KEY_FOUR, NoValue{}));
	It++;
	BOOST_REQUIRE(It.GetKeyValue() == IntRBTree::KeyValueType(KEY_FIVE, NoValue{}));
	It++;
	BOOST_REQUIRE(It.GetKeyValue() == IntRBTree::KeyValueType(KEY_SIX, NoValue{}));
	It++;
	BOOST_REQUIRE(It.GetKeyValue() == IntRBTree::KeyValueType(KEY_SEVEN, NoValue{}));
	It++;
	BOOST_REQUIRE(It.IsEnd());
}

BOOST_AUTO_TEST_CASE
(
	IterationComplexCase,
	*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/IterationSuite/IterationEmpty")
	*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/IterationSuite/IterationRootOnly")
	*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/IterationSuite/Iteration")
	*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/IterationSuite/IterationRightToParent_ManyLevels")
	*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/IterationSuite/IterationLeftToParent_ManyLevels")
)
{
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
	BOOST_REQUIRE(T.Add(KEY_SEVEN, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_NINE, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_TWO, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_ONE, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_FIVE, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_THREE, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_FOUR, NoValue{}));

	BOOST_TEST_CHECKPOINT("Iteration");
	IntRBTree::IteratorType It = T.Iterator();
	BOOST_REQUIRE(!It.IsEnd());
	BOOST_REQUIRE(It.GetKeyValue() == IntRBTree::KeyValueType(KEY_ONE, NoValue{}));
	It++;
	BOOST_REQUIRE(It.GetKeyValue() == IntRBTree::KeyValueType(KEY_TWO, NoValue{}));
	It++;
	BOOST_REQUIRE(It.GetKeyValue() == IntRBTree::KeyValueType(KEY_THREE, NoValue{}));
	It++;
	BOOST_REQUIRE(It.GetKeyValue() == IntRBTree::KeyValueType(KEY_FOUR, NoValue{}));
	It++;
	BOOST_REQUIRE(It.GetKeyValue() == IntRBTree::KeyValueType(KEY_FIVE, NoValue{}));
	It++;
	BOOST_REQUIRE(It.GetKeyValue() == IntRBTree::KeyValueType(KEY_SIX, NoValue{}));
	It++;
	BOOST_REQUIRE(It.GetKeyValue() == IntRBTree::KeyValueType(KEY_SEVEN, NoValue{}));
	It++;
	BOOST_REQUIRE(It.GetKeyValue() == IntRBTree::KeyValueType(KEY_EIGHT, NoValue{}));
	It++;
	BOOST_REQUIRE(It.GetKeyValue() == IntRBTree::KeyValueType(KEY_NINE, NoValue{}));
	It++;
	BOOST_REQUIRE(It.IsEnd());
}


BOOST_AUTO_TEST_CASE(Iteration)
{
	constexpr int NUM = 5;

	constexpr int KEY_TWO = 2;
	constexpr int KEY_THREE = 3;
	constexpr int KEY_FIVE = 5;
	constexpr int KEY_SEVEN = 7;
	constexpr int KEY_EIGHT = 8;

	const IntRBTree::KeyValueType REFERENCE_SEQUENCE[] =
	{
		IntRBTree::KeyValueType(KEY_TWO, NoValue{}),
		IntRBTree::KeyValueType(KEY_THREE, NoValue{}),
		IntRBTree::KeyValueType(KEY_FIVE, NoValue{}),
		IntRBTree::KeyValueType(KEY_SEVEN, NoValue{}),
		IntRBTree::KeyValueType(KEY_EIGHT, NoValue{})
	};

	BOOST_TEST_CHECKPOINT("Construction");
	IntRBTree T;
	BOOST_REQUIRE(T.Add(KEY_TWO, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_THREE, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_FIVE, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_SEVEN, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_EIGHT, NoValue{}));

	BOOST_TEST_CHECKPOINT("Iteration");
	IntRBTree::IteratorType It = T.Iterator();
	for (int Index = 0; Index < T.Num(); Index++)
	{
		BOOST_REQUIRE(!It.IsEnd());
		BOOST_REQUIRE_EQUAL(It.GetKeyValue(), REFERENCE_SEQUENCE[Index]);
		It++;
	}
	BOOST_REQUIRE(It.IsEnd());
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_CASE(CopyToTest, *boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/FirstMinimal"))
{
	constexpr int NUM = 5;

	constexpr int KEY_TWO = 2;
	constexpr int KEY_THREE = 3;
	constexpr int KEY_FIVE = 5;
	constexpr int KEY_SEVEN = 7;
	constexpr int KEY_EIGHT = 8;

	const IntRBTree::KeyValueType REFERENCE_SEQUENCE[] =
	{
		IntRBTree::KeyValueType(KEY_TWO, NoValue{}),
		IntRBTree::KeyValueType(KEY_THREE, NoValue{}),
		IntRBTree::KeyValueType(KEY_FIVE, NoValue{}),
		IntRBTree::KeyValueType(KEY_SEVEN, NoValue{}),
		IntRBTree::KeyValueType(KEY_EIGHT, NoValue{})
	};

	BOOST_TEST_CHECKPOINT("Construction");
	IntRBTree T;
	BOOST_REQUIRE(T.Add(KEY_TWO, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_THREE, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_FIVE, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_SEVEN, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_EIGHT, NoValue{}));

	BOOST_TEST_CHECKPOINT("Copying to buffer");
	IntRBTree::KeyValueType DestBuf[NUM];
	T.CopyUnorderedTo(DestBuf);

	BOOST_REQUIRE(ArrayEquals(DestBuf, REFERENCE_SEQUENCE, NUM));
}

BOOST_AUTO_TEST_CASE(CopyUnorderedToTest, *boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/FirstMinimal"))
{
	constexpr int NUM = 5;

	constexpr int KEY_FIVE = 5;
	constexpr int KEY_TWO = 2;
	constexpr int KEY_THREE = 3;
	constexpr int KEY_SEVEN = 7;
	constexpr int KEY_EIGHT = 8;

	BOOST_TEST_CHECKPOINT("Construction");
	IntRBTree T;
	BOOST_REQUIRE(T.Add(KEY_FIVE, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_TWO, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_THREE, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_SEVEN, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_EIGHT, NoValue{}));

	BOOST_TEST_CHECKPOINT("Copying to buffer");
	IntRBTree::KeyValueType DestBuf[NUM];
	T.CopyUnorderedTo(DestBuf);

	BOOST_REQUIRE(ArrayContainsValue(DestBuf, NUM, KEY_FIVE));
	BOOST_REQUIRE(ArrayContainsValue(DestBuf, NUM, KEY_TWO));
	BOOST_REQUIRE(ArrayContainsValue(DestBuf, NUM, KEY_THREE));
	BOOST_REQUIRE(ArrayContainsValue(DestBuf, NUM, KEY_SEVEN));
	BOOST_REQUIRE(ArrayContainsValue(DestBuf, NUM, KEY_EIGHT));

}

BOOST_AUTO_TEST_CASE(ForEachEmptyTest, *boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/FirstMinimal"))
{
	BOOST_TEST_CHECKPOINT("Construction");
	IntRBTree T;

	BOOST_TEST_CHECKPOINT("ForEach");
	std::vector<IntRBTree::KeyValueType> DestBuf;
	T.ForEach
	(
		[&DestBuf](const IntRBTree::KeyValueType& KV)
	{
		DestBuf.push_back(KV);
	}
	);
	BOOST_REQUIRE(DestBuf.empty());
}

BOOST_AUTO_TEST_CASE(ForEachTest, *boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/FirstMinimal"))
{
	constexpr int NUM = 5;

	constexpr int KEY_FIVE = 5;
	constexpr int KEY_TWO = 2;
	constexpr int KEY_THREE = 3;
	constexpr int KEY_SEVEN = 7;
	constexpr int KEY_EIGHT = 8;

	BOOST_TEST_CHECKPOINT("Construction");
	IntRBTree T;
	BOOST_REQUIRE(T.Add(KEY_FIVE, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_TWO, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_THREE, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_SEVEN, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_EIGHT, NoValue{}));

	BOOST_TEST_CHECKPOINT("ForEach");
	std::vector<IntRBTree::KeyValueType> DestBuf;
	T.ForEach
	(
		[&DestBuf](const IntRBTree::KeyValueType& KV)
	{
		DestBuf.push_back(KV);
	}
	);

	BOOST_REQUIRE(ArrayContainsValue(DestBuf.data(), NUM, KEY_FIVE));
	BOOST_REQUIRE(ArrayContainsValue(DestBuf.data(), NUM, KEY_TWO));
	BOOST_REQUIRE(ArrayContainsValue(DestBuf.data(), NUM, KEY_THREE));
	BOOST_REQUIRE(ArrayContainsValue(DestBuf.data(), NUM, KEY_SEVEN));
	BOOST_REQUIRE(ArrayContainsValue(DestBuf.data(), NUM, KEY_EIGHT));
}

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

BOOST_AUTO_TEST_SUITE(AddSuite)
BOOST_AUTO_TEST_CASE
(
	AddLess,
	*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/FirstMinimal")
	*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/CopyToTest")
)
{
	constexpr int NUM = 2;

	constexpr int KEY_ONE = 1;
	constexpr int KEY_TWO = 2;

	const IntRBTree::KeyValueType REFERENCE_SEQUENCE[] =
	{
		IntRBTree::KeyValueType(KEY_ONE, NoValue{}),
		IntRBTree::KeyValueType(KEY_TWO, NoValue{})
	};

	BOOST_TEST_CHECKPOINT("Construction");
	IntRBTree T;
	BOOST_REQUIRE(T.Add(KEY_TWO, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_ONE, NoValue{}));

	BOOST_TEST_CHECKPOINT("Copying to buffer");
	IntRBTree::KeyValueType DestBuf[NUM];
	T.CopyTo(DestBuf);

	BOOST_REQUIRE(ArrayEquals(DestBuf, REFERENCE_SEQUENCE, NUM));
}

BOOST_AUTO_TEST_CASE(AddLessLess, *boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/AddSuite/AddLess"))
{
	constexpr int NUM = 3;

	constexpr int KEY_ONE = 1;
	constexpr int KEY_TWO = 2;
	constexpr int KEY_THREE = 3;

	const IntRBTree::KeyValueType REFERENCE_SEQUENCE[] =
	{
		IntRBTree::KeyValueType(KEY_ONE, NoValue{}),
		IntRBTree::KeyValueType(KEY_TWO, NoValue{}),
		IntRBTree::KeyValueType(KEY_THREE, NoValue{})
	};

	BOOST_TEST_CHECKPOINT("Construction");
	IntRBTree T;
	BOOST_REQUIRE(T.Add(KEY_THREE, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_TWO, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_ONE, NoValue{}));

	BOOST_TEST_CHECKPOINT("Copying to buffer");
	IntRBTree::KeyValueType DestBuf[NUM];
	T.CopyTo(DestBuf);

	BOOST_REQUIRE(ArrayEquals(DestBuf, REFERENCE_SEQUENCE, NUM));
}

BOOST_AUTO_TEST_CASE(AddLessLessLess, *boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/AddSuite/AddLessLess"))
{
	constexpr int NUM = 4;

	constexpr int KEY_ONE = 1;
	constexpr int KEY_TWO = 2;
	constexpr int KEY_THREE = 3;
	constexpr int KEY_FOUR = 4;

	const IntRBTree::KeyValueType REFERENCE_SEQUENCE[] =
	{
		IntRBTree::KeyValueType(KEY_ONE, NoValue{}),
		IntRBTree::KeyValueType(KEY_TWO, NoValue{}),
		IntRBTree::KeyValueType(KEY_THREE, NoValue{}),
		IntRBTree::KeyValueType(KEY_FOUR, NoValue{})
	};

	BOOST_TEST_CHECKPOINT("Construction");
	IntRBTree T;
	BOOST_REQUIRE(T.Add(KEY_FOUR, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_THREE, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_TWO, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_ONE, NoValue{}));

	BOOST_TEST_CHECKPOINT("Copying to buffer");
	IntRBTree::KeyValueType DestBuf[NUM];
	T.CopyTo(DestBuf);

	BOOST_REQUIRE(ArrayEquals(DestBuf, REFERENCE_SEQUENCE, NUM));
}

BOOST_AUTO_TEST_CASE(AddLessManyMany, *boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/AddSuite/AddLessLessLess"))
{
	IntRBTree T;

	constexpr int COUNT = 40;
	for (int i = COUNT - 1; i >= 0; i--)
	{
		BOOST_REQUIRE(T.Add(i, NoValue{}));
	}

	for (int i = 0; i < COUNT; i++)
	{
		BOOST_REQUIRE(T.Contains(i));
	}
}

BOOST_AUTO_TEST_CASE(AddMiddle, *boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/AddSuite/AddLess"))
{
	constexpr int NUM = 3;

	constexpr int KEY_ONE = 1;
	constexpr int KEY_TWO = 2;
	constexpr int KEY_THREE = 3;

	const IntRBTree::KeyValueType REFERENCE_SEQUENCE[] =
	{
		IntRBTree::KeyValueType(KEY_ONE, NoValue{}),
		IntRBTree::KeyValueType(KEY_TWO, NoValue{}),
		IntRBTree::KeyValueType(KEY_THREE, NoValue{})
	};

	BOOST_TEST_CHECKPOINT("Construction");
	IntRBTree T;
	BOOST_REQUIRE(T.Add(KEY_THREE, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_ONE, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_TWO, NoValue{}));

	BOOST_TEST_CHECKPOINT("Copying to buffer");
	IntRBTree::KeyValueType DestBuf[NUM];
	T.CopyTo(DestBuf);

	BOOST_REQUIRE(ArrayEquals(DestBuf, REFERENCE_SEQUENCE, NUM));
}

BOOST_AUTO_TEST_CASE
(
	AddGreater,
	*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/FirstMinimal")
	*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/CopyToTest")
)
{
	constexpr int NUM = 2;

	constexpr int KEY_ONE = 1;
	constexpr int KEY_TWO = 2;

	const IntRBTree::KeyValueType REFERENCE_SEQUENCE[] =
	{
		IntRBTree::KeyValueType(KEY_ONE, NoValue{}),
		IntRBTree::KeyValueType(KEY_TWO, NoValue{})
	};

	BOOST_TEST_CHECKPOINT("Construction");
	IntRBTree T;
	BOOST_REQUIRE(T.Add(KEY_ONE, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_TWO, NoValue{}));

	BOOST_TEST_CHECKPOINT("Copying to buffer");
	IntRBTree::KeyValueType DestBuf[NUM];
	T.CopyTo(DestBuf);

	BOOST_REQUIRE(ArrayEquals(DestBuf, REFERENCE_SEQUENCE, NUM));
}

BOOST_AUTO_TEST_CASE(AddGreaterGreater, *boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/AddSuite/AddGreater"))
{
	constexpr int NUM = 3;

	constexpr int KEY_ONE = 1;
	constexpr int KEY_TWO = 2;
	constexpr int KEY_THREE = 3;

	const IntRBTree::KeyValueType REFERENCE_SEQUENCE[] =
	{
		IntRBTree::KeyValueType(KEY_ONE, NoValue{}),
		IntRBTree::KeyValueType(KEY_TWO, NoValue{}),
		IntRBTree::KeyValueType(KEY_THREE, NoValue{})
	};

	BOOST_TEST_CHECKPOINT("Construction");
	IntRBTree T;
	BOOST_REQUIRE(T.Add(KEY_ONE, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_TWO, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_THREE, NoValue{}));

	BOOST_TEST_CHECKPOINT("Copying to buffer");
	IntRBTree::KeyValueType DestBuf[NUM];
	T.CopyTo(DestBuf);

	BOOST_REQUIRE_EQUAL(DestBuf[0].Key, KEY_ONE);
	BOOST_REQUIRE_EQUAL(DestBuf[1].Key, KEY_TWO);
	BOOST_REQUIRE_EQUAL(DestBuf[2].Key, KEY_THREE);
}

BOOST_AUTO_TEST_CASE(AddGreaterGreaterGreater, *boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/AddSuite/AddGreaterGreater"))
{
	constexpr int NUM = 4;

	constexpr int KEY_ONE = 1;
	constexpr int KEY_TWO = 2;
	constexpr int KEY_THREE = 3;
	constexpr int KEY_FOUR = 4;

	const IntRBTree::KeyValueType REFERENCE_SEQUENCE[] =
	{
		IntRBTree::KeyValueType(KEY_ONE, NoValue{}),
		IntRBTree::KeyValueType(KEY_TWO, NoValue{}),
		IntRBTree::KeyValueType(KEY_THREE, NoValue{}),
		IntRBTree::KeyValueType(KEY_FOUR, NoValue{})
	};

	BOOST_TEST_CHECKPOINT("Construction");
	IntRBTree T;
	BOOST_REQUIRE(T.Add(KEY_ONE, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_TWO, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_THREE, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_FOUR, NoValue{}));

	BOOST_TEST_CHECKPOINT("Copying to buffer");
	IntRBTree::KeyValueType DestBuf[NUM];
	T.CopyTo(DestBuf);

	BOOST_REQUIRE_EQUAL(DestBuf[0].Key, KEY_ONE);
	BOOST_REQUIRE_EQUAL(DestBuf[1].Key, KEY_TWO);
	BOOST_REQUIRE_EQUAL(DestBuf[2].Key, KEY_THREE);
	BOOST_REQUIRE_EQUAL(DestBuf[3].Key, KEY_FOUR);
}

BOOST_AUTO_TEST_CASE(AddGreaterManyMany, *boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/AddSuite/AddGreaterGreaterGreater"))
{
	IntRBTree T;

	constexpr int COUNT = 40;
	for (int i = 0; i < COUNT; i++)
	{
		BOOST_REQUIRE(T.Add(i, NoValue{}));
	}

	for (int i = 0; i < COUNT; i++)
	{
		BOOST_REQUIRE(T.Contains(i));
	}

	IntRBTree::KeyValueType DestBuf[COUNT];
	T.CopyTo(DestBuf);

	for (int i = 0; i < COUNT; i++)
	{
		BOOST_REQUIRE_EQUAL(DestBuf[i].Key, i);
	}
}


BOOST_AUTO_TEST_CASE(AddGreaterThenMiddle, *boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/AddSuite/AddGreater"))
{
	constexpr int NUM = 3;

	constexpr int KEY_ONE = 1;
	constexpr int KEY_TWO = 2;
	constexpr int KEY_THREE = 3;

	const IntRBTree::KeyValueType REFERENCE_SEQUENCE[] =
	{
		IntRBTree::KeyValueType(KEY_ONE, NoValue{}),
		IntRBTree::KeyValueType(KEY_TWO, NoValue{}),
		IntRBTree::KeyValueType(KEY_THREE, NoValue{})
	};

	BOOST_TEST_CHECKPOINT("Construction");
	IntRBTree T;
	BOOST_REQUIRE(T.Add(KEY_ONE, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_THREE, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_TWO, NoValue{}));

	BOOST_TEST_CHECKPOINT("Copying to buffer");
	IntRBTree::KeyValueType DestBuf[NUM];
	T.CopyTo(DestBuf);
}

BOOST_AUTO_TEST_CASE
(
	AddComplex,
	*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/AddSuite/AddLessLessLess")
	*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/AddSuite/AddGreaterGreaterGreater")
	*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/AddSuite/AddMiddle")
	*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/AddSuite/AddGreaterThenMiddle")
)
{
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

	const IntRBTree::KeyValueType REFERENCE_SEQUENCE[] =
	{
		IntRBTree::KeyValueType(KEY_ONE, NoValue{}),
		IntRBTree::KeyValueType(KEY_TWO, NoValue{}),
		IntRBTree::KeyValueType(KEY_THREE, NoValue{}),
		IntRBTree::KeyValueType(KEY_FOUR, NoValue{}),
		IntRBTree::KeyValueType(KEY_FIVE, NoValue{}),
		IntRBTree::KeyValueType(KEY_SIX, NoValue{}),
		IntRBTree::KeyValueType(KEY_SEVEN, NoValue{}),
		IntRBTree::KeyValueType(KEY_EIGHT, NoValue{}),
		IntRBTree::KeyValueType(KEY_NINE, NoValue{})
	};

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

	BOOST_TEST_CHECKPOINT("Copying to buffer");
	IntRBTree::KeyValueType DestBuf[NUM];
	T.CopyTo(DestBuf);
}

BOOST_AUTO_TEST_SUITE_END() // AddSuite

BOOST_AUTO_TEST_SUITE
(
	FindSuite,
	*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/AddSuite/AddComplex")
)

BOOST_AUTO_TEST_SUITE_END() // Minimal
BOOST_AUTO_TEST_SUITE_END() // TRBTree
BOOST_AUTO_TEST_SUITE_END() // Container
BOOST_AUTO_TEST_SUITE_END() // Core

} // anonymous
