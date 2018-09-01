#pragma once


#include "Core/Cont/TRBTree.h"
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/test/included/unit_test.hpp>
#include <algorithm>
#include <string>
#include <memory>  // unique_ptr

namespace
{

using IntRBTree = TRBTree<KVType<int, NoValue>>;
using StringToIntRBTree = TRBTree<KVType<std::string, int>>;
using IntToUniquePtStr = TRBTree<KVType<int, std::unique_ptr<std::string>>>;
using IntStringRBTree = TRBTree<KVType<int, std::string>>;

struct TestNonPOD
{
	bool* pbDestructed = false;
	TestNonPOD(bool* pbInDestructed) : pbDestructed(pbInDestructed) {}
	~TestNonPOD() { *pbDestructed = true; }
};

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

using MoveOnlyTree = TRBTree<KVType<int, std::unique_ptr<std::string>>>;

BOOST_AUTO_TEST_SUITE
(
	MovingTestSuite,
	*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal")
)

BOOST_AUTO_TEST_CASE(AddMovedKeyValueTest)
{
	BOOST_TEST_CHECKPOINT("Initialization");
	const int KEY_CONST = 3;
	MoveOnlyTree T;
	BOOST_REQUIRE(T.AddCheck(MoveOnlyTree::KeyValueType{1, std::make_unique<std::string>("VALUE")}));
	BOOST_REQUIRE(T.AddCheck(2, std::make_unique<std::string>("VALUE")));
	BOOST_REQUIRE(T.AddCheck(KEY_CONST, std::make_unique<std::string>("VALUE")));
	BOOST_REQUIRE_EQUAL(3, T.Num());
}

BOOST_AUTO_TEST_CASE(AddKeyValueTest_NoMovable)
{
	BOOST_TEST_CHECKPOINT("Initialization");
	const int KEY_CONST = 3;
	const NoValue VALUE_CONST;

	IntRBTree T;
	IntRBTree::KeyValueType const KV { 1, NoValue{} };
	BOOST_REQUIRE(T.AddCheck(KV));
	BOOST_REQUIRE(T.AddCheck(2, VALUE_CONST));
	BOOST_REQUIRE(T.AddCheck(KEY_CONST, VALUE_CONST));
	BOOST_REQUIRE_EQUAL(3, T.Num());
}

BOOST_AUTO_TEST_SUITE_END() // MovingTestSuite

BOOST_AUTO_TEST_SUITE
(
	CopyingSuite,
	*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/AddSuite")
	*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/RemoveSuite")
)


struct TemplateCopyTestIntComparer
{
	int Compare(int A, int B) const
	{
		return DefaultCompare(A, B);
	}
};


BOOST_AUTO_TEST_CASE(TemplateCopyConstruct)
{
	const IntStringRBTree::KeyValueType KV_1{ 1, std::string("one") };
	const IntStringRBTree::KeyValueType KV_2{ 2, std::string("two") };
	const IntStringRBTree::KeyValueType KV_3{ 3, std::string("three") };

	IntStringRBTree Source;
	Source.AddCheck(KV_1);
	Source.AddCheck(KV_2);
	Source.AddCheck(KV_3);

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
	BOOST_REQUIRE(Source.AddCheck(KV_1));
	BOOST_REQUIRE(Source.AddCheck(KV_2));
	BOOST_REQUIRE(Source.AddCheck(KV_3));

	TRBTree<KVType<int, std::string>, TemplateCopyTestIntComparer> T;
	const IntStringRBTree::KeyValueType OTHER_KV_1{ 11, std::string("eleven") };
	const IntStringRBTree::KeyValueType OTHER_KV_2{ 12, std::string("twelve") };
	BOOST_REQUIRE(T.AddCheck(OTHER_KV_1));
	BOOST_REQUIRE(T.AddCheck(OTHER_KV_2));


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
	Source.AddCheck(KV_1);
	Source.AddCheck(KV_2);
	Source.AddCheck(KV_3);

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
	BOOST_REQUIRE(Source.AddCheck(KV_1));
	BOOST_REQUIRE(Source.AddCheck(KV_2));
	BOOST_REQUIRE(Source.AddCheck(KV_3));

	TRBTree<KVType<int, std::string>, TemplateCopyTestIntComparer> T;
	const IntStringRBTree::KeyValueType OTHER_KV_1{ 11, std::string("eleven") };
	const IntStringRBTree::KeyValueType OTHER_KV_2{ 12, std::string("twelve") };
	BOOST_REQUIRE(T.AddCheck(OTHER_KV_1));
	BOOST_REQUIRE(T.AddCheck(OTHER_KV_2));

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
	Source.AddCheck(KV_1);
	Source.AddCheck(KV_2);
	Source.AddCheck(KV_3);

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
	BOOST_REQUIRE(Source.AddCheck(KV_1));
	BOOST_REQUIRE(Source.AddCheck(KV_2));
	BOOST_REQUIRE(Source.AddCheck(KV_3));

	IntStringRBTree T;
	const IntStringRBTree::KeyValueType OTHER_KV_1{ 11, std::string("eleven") };
	const IntStringRBTree::KeyValueType OTHER_KV_2{ 12, std::string("twelve") };
	BOOST_REQUIRE(T.AddCheck(OTHER_KV_1));
	BOOST_REQUIRE(T.AddCheck(OTHER_KV_2));

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
	Source.AddCheck(KV_1);
	Source.AddCheck(KV_2);
	Source.AddCheck(KV_3);
	
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
	BOOST_REQUIRE(Source.AddCheck(KV_1));
	BOOST_REQUIRE(Source.AddCheck(KV_2));
	BOOST_REQUIRE(Source.AddCheck(KV_3));

	IntStringRBTree T;
	const IntStringRBTree::KeyValueType OTHER_KV_1{ 11, std::string("eleven") };
	const IntStringRBTree::KeyValueType OTHER_KV_2{ 12, std::string("twelve") };
	BOOST_REQUIRE(T.AddCheck(OTHER_KV_1));
	BOOST_REQUIRE(T.AddCheck(OTHER_KV_2));


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

BOOST_AUTO_TEST_CASE(FindIterator)
{
	IntRBTree T;
	BOOST_REQUIRE(T.AddCheck(2, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(1, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(3, NoValue{}));

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
	BOOST_REQUIRE(T.AddCheck(2, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(1, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(3, NoValue{}));

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
BOOST_AUTO_TEST_CASE(RemoveAtTest)
{
	constexpr int INITIAL_COUNT = 3;

	IntRBTree T;
	BOOST_REQUIRE(T.AddCheck(2, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(1, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(3, NoValue{}));

	T.RemoveAt(T.Iterator());

	BOOST_REQUIRE_EQUAL(T.Num(), (INITIAL_COUNT - 1));
	BOOST_REQUIRE( ! T.Contains(1) );
	BOOST_REQUIRE(T.Contains(2));
	BOOST_REQUIRE(T.Contains(3));
}
BOOST_AUTO_TEST_SUITE_END() // ExtraRemoveSuite

BOOST_AUTO_TEST_SUITE
(
	ExtraOps,
	*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/AddSuite")
	*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/RemoveSuite")
)

BOOST_AUTO_TEST_SUITE(ExtraAddOpsSuite)

struct EqualityTestStringComparer
{
	int Compare(const std::string& A, const std::string& B) const
	{
		return DefaultCompare(A, B);
	}
};

BOOST_AUTO_TEST_CASE(EqualityTest)
{
	StringToIntRBTree A;
	TRBTree<KVType<std::string, int>, EqualityTestStringComparer> B;

	BOOST_TEST_CHECKPOINT("Equality for empty");
	BOOST_REQUIRE(A == B);
	BOOST_REQUIRE(B == A);
	BOOST_REQUIRE(! ( A != B) );

	BOOST_TEST_CHECKPOINT("Inequality by first element (A > B)");
	A.AddCheck(std::string("one"), 1);
	BOOST_REQUIRE(A != B);
	BOOST_REQUIRE(B != A);

	BOOST_TEST_CHECKPOINT("Equality of one-element containers");
	B.AddCheck(std::string("one"), 1);
	BOOST_REQUIRE(A == B);
	BOOST_REQUIRE(B == A);

	BOOST_TEST_CHECKPOINT("Inequality by first element (B > A)");
	B.AddCheck(std::string("two"), 2);
	BOOST_REQUIRE(A != B);

	BOOST_TEST_CHECKPOINT("Equality of two-element containers");
	A.AddCheck(std::string("two"), 2);
	BOOST_REQUIRE(A == B);

	BOOST_TEST_CHECKPOINT("Non equality if key is the same by value differs");
	A.AddCheck(std::string("three"), 3);
	B.AddCheck(std::string("three"), 2);
	BOOST_REQUIRE(A != B);
}

BOOST_AUTO_TEST_CASE(AddRange)
{
	const int COUNT = 6;

	TVector<IntStringRBTree::KeyValueType> Source;
	Source.Add(IntStringRBTree::KeyValueType(2, std::string("one")));
	Source.Add(IntStringRBTree::KeyValueType(1, std::string("one")));
	Source.Add(IntStringRBTree::KeyValueType(3, std::string("one")));
	Source.Add(IntStringRBTree::KeyValueType(4, std::string("one")));
	Source.Add(IntStringRBTree::KeyValueType(5, std::string("five"))); // this element will be OUT of range

	IntStringRBTree T;
	T.AddCheck(0, "zero");

	T.AddRange(Source.Iterator(), Source.IteratorAt(Source.LastIndex()));

	for (int i = 0; i < (COUNT - 1); i++)
	{
		BOOST_REQUIRE(T.Contains(i));
	}
}

BOOST_AUTO_TEST_CASE(AddAllRange)
{
	const int COUNT = 6;

	IntStringRBTree Source;
	Source.AddCheck(5, std::string("five"));
	Source.AddCheck(2, std::string("one"));
	Source.AddCheck(1, std::string("one"));
	Source.AddCheck(3, std::string("one"));
	Source.AddCheck(4, std::string("one"));

	IntStringRBTree T;
	T.AddCheck(0, "zero");
	T.AddCheck(2, "two_second");

	T.AddRange(Source.Iterator());

	for (int i = 0; i < COUNT; i++)
	{
		BOOST_REQUIRE(T.Contains(i));
	}
}

BOOST_AUTO_TEST_CASE(AddFromRBTreeTest)
{
	constexpr int COUNT = 5;

	IntStringRBTree Source;
	Source.AddCheck(3, "three");
	Source.AddCheck(1, "one");
	Source.AddCheck(2, "two");
	Source.AddCheck(4, "four");

	IntStringRBTree T;
	T.AddCheck(0, "zero");
	T.AddCheck(2, "two_second");

	T.Add(Source);

	for (int i = 0; i < COUNT; i++)
	{
		BOOST_REQUIRE(T.Contains(i));
	}
}

BOOST_AUTO_TEST_CASE(AddMoveFromRBTreeTest)
{
	constexpr int COUNT = 5;

	IntStringRBTree Source;
	Source.AddCheck(3, "three");
	Source.AddCheck(1, "one");
	Source.AddCheck(2, "two");
	Source.AddCheck(4, "four");

	IntStringRBTree T;
	T.AddCheck(0, "zero");

	T.Add(std::move(Source));

	for (int i = 0; i < COUNT; i++)
	{
		BOOST_REQUIRE(T.Contains(i));
	}

	BOOST_REQUIRE(Source.Empty());
}


BOOST_AUTO_TEST_CASE(AddSortedFromVectorTest)
{
	constexpr int COUNT = 6;

	BOOST_TEST_CHECKPOINT("Vector initialization");
	TVector<IntRBTree::KeyValueType> Source
	{
		IntRBTree::KeyValueType{ 0, NoValue{} },
		IntRBTree::KeyValueType{ 1, NoValue{} },
		IntRBTree::KeyValueType{ 2, NoValue{} },
		IntRBTree::KeyValueType{ 3, NoValue{} },
		IntRBTree::KeyValueType{ 4, NoValue{} }
	};

	BOOST_TEST_CHECKPOINT("Initialization");
	IntRBTree T;
	T.AddCheck(5, NoValue{});
	T.AddCheck(1, NoValue{});

	BOOST_TEST_CHECKPOINT("Adding");
	T.AddSorted(Source);
	for (int i = 0; i < COUNT; i++)
	{
		BOOST_REQUIRE(T.Contains(i));
	}
}

BOOST_AUTO_TEST_CASE(AddFromVectorTest)
{
	constexpr int COUNT = 6;

	BOOST_TEST_CHECKPOINT("Vector initialization");
	TVector<IntRBTree::KeyValueType> Source
	{
		IntRBTree::KeyValueType {3, NoValue{} },
		IntRBTree::KeyValueType {1, NoValue{} },
		IntRBTree::KeyValueType {2, NoValue{} },
		IntRBTree::KeyValueType {4, NoValue{} },
		IntRBTree::KeyValueType {0, NoValue{} }
	};

	BOOST_TEST_CHECKPOINT("Initialization");
	IntRBTree T;
	T.AddCheck(5, NoValue{});
	T.AddCheck(1, NoValue{});

	BOOST_TEST_CHECKPOINT("Adding");
	T.Add(Source);
	for (int i = 0; i < COUNT; i++)
	{
		BOOST_REQUIRE(T.Contains(i));
	}
}

BOOST_AUTO_TEST_CASE(AddMovedFromVectorTest)
{
	constexpr int COUNT = 8;

	BOOST_TEST_CHECKPOINT("Vector initialization");
	TVector<IntStringRBTree::KeyValueType> Source
	{
		IntStringRBTree::KeyValueType{ 3, std::string{ "three" } },
		IntStringRBTree::KeyValueType{ 1, std::string{ "one" } },
		IntStringRBTree::KeyValueType{ 2, std::string{ "two" } },
		IntStringRBTree::KeyValueType{ 4, std::string{ "four" } },
		IntStringRBTree::KeyValueType{ 0, std::string{ "zero" } }
	};

	BOOST_TEST_CHECKPOINT("Initialization");
	IntStringRBTree T;
	T.AddCheck(7, std::string{ "seven" });
	T.AddCheck(5, std::string{ "five" });
	T.AddCheck(6, std::string{ "six" });

	BOOST_TEST_CHECKPOINT("Adding");
	T.AddMoved(Source);
	for (int i = 0; i < COUNT; i++)
	{
		BOOST_REQUIRE(T.Contains(i));
	}

	for (int i = 0; i < Source.Num(); i++)
	{
		BOOST_REQUIRE(Source[i].Value.empty());
	}
}

BOOST_AUTO_TEST_CASE(AddInitializeListTest)
{
	std::initializer_list<IntRBTree::KeyValueType> const lst = 
	{
		IntRBTree::KeyValueType{0, NoValue{}},
		IntRBTree::KeyValueType{3, NoValue{}},
		IntRBTree::KeyValueType{5, NoValue{}},
		IntRBTree::KeyValueType{1, NoValue{}},
		IntRBTree::KeyValueType{2, NoValue{}},
		IntRBTree::KeyValueType{4, NoValue{}}
	};

	BOOST_TEST_CHECKPOINT("Initialization");
	IntRBTree T;
	T.AddCheck(0, NoValue{});

	BOOST_TEST_CHECKPOINT("Adding");
	T.Add(lst);

	for (int i = 0; i <= 5; i++)
	{
		BOOST_REQUIRE(T.Contains(i));
	}
}

BOOST_AUTO_TEST_CASE(ExtraAddTest)
{
	const int COUNT = 10;
	// NOTE: At the end will must have all elements from ZERO to [COUNT-1] inclusively.

	BOOST_TEST_CHECKPOINT("Filling source array");
	std::vector<IntRBTree::KeyValueType> SourceArray;
	SourceArray.push_back(IntRBTree::KeyValueType(3, NoValue{}));
	SourceArray.push_back(IntRBTree::KeyValueType(7, NoValue{}));
	SourceArray.push_back(IntRBTree::KeyValueType(1, NoValue{}));
	SourceArray.push_back(IntRBTree::KeyValueType(9, NoValue{}));
	SourceArray.push_back(IntRBTree::KeyValueType(2, NoValue{}));

	BOOST_TEST_CHECKPOINT("Initializing RBTree");
	IntRBTree T;
	T.AddCheck(3, NoValue{});
	T.AddCheck(0, NoValue{});
	T.AddCheck(8, NoValue{});
	T.AddCheck(6, NoValue{});
	T.AddCheck(9, NoValue{});
	T.AddCheck(5, NoValue{});
	T.AddCheck(4, NoValue{});

	BOOST_TEST_CHECKPOINT("Adding elements from SourceArray");
	T.Add(SourceArray.data(), SourceArray.size());
	for (int i = 0; i < COUNT; i++)
	{
		BOOST_REQUIRE(T.Contains(i));
	}
}

BOOST_AUTO_TEST_CASE(ExtraAddSortedTest)
{
	const int COUNT = 10;
	// NOTE: At the end will must have all elements from ZERO to [COUNT-1] inclusively.

	BOOST_TEST_CHECKPOINT("Filling source array");
	std::vector<IntRBTree::KeyValueType> SourceArray;
	SourceArray.push_back(IntRBTree::KeyValueType(1, NoValue{}));
	SourceArray.push_back(IntRBTree::KeyValueType(2, NoValue{}));
	SourceArray.push_back(IntRBTree::KeyValueType(3, NoValue{}));
	SourceArray.push_back(IntRBTree::KeyValueType(7, NoValue{}));
	SourceArray.push_back(IntRBTree::KeyValueType(9, NoValue{}));

	BOOST_TEST_CHECKPOINT("Initializing RBTree");
	IntRBTree T;
	T.AddCheck(3, NoValue{});
	T.AddCheck(0, NoValue{});
	T.AddCheck(8, NoValue{});
	T.AddCheck(6, NoValue{});
	T.AddCheck(9, NoValue{});
	T.AddCheck(5, NoValue{});
	T.AddCheck(4, NoValue{});

	BOOST_TEST_CHECKPOINT("Adding elements from SourceArray");
	T.AddSorted(SourceArray.data(), SourceArray.size());
	for (int i = 0; i < COUNT; i++)
	{
		BOOST_REQUIRE(T.Contains(i));
	}
}

BOOST_AUTO_TEST_CASE(GetMovedByIteratorTest)
{
	constexpr int INITIAL_COUNT = 3;

	IntToUniquePtStr T;
	BOOST_REQUIRE(T.AddCheck(1, std::make_unique<std::string>("one")));
	BOOST_REQUIRE(T.AddCheck(2, std::make_unique<std::string>("two")));
	BOOST_REQUIRE(T.AddCheck(3, std::make_unique<std::string>("three")));
	BOOST_REQUIRE_EQUAL(T.Num(), INITIAL_COUNT);

	IntToUniquePtStr::KeyValueType MovedKV = T.GetMovedByIterator(T.FindIteratorFor(2));
	BOOST_REQUIRE(!T.Contains(2));
	BOOST_REQUIRE_EQUAL(T.Num(), INITIAL_COUNT - 1);
}

BOOST_AUTO_TEST_CASE(MoveByKeyTest)
{
	constexpr int INITIAL_COUNT = 3;

	IntToUniquePtStr T;
	BOOST_REQUIRE(T.AddCheck(1, std::make_unique<std::string>("one")));
	BOOST_REQUIRE(T.AddCheck(2, std::make_unique<std::string>("two")));
	BOOST_REQUIRE(T.AddCheck(3, std::make_unique<std::string>("three")));
	BOOST_REQUIRE_EQUAL(T.Num(), INITIAL_COUNT);

	IntToUniquePtStr::KeyValueType MovedKV;
	BOOST_REQUIRE( ! T.MoveByKey(111, /*Out*/MovedKV) );
	BOOST_REQUIRE_EQUAL( T.Num(), INITIAL_COUNT );

	BOOST_REQUIRE( T.MoveByKey(1, /*Out*/MovedKV) );
	BOOST_REQUIRE( ! T.Contains(1) );
	BOOST_REQUIRE_EQUAL(T.Num(), INITIAL_COUNT - 1);

	// TODO: Here we would check that the moved value is NOT enumerated second time, however have no MoveUnorderedTo function yet.
	/*
	BOOST_TEST_CHECKPOINT("Output sequence must NOT contain the removed KeyValue!!!");
	IntToUniquePtStr::KeyValueType buf[(INITIAL_COUNT-1)];
	T.CopyUnorderedTo(buf);

	IntToUniquePtStr::KeyValueType* const End = buf + INITIAL_COUNT - 1;
	BOOST_REQUIRE(End == std::find_if
	(
		buf, End, 
		[](const IntToUniquePtStr::KeyValueType& InKV) 
		{
			return InKV.Key == /*Removed key*//*1;
		}
	));
	*/
}


BOOST_AUTO_TEST_CASE(MovingFromPartOfCArrayTest)
{
	const int COUNT = 10;
	// NOTE: At the end will must have all elements from ZERO to [COUNT-1] inclusively.

	BOOST_TEST_CHECKPOINT("Filling source array");
	std::vector<IntStringRBTree::KeyValueType> SourceArray;
	SourceArray.push_back(IntStringRBTree::KeyValueType(7, std::string("seventh")));
	SourceArray.push_back(IntStringRBTree::KeyValueType(2, std::string("second")));
	SourceArray.push_back(IntStringRBTree::KeyValueType(3, std::string("third")));	
	SourceArray.push_back(IntStringRBTree::KeyValueType(9, std::string("ninth")));
	SourceArray.push_back(IntStringRBTree::KeyValueType(1, std::string("first")));

	BOOST_TEST_CHECKPOINT("Initializing RBTree");
	IntStringRBTree T;
	T.AddCheck(0, std::string("third_zero"));
	T.AddCheck(8, std::string("third_eight"));
	T.AddCheck(6, std::string("third_six"));
	T.AddCheck(5, std::string("third_five"));
	T.AddCheck(4, std::string("third_fourth"));

	BOOST_TEST_CHECKPOINT("Moving elements from SourceArray");
	T.AddMoved(SourceArray.data(), SourceArray.size());
	for (int i = 0; i < COUNT; i++)
	{
		BOOST_REQUIRE(T.Contains(i));
	}
	for (int i = 0; i < SourceArray.size(); i++)
	{
		BOOST_REQUIRE(SourceArray[i].Value.empty());
	}
}

BOOST_AUTO_TEST_CASE(MovingSortedFromPartOfCArrayTest)
{
	const int COUNT = 10;
	// NOTE: At the end will must have all elements from ZERO to [COUNT-1] inclusively.

	BOOST_TEST_CHECKPOINT("Filling source array");
	std::vector<IntStringRBTree::KeyValueType> SourceArray;
	SourceArray.push_back(IntStringRBTree::KeyValueType(1, std::string("first")));
	SourceArray.push_back(IntStringRBTree::KeyValueType(2, std::string("second")));
	SourceArray.push_back(IntStringRBTree::KeyValueType(3, std::string("third")));
	SourceArray.push_back(IntStringRBTree::KeyValueType(7, std::string("seventh")));
	SourceArray.push_back(IntStringRBTree::KeyValueType(9, std::string("ninth")));

	BOOST_TEST_CHECKPOINT("Initializing RBTree");
	IntStringRBTree T;
	T.AddCheck(0, std::string("third_zero"));
	T.AddCheck(8, std::string("third_eight"));
	T.AddCheck(6, std::string("third_six"));
	T.AddCheck(5, std::string("third_five"));
	T.AddCheck(4, std::string("third_fourth"));

	BOOST_TEST_CHECKPOINT("Moving elements from SourceArray");
	T.AddMovedSorted(SourceArray.data(), SourceArray.size());
	for (int i = 0; i < COUNT; i++)
	{
		BOOST_REQUIRE(T.Contains(i));
	}
	for (int i = 0; i < SourceArray.size(); i++)
	{
		BOOST_REQUIRE(SourceArray[i].Value.empty());
	}
}

BOOST_AUTO_TEST_CASE(AccessTestCase)
{
	IntStringRBTree T;
	BOOST_REQUIRE(T.AddCheck(1, std::string("one")));
	BOOST_REQUIRE(T.AddCheck(2, std::string("two")));

	BOOST_REQUIRE_EQUAL(T[1], std::string("one"));
	BOOST_REQUIRE_EQUAL(T[2], std::string("two"));

	BOOST_REQUIRE(T.GetValueOrNull(1234) == nullptr);
	IntStringRBTree::ValueType* pValue = T.GetValueOrNull(1);
	BOOST_REQUIRE(pValue);
	BOOST_REQUIRE_EQUAL(*pValue, std::string("one"));
}

BOOST_AUTO_TEST_SUITE_END() // ExtraAddOpsSuite

BOOST_AUTO_TEST_CASE(SerializationTest)
{
	constexpr int COUNT = 10;

	BOOST_TEST_CHECKPOINT("Initialization");
	IntRBTree T;
	for (int i = 0; i < COUNT; i++)
	{
		BOOST_REQUIRE(T.AddCheck(i, NoValue{}));
	}

	std::string serialization_buffer;

	BOOST_TEST_CHECKPOINT("Output archive");
	std::stringstream s_in_out_strm { serialization_buffer, std::ios::in | std::ios::out };
	boost::archive::text_oarchive out_archive{ s_in_out_strm };
	out_archive << T;

	BOOST_TEST_CHECKPOINT("Input archive");
	s_in_out_strm.seekg(0);
	boost::archive::text_iarchive input_archive { s_in_out_strm };
	IntRBTree T_deserialized;
	input_archive >> T_deserialized;
	BOOST_REQUIRE_EQUAL(T_deserialized.Num(), COUNT);
	IntRBTree::KeyValueType buf[COUNT];
	T_deserialized.CopyTo(buf);
	for (int i = 0; i < COUNT; i++)
	{
		BOOST_REQUIRE_EQUAL(buf[i].Key, i);
	}
}

BOOST_AUTO_TEST_CASE(ClearingTest)
{
	constexpr int COUNT = 10;

	BOOST_TEST_CHECKPOINT("Preparing");
	TRBTree<KVType<int, TestNonPOD>> T;
	bool DestroyFlags[COUNT];
	std::fill(DestroyFlags, DestroyFlags + COUNT, false);
	for (int i = 0; i < COUNT; i++)
	{
		BOOST_REQUIRE(T.AddCheck(i, TestNonPOD(&DestroyFlags[i])));
	}

	BOOST_TEST_CHECKPOINT("Clearing");
	T.Clear();
	BOOST_REQUIRE(T.Empty());

	bool bTestFlag = false;
	BOOST_TEST_CHECKPOINT("Checking content after cleared");
	TKeyValue<KVType<int, TestNonPOD>> buf { 0, TestNonPOD( &bTestFlag ) };
	T.CopyUnorderedTo(&buf);
	BOOST_REQUIRE_EQUAL(0, buf.Key);

	for (int i = 0; i < COUNT; i++)
	{
		BOOST_REQUIRE( DestroyFlags[i] );
	}
}

BOOST_AUTO_TEST_CASE(ReplaceTest)
{
	BOOST_TEST_CHECKPOINT("Preparing");
	IntStringRBTree T;
	const IntStringRBTree::KeyValueType RefKV = IntStringRBTree::KeyValueType{ 1, std::string("first") };
	const IntStringRBTree::KeyValueType RefKV_2 = IntStringRBTree::KeyValueType{ 1, std::string("first_one") };

	const IntStringRBTree::KeyValueType RefKV_Two = IntStringRBTree::KeyValueType{ 2, std::string("second") };

	IntStringRBTree::IteratorType It = T.Replace(RefKV);
	BOOST_REQUIRE( It );
	BOOST_REQUIRE( It.GetKeyValue() == RefKV  );

	IntStringRBTree::IteratorType ItSecond = T.Replace(RefKV_Two);
	BOOST_REQUIRE(ItSecond);
	BOOST_REQUIRE(ItSecond.GetKeyValue() == RefKV_Two);
	BOOST_REQUIRE(It);
	BOOST_REQUIRE(It.GetKeyValue() == RefKV);

	It = T.Replace(RefKV_2);
	BOOST_REQUIRE(It);
	BOOST_REQUIRE(It.GetKeyValue() == RefKV_2);
}

BOOST_AUTO_TEST_CASE(GetOrAddTest)
{
	BOOST_TEST_CHECKPOINT("Preparing");
	IntStringRBTree T;
	BOOST_REQUIRE(T.Empty());
	const IntStringRBTree::KeyValueType RefKV = IntStringRBTree::KeyValueType{ 1, std::string("first") };
	IntStringRBTree::IteratorType const It_KV = T.GetOrAdd(RefKV);
	BOOST_REQUIRE( ! It_KV.IsEnd() );
	BOOST_REQUIRE_EQUAL(It_KV.GetKeyValue(), RefKV);
	IntStringRBTree::IteratorType const It_KV_second_time_diff_value = T.GetOrAdd(IntStringRBTree::KeyValueType{ RefKV.Key, std::string("second") });
	BOOST_REQUIRE( ! It_KV_second_time_diff_value.IsEnd() );
	BOOST_REQUIRE_EQUAL(It_KV_second_time_diff_value, It_KV);
	BOOST_REQUIRE_EQUAL(It_KV_second_time_diff_value.GetKeyValue(), It_KV.GetKeyValue());
	IntStringRBTree::IteratorType const It_KV_key_value_args = T.GetOrAdd(1, std::string("first"));
	BOOST_REQUIRE(!It_KV_key_value_args.IsEnd());
	BOOST_REQUIRE_EQUAL(It_KV_key_value_args, It_KV);

	std::string const s_value("first");
	IntStringRBTree::IteratorType const It_KV_key_value_args_2 = T.GetOrAdd(1, s_value);
	BOOST_REQUIRE(!It_KV_key_value_args_2.IsEnd());
	BOOST_REQUIRE_EQUAL(It_KV_key_value_args_2, It_KV);


	int const KEY = 1;
	IntStringRBTree::IteratorType const It_KV_key_value_args_3 = T.GetOrAdd(KEY, s_value);
	BOOST_REQUIRE(!It_KV_key_value_args_3.IsEnd());
	BOOST_REQUIRE_EQUAL(It_KV_key_value_args_3, It_KV);

	IntStringRBTree::IteratorType const It_KV_key_value_args_4 = T.GetOrAdd(KEY, std::string("first"));
	BOOST_REQUIRE(!It_KV_key_value_args_4.IsEnd());
	BOOST_REQUIRE_EQUAL(It_KV_key_value_args_4, It_KV);
}

BOOST_AUTO_TEST_SUITE_END() // ExtraOps

BOOST_AUTO_TEST_SUITE
(
	NonPODTestSuite,
	*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/AddSuite")
	*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/RemoveSuite")
)
BOOST_AUTO_TEST_CASE(DeletedProperlyTest)
{
	TRBTree<KVType<int, TestNonPOD>> T;
	bool bDeleted = false;
	BOOST_REQUIRE(T.AddCheck(1, TestNonPOD(&bDeleted)));
	BOOST_REQUIRE(T.Remove(1));
	BOOST_REQUIRE( bDeleted );
}
BOOST_AUTO_TEST_SUITE_END() // NonPODTestSuite

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
	BOOST_REQUIRE(T.AddCheck(PTR_TO_KEY_ONE, NoValue{}));

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
	BOOST_REQUIRE(T.AddCheck(KEY_ONE, NoValue{}));

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
	BOOST_REQUIRE(T.AddCheck(KEY_ONE, NoValue{}));
	BOOST_REQUIRE_EQUAL(1, T.Num());
	BOOST_REQUIRE( ! T.Empty() );
	BOOST_REQUIRE( T.Contains(KEY_ONE) );
	BOOST_REQUIRE( ! T.Contains(KEY_TWO) );
	BOOST_REQUIRE( ! T.Contains(KEY_ZERO) );
	BOOST_REQUIRE( T.Find(KEY_ONE) );
	BOOST_REQUIRE_EQUAL( KEY_ONE, T.Find(KEY_ONE)->Key );

	BOOST_TEST_CHECKPOINT("Add Already Included");
	BOOST_REQUIRE( ! T.AddCheck(KEY_ONE, NoValue{}) );
	BOOST_REQUIRE_EQUAL( 1, T.Num() );

	BOOST_TEST_CHECKPOINT("Add another (on the right of the root)");
	BOOST_REQUIRE( T.AddCheck(KEY_TWO, NoValue{}) );
	BOOST_REQUIRE( T.Contains(KEY_TWO) );
	BOOST_REQUIRE( T.Contains(KEY_ONE) );
	BOOST_REQUIRE_EQUAL( KEY_TWO, T.Find(KEY_TWO)->Key );
	BOOST_REQUIRE_EQUAL( KEY_ONE, T.Find(KEY_ONE)->Key );
	BOOST_REQUIRE_EQUAL( 2, T.Num() );

	BOOST_TEST_CHECKPOINT("Add another (on the left of the root)");
	BOOST_REQUIRE(T.AddCheck(KEY_ZERO, NoValue{}));
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
	BOOST_REQUIRE(T.AddCheck(KEY_FIVE, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_THREE, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_SEVEN, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_SIX, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_TWO, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_ONE, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_FOUR, NoValue{}));
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
	BOOST_REQUIRE(T.AddCheck(KEY_ONE, NoValue{}));

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

	BOOST_REQUIRE(T.AddCheck(STR_KEY_ONE, 1));
	BOOST_REQUIRE(T.AddCheck(STR_KEY_TWO, 2));
	BOOST_REQUIRE(T.AddCheck(STR_KEY_THREE, 3));

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
	BOOST_REQUIRE(T.AddCheck(KEY_TWO, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_ONE, NoValue{}));

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

	BOOST_REQUIRE(T.AddCheck(KEY_ONE, 1));
	BOOST_REQUIRE(T.AddCheck(KEY_TWO, 2));
	BOOST_REQUIRE(T.AddCheck(KEY_THREE, 3));

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
	BOOST_REQUIRE(T.AddCheck(KEY_TWO, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_ONE, NoValue{}));

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
	BOOST_REQUIRE(T.AddCheck(KEY_THREE, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_TWO, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_ONE, NoValue{}));

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
	BOOST_REQUIRE(T.AddCheck(KEY_FOUR, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_THREE, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_TWO, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_ONE, NoValue{}));

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
	BOOST_REQUIRE(T.AddCheck(KEY_ONE, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_TWO, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_THREE, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_FOUR, NoValue{}));

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
	BOOST_REQUIRE(T.AddCheck(KEY_ONE, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_TWO, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_THREE, NoValue{}));

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
	BOOST_REQUIRE(T.AddCheck(KEY_ONE, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_TWO, NoValue{}));

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
	BOOST_REQUIRE(T.AddCheck(KEY_TWO, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_ONE, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_THREE, NoValue{}));

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
	BOOST_REQUIRE(T.AddCheck(KEY_FOUR, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_TWO, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_SIX, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_ONE, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_THREE, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_FIVE, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_SEVEN, NoValue{}));

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
	BOOST_REQUIRE(T.AddCheck(KEY_SIX, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_EIGHT, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_SEVEN, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_NINE, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_TWO, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_ONE, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_FIVE, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_THREE, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_FOUR, NoValue{}));

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
	BOOST_REQUIRE(T.AddCheck(KEY_TWO, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_THREE, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_FIVE, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_SEVEN, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_EIGHT, NoValue{}));

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
	BOOST_REQUIRE(T.AddCheck(KEY_TWO, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_THREE, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_FIVE, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_SEVEN, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_EIGHT, NoValue{}));

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
	BOOST_REQUIRE(T.AddCheck(KEY_FIVE, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_TWO, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_THREE, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_SEVEN, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_EIGHT, NoValue{}));

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
	BOOST_REQUIRE(T.AddCheck(KEY_FIVE, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_TWO, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_THREE, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_SEVEN, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_EIGHT, NoValue{}));

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
	T.AddCheck(KEY_EXISTING, NoValue{});
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
	T.AddCheck(KEY_ROOT, NoValue{});
	T.AddCheck(KEY_LEFT_CHILD, NoValue{});
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
	T.AddCheck(KEY_ROOT, NoValue{});
	T.AddCheck(KEY_RIGHT_CHILD, NoValue{});
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
		T.AddCheck(i, NoValue{});
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
		T.AddCheck(i, NoValue{});
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
	T.AddCheck(2, NoValue{});
	T.AddCheck(1, NoValue{});
	T.AddCheck(4, NoValue{});
	T.AddCheck(3, NoValue{});
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
		T.AddCheck(i, NoValue{});
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
	T.AddCheck(KEY_ROOT, NoValue{});
	T.AddCheck(KEY_LEFT_CHILD, NoValue{});
	T.AddCheck(KEY_RIGHT_CHILD, NoValue{});
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
	T.AddCheck(KEY_ROOT, NoValue{});
	T.AddCheck(KEY_LEFT_CHILD, NoValue{});
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
	BOOST_REQUIRE(T.AddCheck(KEY_ROOT, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_LEFT_CHILD, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_RIGHT_CHILD, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_LEFT_LEFT_CHILD_CHILD, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_LEFT_RIGHT_CHILD_CHILD, NoValue{}));
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
	BOOST_REQUIRE(T.AddCheck(KEY_ROOT, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_LEFT_CHILD, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_RIGHT_CHILD, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_LEFT_LEFT_CHILD_CHILD, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_BROTHER_RIGHT_CHILD, NoValue{}));
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
	BOOST_REQUIRE(T.AddCheck(KEY_ROOT, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_LEFT_CHILD, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_RIGHT_CHILD, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_LEFT_LEFT_CHILD_CHILD, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_LEFT_RIGHT_CHILD_CHILD, NoValue{}));
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
	BOOST_REQUIRE(T.AddCheck(KEY_ROOT, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_LEFT_CHILD, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_RIGHT_CHILD, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_RIGHT_LEFT_CHILD_CHILD, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_RIGHT_RIGHT_CHILD_CHILD, NoValue{}));
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
	BOOST_REQUIRE(T.AddCheck(KEY_ROOT, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_LEFT_CHILD, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_RIGHT_CHILD, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_LEFT_LEFT_CHILD_CHILD, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_LEFT_RIGHT_CHILD_CHILD, NoValue{}));
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
	T.AddCheck(KEY_ROOT, NoValue{});
	T.AddCheck(KEY_RIGHT_CHILD, NoValue{});
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
	T.AddCheck(KEY_FIRST, NoValue{});
	T.AddCheck(KEY_SECOND, NoValue{});
	T.AddCheck(KEY_THIRD, NoValue{});
	T.AddCheck(KEY_FOURTH, NoValue{});
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
	T.AddCheck(KEY_FIRST, NoValue{});
	T.AddCheck(KEY_SECOND, NoValue{});
	T.AddCheck(KEY_THIRD, NoValue{});
	T.AddCheck(KEY_FOURTH, NoValue{});
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
	T.AddCheck(KEY_FIRST, NoValue{});
	T.AddCheck(KEY_SECOND, NoValue{});
	T.AddCheck(KEY_THIRD, NoValue{});
	T.AddCheck(KEY_FOURTH, NoValue{});
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
	T.AddCheck(KEY_FIRST, NoValue{});
	T.AddCheck(KEY_SECOND, NoValue{});
	T.AddCheck(KEY_THIRD, NoValue{});
	T.AddCheck(KEY_FOURTH, NoValue{});
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
	T.AddCheck(KEY_FIRST, NoValue{});
	T.AddCheck(KEY_SECOND, NoValue{});
	T.AddCheck(KEY_THIRD, NoValue{});
	T.AddCheck(KEY_FOURTH, NoValue{});
	T.AddCheck(KEY_FIVE, NoValue{});
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
	T.AddCheck(KEY_FIRST, NoValue{});
	T.AddCheck(KEY_SECOND, NoValue{});
	T.AddCheck(KEY_THIRD, NoValue{});
	T.AddCheck(KEY_FOURTH, NoValue{});
	T.AddCheck(KEY_FIVE, NoValue{});
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
	T.AddCheck(KEY_FIRST, NoValue{});
	T.AddCheck(KEY_SECOND, NoValue{});
	T.AddCheck(KEY_THIRD, NoValue{});
	T.AddCheck(KEY_FOURTH, NoValue{});
	T.AddCheck(KEY_FIVE, NoValue{});
	T.AddCheck(KEY_SIX, NoValue{});
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
	T.AddCheck(KEY_FIRST, NoValue{});
	T.AddCheck(KEY_SECOND, NoValue{});
	T.AddCheck(KEY_THIRD, NoValue{});
	T.AddCheck(KEY_FOURTH, NoValue{});
	T.AddCheck(KEY_FIVE, NoValue{});
	T.AddCheck(KEY_SIX, NoValue{});
	T.AddCheck(KEY_SEVEN, NoValue{});
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
	T.AddCheck(KEY_FIRST, NoValue{});
	T.AddCheck(KEY_SECOND, NoValue{});
	T.AddCheck(KEY_THIRD, NoValue{});
	T.AddCheck(KEY_FOURTH, NoValue{});
	T.AddCheck(KEY_FIVE, NoValue{});
	T.AddCheck(KEY_SIX, NoValue{});
	T.AddCheck(KEY_SEVEN, NoValue{});
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
		BOOST_REQUIRE(T.AddCheck(i, NoValue{}));
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
		BOOST_REQUIRE(T.AddCheck(i, NoValue{}));
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
		BOOST_REQUIRE(T.AddCheck(i, NoValue{}));
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
	BOOST_REQUIRE(T.AddCheck(KEY_TWO, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_ONE, NoValue{}));

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
	BOOST_REQUIRE(T.AddCheck(KEY_THREE, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_TWO, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_ONE, NoValue{}));

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
	BOOST_REQUIRE(T.AddCheck(KEY_FOUR, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_THREE, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_TWO, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_ONE, NoValue{}));

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
		BOOST_REQUIRE(T.AddCheck(i, NoValue{}));
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
	BOOST_REQUIRE(T.AddCheck(KEY_THREE, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_ONE, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_TWO, NoValue{}));

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
	BOOST_REQUIRE(T.AddCheck(KEY_ONE, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_TWO, NoValue{}));

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
	BOOST_REQUIRE(T.AddCheck(KEY_ONE, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_TWO, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_THREE, NoValue{}));

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
	BOOST_REQUIRE(T.AddCheck(KEY_ONE, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_TWO, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_THREE, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_FOUR, NoValue{}));

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
		BOOST_REQUIRE(T.AddCheck(i, NoValue{}));
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
	BOOST_REQUIRE(T.AddCheck(KEY_ONE, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_THREE, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_TWO, NoValue{}));

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
	BOOST_REQUIRE(T.AddCheck(KEY_SIX, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_EIGHT, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_FOUR, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_SEVEN, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_NINE, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_TWO, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_THREE, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_FIVE, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_ONE, NoValue{}));

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
	BOOST_REQUIRE(T.AddCheck(KEY_ONE, NoValue{}));

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
	BOOST_REQUIRE(T.AddCheck(KEY_ONE, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_TWO, NoValue{}));

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
	BOOST_REQUIRE(T.AddCheck(KEY_ONE, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_TWO, NoValue{}));

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
	BOOST_REQUIRE(T.AddCheck(KEY_ONE, NoValue{}));

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
	BOOST_REQUIRE(T.AddCheck(KEY_SIX, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_EIGHT, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_FOUR, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_SEVEN, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_NINE, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_TWO, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_THREE, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_FIVE, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_ONE, NoValue{}));

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
	BOOST_REQUIRE(T.AddCheck(KEY_SIX, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_EIGHT, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_FOUR, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_SEVEN, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_NINE, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_TWO, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_THREE, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_FIVE, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_ONE, NoValue{}));

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
	BOOST_REQUIRE(T.AddCheck(KEY_SIX, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_EIGHT, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_FOUR, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_SEVEN, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_NINE, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_TWO, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_THREE, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_FIVE, NoValue{}));
	BOOST_REQUIRE(T.AddCheck(KEY_ONE, NoValue{}));

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