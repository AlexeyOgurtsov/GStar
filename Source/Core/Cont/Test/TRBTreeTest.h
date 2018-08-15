#pragma once

#include <boost/test/included/unit_test.hpp>
#include "Core/Cont/TRBTree.h"
#include <algorithm>
#include <string>

namespace
{

using IntRBTree = TRBTree<KVType<int, NoValue>>;
using StringToIntRBTree = TRBTree<KVType<std::string, int>>;


const std::string STR_KEY_ONE = std::string("one");
const std::string STR_KEY_TWO = std::string("two");
const std::string STR_KEY_THREE = std::string("three");

const int VALUE_NEG_ONE = -1;

bool ArrayEquals
(
	const TKeyValue<KVType<int, NoValue>>* A, 
	const TKeyValue<KVType<int, NoValue>>* B, 
	int InSize
)
{
	return std::equal(A, A + InSize, B, B+InSize);
}

bool ArrayContainsValue(const TKeyValue<KVType<int, NoValue>>* Ptr, int InSize, const int& InKey)
{
	const TKeyValue<KVType<int, NoValue>>* END_IT = Ptr + InSize;

	BOOST_ASSERT(Ptr);
	BOOST_ASSERT(InSize >=0);
	BOOST_ASSERT(InSize >= 0);
	auto it = std::find_if
	(
		Ptr, END_IT, 
		[&InKey](const TKeyValue<KVType<int, NoValue>>& InCurrKV)
		{ 
			return InCurrKV.Key == InKey;
		}
	);
	return it != END_IT;
}


BOOST_AUTO_TEST_SUITE(Core)
BOOST_AUTO_TEST_SUITE(Container)
BOOST_AUTO_TEST_SUITE(TRBTreeTestSuite)

BOOST_AUTO_TEST_SUITE
(
	CustomCompare,
	*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal")
)

struct IntWithIntPointerComparer
{
	int Compare(int A, const int* pB) const
	{
		return DefaultCompare(A, *pB);
	}
};

struct TestByValueComparer
{
	int Compare(const int* pA, const int* pB) const
	{
		return DefaultCompare(*pA, *pB);
	}
};

BOOST_AUTO_TEST_CASE
(
	CompareArgTest,
	*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/CustomCompare/CompareIntWithPointerToInt")
)
{

	using CustomIntPtrRBTree = TRBTree<KVType<const int*, NoValue>, TestByValueComparer>;

	const int KEY_ONE = 1;
	const int* const PTR_TO_KEY_ONE = &KEY_ONE;

	BOOST_TEST_CHECKPOINT("Construction");
	CustomIntPtrRBTree T;

	BOOST_TEST_CHECKPOINT("Add");
	BOOST_REQUIRE(T.Add(PTR_TO_KEY_ONE, NoValue{}));

	BOOST_TEST_CHECKPOINT("Find");
	const CustomIntPtrRBTree::KeyValueType* const pFound = T.Find(PTR_TO_KEY_ONE);

	BOOST_REQUIRE(pFound);
	BOOST_REQUIRE_EQUAL(pFound->Key, PTR_TO_KEY_ONE);
}

BOOST_AUTO_TEST_CASE(CompareIntWithPointerToInt)
{
	const int KEY_ONE = 1;
	const int* const PTR_TO_KEY_ONE = &KEY_ONE;

	BOOST_TEST_CHECKPOINT("Construction");
	IntRBTree T;

	BOOST_TEST_CHECKPOINT("Add");
	BOOST_REQUIRE(T.Add(KEY_ONE, NoValue{}));

	BOOST_TEST_CHECKPOINT("Find");
	const IntRBTree::KeyValueType* const pFound = T.Find(PTR_TO_KEY_ONE, IntWithIntPointerComparer());
	BOOST_REQUIRE(pFound);
	BOOST_REQUIRE_EQUAL(pFound->Key, KEY_ONE);
}

BOOST_AUTO_TEST_SUITE_END() // CustomCompare

BOOST_AUTO_TEST_SUITE(Minimal)

/**
* Tests a minimal set of operations, to make it possible to write other tests dependent on it.
* - Construction test;
* - Num, Empty tests;
* - Contains minimal test;
* - Add minimal test;
*/
BOOST_AUTO_TEST_CASE(FirstMinimal)
{
	/**
	* WARNING!!! This keys are ordered (see test).
	*/
	const int KEY_ONE = 1;
	const int KEY_TWO = 2;
	const int KEY_ZERO = 0;

	BOOST_TEST_CHECKPOINT("Construction");
	IntRBTree T;
	BOOST_REQUIRE_EQUAL(0, T.Num());
	BOOST_REQUIRE(T.Empty());

	BOOST_TEST_CHECKPOINT("Find on empty");
	BOOST_REQUIRE( ! T.Contains(KEY_ONE) );
	BOOST_REQUIRE( ! T.Find(KEY_ONE) );
	BOOST_REQUIRE( ! T.FindValue(KEY_ONE) );

	BOOST_TEST_CHECKPOINT("Add");
	BOOST_REQUIRE(T.Add(KEY_ONE, NoValue{}));
	BOOST_REQUIRE_EQUAL(1, T.Num());
	BOOST_REQUIRE( ! T.Empty() );
	BOOST_REQUIRE( T.Contains(KEY_ONE) );
	BOOST_REQUIRE( ! T.Contains(KEY_TWO) );
	BOOST_REQUIRE( ! T.Contains(KEY_ZERO) );
	BOOST_REQUIRE( T.Find(KEY_ONE) );
	BOOST_REQUIRE_EQUAL( KEY_ONE, T.Find(KEY_ONE)->Key );

	BOOST_TEST_CHECKPOINT("Add Already Included");
	BOOST_REQUIRE( ! T.Add(KEY_ONE, NoValue{}) );
	BOOST_REQUIRE_EQUAL( 1, T.Num() );

	BOOST_TEST_CHECKPOINT("Add another (on the right of the root)");
	BOOST_REQUIRE( T.Add(KEY_TWO, NoValue{}) );
	BOOST_REQUIRE( T.Contains(KEY_TWO) );
	BOOST_REQUIRE( T.Contains(KEY_ONE) );
	BOOST_REQUIRE_EQUAL( KEY_TWO, T.Find(KEY_TWO)->Key );
	BOOST_REQUIRE_EQUAL( KEY_ONE, T.Find(KEY_ONE)->Key );
	BOOST_REQUIRE_EQUAL( 2, T.Num() );

	BOOST_TEST_CHECKPOINT("Add another (on the left of the root)");
	BOOST_REQUIRE(T.Add(KEY_ZERO, NoValue{}));
	BOOST_REQUIRE(T.Contains(KEY_ZERO));
	BOOST_REQUIRE(T.Contains(KEY_TWO));
	BOOST_REQUIRE(T.Contains(KEY_ONE));
	BOOST_REQUIRE_EQUAL(KEY_TWO, T.Find(KEY_TWO)->Key);
	BOOST_REQUIRE_EQUAL(KEY_ONE, T.Find(KEY_ONE)->Key);
	BOOST_REQUIRE_EQUAL(KEY_ZERO, T.Find(KEY_ZERO)->Key);
	BOOST_REQUIRE_EQUAL(3, T.Num());
}

BOOST_AUTO_TEST_CASE
(
	MinMax,
	*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/FirstMinimal")
)
{
	/**
	* WARNING!!! This keys are ordered (see test).
	*/
	const int KEY_ONE = 1;
	const int KEY_TWO = 2;
	const int KEY_THREE = 3;
	const int KEY_FOUR = 4;
	const int KEY_FIVE = 5;
	const int KEY_SIX = 6;
	const int KEY_SEVEN = 7;

	BOOST_TEST_CHECKPOINT("Construction");
	IntRBTree T;
	
	BOOST_TEST_CHECKPOINT("Add");
	BOOST_REQUIRE(T.Add(KEY_FIVE, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_THREE, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_SEVEN, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_SIX, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_TWO, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_ONE, NoValue{}));
	BOOST_REQUIRE(T.Add(KEY_FOUR, NoValue{}));
	BOOST_REQUIRE_EQUAL(T.Num(), KEY_SEVEN);

	BOOST_TEST_CHECKPOINT("MinMax");
	BOOST_REQUIRE_EQUAL(KEY_ONE, T.Min().Key);
	BOOST_REQUIRE_EQUAL(KEY_SEVEN, T.Max().Key);
}

BOOST_AUTO_TEST_SUITE
(
	IterationSuite, 
	*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/FirstMinimal")
	*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/MinMax")
)
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
	BOOST_REQUIRE( ! It.IsEnd() );
	BOOST_REQUIRE(It.GetKeyValue() == IntRBTree::KeyValueType( KEY_ONE, NoValue{} ));
	BOOST_REQUIRE(It.GetKey() == IntRBTree::KeyType(KEY_ONE));
	BOOST_REQUIRE(It.GetValue() == IntRBTree::ValueType());
	auto OldIt = It;
	auto OldIt2 = It++;
	// We must test the postfix increment
	BOOST_REQUIRE(OldIt == OldIt2);
	BOOST_REQUIRE( It.IsEnd() );

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
	BOOST_REQUIRE( ! cit.IsEnd() );
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
	while(It.GetKey() != KEY_TO_SEARCH)
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
		BOOST_REQUIRE( ! It.IsEnd() );
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

BOOST_AUTO_TEST_CASE(TraverseEmptyTest, *boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/FirstMinimal"))
{
	BOOST_TEST_CHECKPOINT("Construction");
	IntRBTree T;

	BOOST_TEST_CHECKPOINT("Traverse");
	std::vector<IntRBTree::KeyValueType> DestBuf;
	T.Traverse
	(
		[&DestBuf](const IntRBTree::KeyValueType& KV)
		{
			DestBuf.push_back(KV);
		}
	);
	BOOST_REQUIRE(DestBuf.empty());
}

BOOST_AUTO_TEST_CASE(TraverseTest, *boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/FirstMinimal"))
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

	BOOST_TEST_CHECKPOINT("Traverse");
	std::vector<IntRBTree::KeyValueType> DestBuf;
	T.Traverse
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
	BOOST_REQUIRE( ! T.Remove(KEY_ONE) );
	BOOST_REQUIRE( ! T.Remove(KEY_TWO) );
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
	BOOST_REQUIRE( ! T.Remove(KEY_NOT_EXISTING) );
	BOOST_REQUIRE( ! T.Empty() );

	BOOST_TEST_CHECKPOINT("Removing");
	BOOST_REQUIRE( T.Remove(KEY_EXISTING) );
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

BOOST_AUTO_TEST_CASE(RemoveLeftChildOfRoot_ChildHasTwoChildren)
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
	BOOST_REQUIRE( ! pFound );
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
	BOOST_REQUIRE( ! pFound );
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
	BOOST_REQUIRE( ! pFound );
}

BOOST_AUTO_TEST_SUITE_END() // FindSuite

BOOST_AUTO_TEST_SUITE_END() // Minimal
BOOST_AUTO_TEST_SUITE_END() // TRBTree
BOOST_AUTO_TEST_SUITE_END() // Container
BOOST_AUTO_TEST_SUITE_END() // Core

} // anonymous