#pragma once

#include <boost/test/included/unit_test.hpp>
#include "Core/Cont/TRBTree.h"
#include <algorithm>

namespace
{

using IntRBTree = TRBTree<KVType<int, NoValue>>;

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
	BOOST_REQUIRE_EQUAL( 1, T.Num() );
	BOOST_REQUIRE( ! T.Empty() );
	BOOST_REQUIRE( T.Add(KEY_ONE, NoValue{}) );
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