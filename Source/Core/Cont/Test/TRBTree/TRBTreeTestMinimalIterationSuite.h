#pragma once

#include "TRBTreeTestUtils.h"

namespace
{
	BOOST_AUTO_TEST_SUITE(Core)
	BOOST_AUTO_TEST_SUITE(Container)
	BOOST_AUTO_TEST_SUITE(TRBTreeTestSuite)
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
	BOOST_AUTO_TEST_SUITE_END() // IterationSuite
	BOOST_AUTO_TEST_SUITE_END() // Minimal
	BOOST_AUTO_TEST_SUITE_END() // TRBTreeTestSuite
	BOOST_AUTO_TEST_SUITE_END() // Container
	BOOST_AUTO_TEST_SUITE_END() // Core
} // anonymous