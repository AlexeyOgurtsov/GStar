#pragma once

#include "TRBTreeTestUtils.h"

namespace
{
	BOOST_AUTO_TEST_SUITE(Core)
	BOOST_AUTO_TEST_SUITE(Container)
	BOOST_AUTO_TEST_SUITE(TRBTreeTestSuite)

		BOOST_AUTO_TEST_SUITE
		(
			ExtraOps,
			*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/AddSuite")
			*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/RemoveSuite")
		)

		BOOST_AUTO_TEST_SUITE(CountTestSuite)


		BOOST_AUTO_TEST_CASE(CountTest)
	{
		using TreeType = TRBTree<KVType<int, std::string>>;
		constexpr int INITIAL_COUNT = 8;
		constexpr int TOTAL_P_KEY_COUNT_IN_RANGE = 2;
		constexpr int TOTAL_P_KEY_COUNT = 4;
		constexpr int TOTAL_VALUE_TO_COUNT = 4;
		constexpr int TOTAL_OTHER_VALUE = INITIAL_COUNT - TOTAL_VALUE_TO_COUNT;
		std::string const VALUE_TO_COUNT = "";
		std::string const OTHER_VALUE = "";


		const auto P = [](const TreeType::KeyValueType& KV)
		{
			if (KV.Key % 2 == 0) { return true; }
			return false;
		};

		const auto PK = [](const TreeType::KeyType& K)
		{
			if (K % 2 == 0) { return true; }
			return false;
		};

		const auto PV_Empty = [](const TreeType::ValueType& V)
		{
			if (V.empty()) { return true; }
			return false;
		};

		BOOST_TEST_CHECKPOINT("Empty");
		TreeType T;
		BOOST_REQUIRE_EQUAL(0, T.CountPredicate(P));
		BOOST_REQUIRE_EQUAL(0, T.CountNotPredicate(P));
		BOOST_REQUIRE_EQUAL(0, T.CountPredicate_InRange(P, T.Iterator(), T.EndIterator()));
		BOOST_REQUIRE_EQUAL(0, T.CountNotPredicate_InRange(P, T.Iterator(), T.EndIterator()));
		BOOST_REQUIRE_EQUAL(0, T.CountValues(0));
		BOOST_REQUIRE_EQUAL(0, T.CountValues_InRange(0, T.Iterator(), T.EndIterator()));
		BOOST_REQUIRE_EQUAL(0, T.CountOtherValues(0));
		BOOST_REQUIRE_EQUAL(0, T.CountOtherValues_InRange(0, T.Iterator(), T.EndIterator()));
		BOOST_REQUIRE_EQUAL(0, T.CountValuePredicate(PV_Empty));
		BOOST_REQUIRE_EQUAL(0, T.CountValuePredicate_InRange(PV_Empty, T.Iterator(), T.EndIterator()));
		BOOST_REQUIRE_EQUAL(0, T.CountNotValuePredicate(PV_Empty));
		BOOST_REQUIRE_EQUAL(0, T.CountNotValuePredicate_InRange(PV_Empty, T.Iterator(), T.EndIterator()));
		BOOST_REQUIRE_EQUAL(0, T.CountKeyPredicate(PK));
		BOOST_REQUIRE_EQUAL(0, T.CountNotKeyPredicate(PK));
		BOOST_REQUIRE_EQUAL(0, T.CountKeyPredicate_InRange(PK, T.Iterator(), T.EndIterator()));
		BOOST_REQUIRE_EQUAL(0, T.CountNotKeyPredicate_InRange(PK, T.Iterator(), T.EndIterator()));

		BOOST_TEST_CHECKPOINT("Fill");
		T.Add(1, VALUE_TO_COUNT);
		T.Add(2, OTHER_VALUE);
		T.Add(3, VALUE_TO_COUNT);
		T.Add(4, VALUE_TO_COUNT);
		T.Add(5, OTHER_VALUE);
		T.Add(6, OTHER_VALUE);
		T.Add(7, VALUE_TO_COUNT);
		T.Add(8, OTHER_VALUE);

		BOOST_REQUIRE_EQUAL(T.MinKey(), 1);

		TreeType::ConstIteratorType const It_Second = T.FindIteratorFor(2);
		TreeType::ConstIteratorType const It_Sixth = T.FindIteratorFor(6);

		TreeType::ConstIteratorType const It_After_ValueToCount = GetNextIt(T.FindIteratorFor(1));
		TreeType::ConstIteratorType const It_LastValueToCount = T.FindIteratorFor(7);
		BOOST_REQUIRE(It_LastValueToCount && It_LastValueToCount.GetValue() == VALUE_TO_COUNT);

		BOOST_REQUIRE_EQUAL(T.Num(), INITIAL_COUNT);

		BOOST_TEST_CHECKPOINT("Total key count");
		int32_t const KeyCount = T.CountKeyPredicate(PK);
		BOOST_REQUIRE_EQUAL(KeyCount, TOTAL_P_KEY_COUNT);
		BOOST_REQUIRE_EQUAL(T.CountPredicate(P), KeyCount);

		BOOST_TEST_CHECKPOINT("Total predicate count in range");
		BOOST_REQUIRE_EQUAL(T.CountKeyPredicate_InRange(PK, It_Second, It_Sixth), (TOTAL_P_KEY_COUNT_IN_RANGE));
		BOOST_REQUIRE_EQUAL(T.CountPredicate_InRange(P, It_Second, It_Sixth), (TOTAL_P_KEY_COUNT_IN_RANGE));

		BOOST_TEST_CHECKPOINT("Total not predicate count");
		BOOST_REQUIRE_EQUAL(T.CountNotKeyPredicate(PK), (INITIAL_COUNT - TOTAL_P_KEY_COUNT));
		BOOST_REQUIRE_EQUAL(T.CountNotPredicate(P), (INITIAL_COUNT - TOTAL_P_KEY_COUNT));

		BOOST_TEST_CHECKPOINT("Total not key predicate count in range");
		BOOST_REQUIRE_EQUAL(T.CountNotKeyPredicate_InRange(PK, It_Second, It_Sixth), (INITIAL_COUNT - TOTAL_P_KEY_COUNT_IN_RANGE));
		BOOST_REQUIRE_EQUAL(T.CountNotPredicate_InRange(P, It_Second, It_Sixth), (INITIAL_COUNT - TOTAL_P_KEY_COUNT_IN_RANGE));

		BOOST_TEST_CHECKPOINT("Total value count");
		BOOST_REQUIRE_EQUAL(T.CountValues(VALUE_TO_COUNT), TOTAL_VALUE_TO_COUNT);
		BOOST_REQUIRE_EQUAL(T.CountOtherValues(VALUE_TO_COUNT), TOTAL_OTHER_VALUE);

		BOOST_TEST_CHECKPOINT("Total value predicate");
		BOOST_REQUIRE_EQUAL(T.CountValuePredicate(PV_Empty), TOTAL_VALUE_TO_COUNT);
		BOOST_REQUIRE_EQUAL(T.CountNotValuePredicate(PV_Empty), TOTAL_OTHER_VALUE);

		BOOST_TEST_CHECKPOINT("Total value count in range");
		int32_t const ValueCountInRange = T.CountValues_InRange(VALUE_TO_COUNT, It_After_ValueToCount, It_LastValueToCount);
		BOOST_REQUIRE_EQUAL(ValueCountInRange, (TOTAL_VALUE_TO_COUNT - 2));
		BOOST_REQUIRE_EQUAL(T.CountOtherValues_InRange(VALUE_TO_COUNT, It_After_ValueToCount, It_LastValueToCount), (INITIAL_COUNT - ValueCountInRange));

		BOOST_TEST_CHECKPOINT("Total value predicate in range");
		BOOST_REQUIRE_EQUAL(T.CountValuePredicate_InRange(PV_Empty, It_After_ValueToCount, It_LastValueToCount), (TOTAL_VALUE_TO_COUNT - 2));
		BOOST_REQUIRE_EQUAL(T.CountNotValuePredicate_InRange(PV_Empty, It_After_ValueToCount, It_LastValueToCount), (INITIAL_COUNT - ValueCountInRange));
	}

	BOOST_AUTO_TEST_SUITE_END() // CountTestSuite

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
		BOOST_REQUIRE(!(A != B));

		BOOST_TEST_CHECKPOINT("Inequality by first element (A > B)");
		A.Add(std::string("one"), 1);
		BOOST_REQUIRE(A != B);
		BOOST_REQUIRE(B != A);

		BOOST_TEST_CHECKPOINT("Equality of one-element containers");
		B.Add(std::string("one"), 1);
		BOOST_REQUIRE(A == B);
		BOOST_REQUIRE(B == A);

		BOOST_TEST_CHECKPOINT("Inequality by first element (B > A)");
		B.Add(std::string("two"), 2);
		BOOST_REQUIRE(A != B);

		BOOST_TEST_CHECKPOINT("Equality of two-element containers");
		A.Add(std::string("two"), 2);
		BOOST_REQUIRE(A == B);

		BOOST_TEST_CHECKPOINT("Non equality if key is the same by value differs");
		A.Add(std::string("three"), 3);
		B.Add(std::string("three"), 2);
		BOOST_REQUIRE(A != B);
	}

	BOOST_AUTO_TEST_CASE(AppendRange)
	{
		const int COUNT = 6;

		TVector<IntStringRBTree::KeyValueType> Source;
		Source.Add(IntStringRBTree::KeyValueType(2, std::string("one")));
		Source.Add(IntStringRBTree::KeyValueType(1, std::string("one")));
		Source.Add(IntStringRBTree::KeyValueType(3, std::string("one")));
		Source.Add(IntStringRBTree::KeyValueType(4, std::string("one")));
		Source.Add(IntStringRBTree::KeyValueType(5, std::string("five"))); // this element will be OUT of range

		IntStringRBTree T;
		T.Add(0, "zero");

		T.AppendRange(Source.Iterator(), Source.IteratorAt(Source.LastIndex()));

		for (int i = 0; i < (COUNT - 1); i++)
		{
			BOOST_REQUIRE(T.Contains(i));
		}
	}

	BOOST_AUTO_TEST_CASE(AddAllRange)
	{
		const int COUNT = 6;

		IntStringRBTree Source;
		Source.Add(5, std::string("five"));
		Source.Add(2, std::string("one"));
		Source.Add(1, std::string("one"));
		Source.Add(3, std::string("one"));
		Source.Add(4, std::string("one"));

		IntStringRBTree T;
		T.Add(0, "zero");
		T.Add(2, "two_second");

		T.AppendRange(Source.Iterator());

		for (int i = 0; i < COUNT; i++)
		{
			BOOST_REQUIRE(T.Contains(i));
		}
	}

	BOOST_AUTO_TEST_CASE(AddFromRBTreeTest)
	{
		constexpr int COUNT = 5;

		IntStringRBTree Source;
		Source.Add(3, "three");
		Source.Add(1, "one");
		Source.Add(2, "two");
		Source.Add(4, "four");

		IntStringRBTree T;
		T.Add(0, "zero");
		T.Add(2, "two_second");

		T.Append(Source);

		for (int i = 0; i < COUNT; i++)
		{
			BOOST_REQUIRE(T.Contains(i));
		}
	}

	BOOST_AUTO_TEST_CASE(AddMoveFromRBTreeTest)
	{
		constexpr int COUNT = 5;

		IntStringRBTree Source;
		Source.Add(3, "three");
		Source.Add(1, "one");
		Source.Add(2, "two");
		Source.Add(4, "four");

		IntStringRBTree T;
		T.Add(0, "zero");

		T.Append(std::move(Source));

		for (int i = 0; i < COUNT; i++)
		{
			BOOST_REQUIRE(T.Contains(i));
		}

		BOOST_REQUIRE(Source.Empty());
	}


	BOOST_AUTO_TEST_CASE(AppendSortedFromVectorTest)
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
		T.Add(5, NoValue{});
		T.Add(1, NoValue{});

		BOOST_TEST_CHECKPOINT("Adding");
		T.AppendSorted(Source);
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
			IntRBTree::KeyValueType{ 3, NoValue{} },
			IntRBTree::KeyValueType{ 1, NoValue{} },
			IntRBTree::KeyValueType{ 2, NoValue{} },
			IntRBTree::KeyValueType{ 4, NoValue{} },
			IntRBTree::KeyValueType{ 0, NoValue{} }
		};

		BOOST_TEST_CHECKPOINT("Initialization");
		IntRBTree T;
		T.Add(5, NoValue{});
		T.Add(1, NoValue{});

		BOOST_TEST_CHECKPOINT("Adding");
		T.Append(Source);
		for (int i = 0; i < COUNT; i++)
		{
			BOOST_REQUIRE(T.Contains(i));
		}
	}

	BOOST_AUTO_TEST_CASE(AppendMovedFromVectorTest)
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
		T.Add(7, std::string{ "seven" });
		T.Add(5, std::string{ "five" });
		T.Add(6, std::string{ "six" });

		BOOST_TEST_CHECKPOINT("Adding");
		T.AppendMoved(Source);
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
			IntRBTree::KeyValueType{ 0, NoValue{} },
			IntRBTree::KeyValueType{ 3, NoValue{} },
			IntRBTree::KeyValueType{ 5, NoValue{} },
			IntRBTree::KeyValueType{ 1, NoValue{} },
			IntRBTree::KeyValueType{ 2, NoValue{} },
			IntRBTree::KeyValueType{ 4, NoValue{} }
		};

		BOOST_TEST_CHECKPOINT("Initialization");
		IntRBTree T;
		T.Add(0, NoValue{});

		BOOST_TEST_CHECKPOINT("Adding");
		T.Append(lst);

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
		T.Add(3, NoValue{});
		T.Add(0, NoValue{});
		T.Add(8, NoValue{});
		T.Add(6, NoValue{});
		T.Add(9, NoValue{});
		T.Add(5, NoValue{});
		T.Add(4, NoValue{});

		BOOST_TEST_CHECKPOINT("Adding elements from SourceArray");
		T.Append(SourceArray.data(), SourceArray.size());
		for (int i = 0; i < COUNT; i++)
		{
			BOOST_REQUIRE(T.Contains(i));
		}
	}

	BOOST_AUTO_TEST_CASE(ExtraAppendSortedTest)
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
		T.Add(3, NoValue{});
		T.Add(0, NoValue{});
		T.Add(8, NoValue{});
		T.Add(6, NoValue{});
		T.Add(9, NoValue{});
		T.Add(5, NoValue{});
		T.Add(4, NoValue{});

		BOOST_TEST_CHECKPOINT("Adding elements from SourceArray");
		T.AppendSorted(SourceArray.data(), SourceArray.size());
		for (int i = 0; i < COUNT; i++)
		{
			BOOST_REQUIRE(T.Contains(i));
		}
	}

	BOOST_AUTO_TEST_CASE(GetMovedByIteratorTest)
	{
		constexpr int INITIAL_COUNT = 3;

		IntToUniquePtStr T;
		BOOST_REQUIRE(T.Add(1, std::make_unique<std::string>("one")));
		BOOST_REQUIRE(T.Add(2, std::make_unique<std::string>("two")));
		BOOST_REQUIRE(T.Add(3, std::make_unique<std::string>("three")));
		BOOST_REQUIRE_EQUAL(T.Num(), INITIAL_COUNT);

		IntToUniquePtStr::KeyValueType MovedKV = T.GetMovedByIterator(T.FindIteratorFor(2));
		BOOST_REQUIRE(!T.Contains(2));
		BOOST_REQUIRE_EQUAL(T.Num(), INITIAL_COUNT - 1);
	}

	BOOST_AUTO_TEST_CASE(MoveByKeyTest)
	{
		constexpr int INITIAL_COUNT = 3;

		IntToUniquePtStr T;
		BOOST_REQUIRE(T.Add(1, std::make_unique<std::string>("one")));
		BOOST_REQUIRE(T.Add(2, std::make_unique<std::string>("two")));
		BOOST_REQUIRE(T.Add(3, std::make_unique<std::string>("three")));
		BOOST_REQUIRE_EQUAL(T.Num(), INITIAL_COUNT);

		IntToUniquePtStr::KeyValueType MovedKV;
		BOOST_REQUIRE(!T.MoveByKey(111, /*Out*/MovedKV));

		BOOST_REQUIRE_EQUAL(T.Num(), INITIAL_COUNT);

		BOOST_REQUIRE(T.MoveByKey(1, /*Out*/MovedKV));
		BOOST_REQUIRE(!T.Contains(1));
		BOOST_REQUIRE_EQUAL(T.Num(), INITIAL_COUNT - 1);

		// TODO: Here we would check that the moved value is NOT enumerated second time, however have no MoveUnorderedTo function yet.
		//
		//BOOST_TEST_CHECKPOINT("Output sequence must NOT contain the removed KeyValue!!!");
		//	IntToUniquePtStr::KeyValueType buf[(INITIAL_COUNT-1)];
		//T.CopyUnorderedTo(buf);

		//IntToUniquePtStr::KeyValueType* const End = buf + INITIAL_COUNT - 1;
		//BOOST_REQUIRE(End == std::find_if
		//(
		//	buf, End, 
		//	[](const IntToUniquePtStr::KeyValueType& InKV) 
		//	{
		//		return InKV.Key == /*Removed key*//*1;
		//	}
		//));
		//
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
		T.Add(0, std::string("third_zero"));
		T.Add(8, std::string("third_eight"));
		T.Add(6, std::string("third_six"));
		T.Add(5, std::string("third_five"));
		T.Add(4, std::string("third_fourth"));

		BOOST_TEST_CHECKPOINT("Moving elements from SourceArray");
		T.AppendMoved(SourceArray.data(), SourceArray.size());
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
		T.Add(0, std::string("third_zero"));
		T.Add(8, std::string("third_eight"));
		T.Add(6, std::string("third_six"));
		T.Add(5, std::string("third_five"));
		T.Add(4, std::string("third_fourth"));

		BOOST_TEST_CHECKPOINT("Moving elements from SourceArray");
		T.AppendMovedSorted(SourceArray.data(), SourceArray.size());
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
		BOOST_REQUIRE(T.Add(1, std::string("one")));
		BOOST_REQUIRE(T.Add(2, std::string("two")));

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
			BOOST_REQUIRE(T.Add(i, NoValue{}));
		}

		std::string serialization_buffer;

		BOOST_TEST_CHECKPOINT("Output archive");
		std::stringstream s_in_out_strm{ serialization_buffer, std::ios::in | std::ios::out };
		boost::archive::text_oarchive out_archive{ s_in_out_strm };
		out_archive << T;

		BOOST_TEST_CHECKPOINT("Input archive");
		s_in_out_strm.seekg(0);
		boost::archive::text_iarchive input_archive{ s_in_out_strm };
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
			BOOST_REQUIRE(T.Add(i, TestNonPOD(&DestroyFlags[i])));
		}

		BOOST_TEST_CHECKPOINT("Clearing");
		T.Clear();
		BOOST_REQUIRE(T.Empty());

		bool bTestFlag = false;
		BOOST_TEST_CHECKPOINT("Checking content after cleared");
		TKeyValue<KVType<int, TestNonPOD>> buf{ 0, TestNonPOD(&bTestFlag) };
		T.CopyUnorderedTo(&buf);
		BOOST_REQUIRE_EQUAL(0, buf.Key);

		for (int i = 0; i < COUNT; i++)
		{
			BOOST_REQUIRE(DestroyFlags[i]);
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
		BOOST_REQUIRE(It);
		BOOST_REQUIRE(It.GetKeyValue() == RefKV);

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
		BOOST_REQUIRE(!It_KV.IsEnd());
		BOOST_REQUIRE_EQUAL(It_KV.GetKeyValue(), RefKV);
		IntStringRBTree::IteratorType const It_KV_second_time_diff_value = T.GetOrAdd(IntStringRBTree::KeyValueType{ RefKV.Key, std::string("second") });
		BOOST_REQUIRE(!It_KV_second_time_diff_value.IsEnd());
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
		BOOST_REQUIRE(T.Add(1, TestNonPOD(&bDeleted)));
		BOOST_REQUIRE(T.Remove(1));
		BOOST_REQUIRE(bDeleted);
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

	BOOST_AUTO_TEST_SUITE_END() // ExtraOps

	BOOST_AUTO_TEST_SUITE_END() // TRBTreeTestSuite
	BOOST_AUTO_TEST_SUITE_END() // Container
	BOOST_AUTO_TEST_SUITE_END() // Core
} // anonymous