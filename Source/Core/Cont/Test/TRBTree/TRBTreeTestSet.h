#pragma once

#include "TRBTreeTestUtils.h"

namespace
{
	BOOST_AUTO_TEST_SUITE(Core)
	BOOST_AUTO_TEST_SUITE(Container)
	BOOST_AUTO_TEST_SUITE(TRBTreeTestSuite)
	
	// WARNING!!! This line causes compilation failure if you uncomment it!
	BOOST_AUTO_TEST_SUITE
	(
		SetOperationTestSuite,
		*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal")
	)

	BOOST_AUTO_TEST_CASE(AppendIntersectTestCase)
	{
		using TreeType = TRBTree<KVType<int, int>>;

		TreeType const EmptyA, EmptyB;
		TreeType EmptyIntersect;
		BOOST_REQUIRE(EmptyIntersect.Add(1, 1));
		AppendIntersectTo(EmptyIntersect, EmptyA, EmptyB);
		BOOST_REQUIRE_EQUAL(EmptyIntersect.Num(), 1);
		BOOST_REQUIRE(EmptyIntersect.Contains(1));

		TreeType OneOnly;
		BOOST_REQUIRE(OneOnly.Add(1,1));
		TreeType OneIntersectEmpty;
		BOOST_REQUIRE(OneIntersectEmpty.Add(2, 2));
		AppendIntersectTo(OneIntersectEmpty, OneOnly, EmptyB);
		BOOST_REQUIRE_EQUAL(OneIntersectEmpty.Num(), 1);
		BOOST_REQUIRE(OneIntersectEmpty.Contains(1));

		TreeType EmptyIntersectOne;
		BOOST_REQUIRE(EmptyIntersectOne.Add(2, 2));
		AppendIntersectTo(EmptyIntersectOne, EmptyB, OneOnly);
		BOOST_REQUIRE_EQUAL(EmptyIntersectOne.Num(), 1);
		BOOST_REQUIRE(EmptyIntersectOne.Contains(2));

		TreeType OneIntersectOne;
		BOOST_REQUIRE(OneIntersectOne.Add(3,3));
		AppendIntersectTo(OneIntersectOne, OneOnly, OneOnly);
		BOOST_REQUIRE_EQUAL(OneIntersectOne.Num(), 2);
		BOOST_REQUIRE(OneIntersectOne.Contains(1));
		BOOST_REQUIRE(OneIntersectOne.Contains(3));

		TreeType Tree_Zero_One;
		BOOST_REQUIRE(Tree_Zero_One.Add(0, 0));
		BOOST_REQUIRE(Tree_Zero_One.Add(1, 1));
		TreeType One_Intersect_ZeroOne;
		AppendIntersectTo(One_Intersect_ZeroOne, OneOnly, Tree_Zero_One);
		BOOST_REQUIRE_EQUAL(One_Intersect_ZeroOne.Num(), 1);
		BOOST_REQUIRE(One_Intersect_ZeroOne.Contains(1));

		TreeType Tree_Zero_Two;
		BOOST_REQUIRE(Tree_Zero_Two.Add(0, 0));
		BOOST_REQUIRE(Tree_Zero_Two.Add(2, 2));
		TreeType One_Intersect_ZeroTwo;
		BOOST_REQUIRE(One_Intersect_ZeroTwo.Add(4, 4));
		AppendIntersectTo(One_Intersect_ZeroTwo, OneOnly, Tree_Zero_Two);
		BOOST_REQUIRE_EQUAL(One_Intersect_ZeroTwo.Num(), 0);

		TreeType Tree_One_Two;
		BOOST_REQUIRE(Tree_One_Two.Add(1, 1));
		BOOST_REQUIRE(Tree_One_Two.Add(2, 2));
		BOOST_REQUIRE_EQUAL(Tree_One_Two.Num(), 2);

		TreeType Tree_0_12_34;
		BOOST_REQUIRE(Tree_0_12_34.Add(0, 0));
		BOOST_REQUIRE(Tree_0_12_34.Add(1, 1));
		BOOST_REQUIRE(Tree_0_12_34.Add(2, 2));
		BOOST_REQUIRE(Tree_0_12_34.Add(3, 3));
		BOOST_REQUIRE(Tree_0_12_34.Add(4, 4));
		BOOST_REQUIRE_EQUAL(Tree_0_12_34.Num(), 5);

		TreeType Result_12;
		AppendIntersectTo(Result_12, Tree_One_Two, Tree_0_12_34);
		BOOST_REQUIRE(Result_12.Contains(1));
		BOOST_REQUIRE(Result_12.Contains(2));
		BOOST_REQUIRE_EQUAL(Result_12.Num(), 2);

		TreeType Tree_123_57_9;
		BOOST_REQUIRE(Tree_123_57_9.Add(1, 1));
		BOOST_REQUIRE(Tree_123_57_9.Add(2, 2));
		BOOST_REQUIRE(Tree_123_57_9.Add(3, 3));
		BOOST_REQUIRE(Tree_123_57_9.Add(5, 5));
		BOOST_REQUIRE(Tree_123_57_9.Add(7, 7));
		BOOST_REQUIRE(Tree_123_57_9.Add(9, 9));
		BOOST_REQUIRE_EQUAL(Tree_123_57_9.Num(), 6);

		TreeType Tree_13_4_7_A_B;
		BOOST_REQUIRE(Tree_13_4_7_A_B.Add(1, 1));
		BOOST_REQUIRE(Tree_13_4_7_A_B.Add(3, 3));
		BOOST_REQUIRE(Tree_13_4_7_A_B.Add(4, 4));
		BOOST_REQUIRE(Tree_13_4_7_A_B.Add(7, 7));
		BOOST_REQUIRE(Tree_13_4_7_A_B.Add(10, 10));
		BOOST_REQUIRE(Tree_13_4_7_A_B.Add(11, 11));
		BOOST_REQUIRE_EQUAL(Tree_13_4_7_A_B.Num(), 6);

		TreeType Result_137;
		AppendIntersectTo(Result_137, Tree_123_57_9, Tree_13_4_7_A_B);
		BOOST_REQUIRE_EQUAL(Result_137.Num(), 3);
		BOOST_REQUIRE(Result_137.Contains(1));
		BOOST_REQUIRE(Result_137.Contains(3));
		BOOST_REQUIRE(Result_137.Contains(7));
	}

	BOOST_AUTO_TEST_CASE(AppendDifferenceTestCase)
	{
		using TreeType = TRBTree<KVType<int, int>>;

		TreeType const EmptyA, EmptyB;
		TreeType EmptyDiff;
		BOOST_REQUIRE(EmptyDiff.Add(1, 1));
		AppendDifferenceTo(EmptyDiff, EmptyA, EmptyB);
		BOOST_REQUIRE_EQUAL(EmptyDiff.Num(), 1);
		BOOST_REQUIRE(EmptyDiff.Contains(1));

		TreeType OneOnly;
		BOOST_REQUIRE(OneOnly.Add(1,1));
		TreeType OneDiffEmpty;
		BOOST_REQUIRE(OneDiffEmpty.Add(2, 2));
		AppendDifferenceTo(OneDiffEmpty, OneOnly, EmptyB);
		BOOST_REQUIRE_EQUAL(OneDiffEmpty.Num(), 2);
		BOOST_REQUIRE(OneDiffEmpty.Contains(1));
		BOOST_REQUIRE(OneDiffEmpty.Contains(2));

		TreeType EmptyDiffOne;
		BOOST_REQUIRE(EmptyDiffOne.Add(2, 2));
		AppendDifferenceTo(EmptyDiffOne, EmptyB, OneOnly);
		BOOST_REQUIRE_EQUAL(EmptyDiffOne.Num(), 1);
		BOOST_REQUIRE(EmptyDiffOne.Contains(2));

		TreeType OneDiffOne;
		BOOST_REQUIRE(OneDiffOne.Add(3,3));
		AppendDifferenceTo(OneDiffOne, OneOnly, OneOnly);
		BOOST_REQUIRE_EQUAL(OneDiffOne.Num(), 2);
		BOOST_REQUIRE(OneDiffOne.Contains(1));
		BOOST_REQUIRE(OneDiffOne.Contains(3));

		TreeType Tree_Zero_One;
		BOOST_REQUIRE(Tree_Zero_One.Add(0, 0));
		BOOST_REQUIRE(Tree_Zero_One.Add(1, 1));
		TreeType OneDiffZeroOne;
		AppendDifferenceTo(OneDiffZeroOne, OneOnly, Tree_Zero_One);
		BOOST_REQUIRE_EQUAL(OneDiffZeroOne.Num(), 1);
		BOOST_REQUIRE(OneDiffZeroOne.Contains(0));

		TreeType Tree_Zero_Two;
		BOOST_REQUIRE(Tree_Zero_Two.Add(0, 0));
		BOOST_REQUIRE(Tree_Zero_Two.Add(2, 2));
		TreeType OneDiff_ZeroTwo;
		BOOST_REQUIRE(OneDiff_ZeroTwo.Add(4, 4));
		AppendDifferenceTo(OneDiff_ZeroTwo, OneOnly, Tree_Zero_Two);
		BOOST_REQUIRE_EQUAL(OneDiff_ZeroTwo.Num(), 3);
		BOOST_REQUIRE(OneDiff_ZeroTwo.Contains(0));
		BOOST_REQUIRE(OneDiff_ZeroTwo.Contains(2));
		BOOST_REQUIRE(OneDiff_ZeroTwo.Contains(4));

		TreeType Tree_One_Two;
		BOOST_REQUIRE(Tree_One_Two.Add(1, 1));
		BOOST_REQUIRE(Tree_One_Two.Add(2, 2));
		BOOST_REQUIRE_EQUAL(Tree_One_Two.Num(), 2);

		TreeType Tree_0_12_34;
		BOOST_REQUIRE(Tree_0_12_34.Add(0, 0));
		BOOST_REQUIRE(Tree_0_12_34.Add(1, 1));
		BOOST_REQUIRE(Tree_0_12_34.Add(2, 2));
		BOOST_REQUIRE(Tree_0_12_34.Add(3, 3));
		BOOST_REQUIRE(Tree_0_12_34.Add(4, 4));
		BOOST_REQUIRE_EQUAL(Tree_0_12_34.Num(), 5);

		TreeType Result_0_3_4;
		AppendDifferenceTo(Result_0_3_4, Tree_One_Two, Tree_0_12_34);
		BOOST_REQUIRE(Result_0_3_4.Contains(0));
		BOOST_REQUIRE(Result_0_3_4.Contains(3));
		BOOST_REQUIRE(Result_0_3_4.Contains(4));
		BOOST_REQUIRE_EQUAL(Result_0_3_4.Num(), 3);

		TreeType Tree_123_57_9;
		BOOST_REQUIRE(Tree_123_57_9.Add(1,1));
		BOOST_REQUIRE(Tree_123_57_9.Add(2,2));
		BOOST_REQUIRE(Tree_123_57_9.Add(3,3));
		BOOST_REQUIRE(Tree_123_57_9.Add(5,5));
		BOOST_REQUIRE(Tree_123_57_9.Add(7,7));
		BOOST_REQUIRE(Tree_123_57_9.Add(9,9));
		BOOST_REQUIRE_EQUAL(Tree_123_57_9.Num(), 6);

		TreeType Tree_13_4_7_A_B;
		BOOST_REQUIRE(Tree_13_4_7_A_B.Add(1, 1));
		BOOST_REQUIRE(Tree_13_4_7_A_B.Add(3, 3));
		BOOST_REQUIRE(Tree_13_4_7_A_B.Add(4, 4));
		BOOST_REQUIRE(Tree_13_4_7_A_B.Add(7, 7));
		BOOST_REQUIRE(Tree_13_4_7_A_B.Add(10, 10));
		BOOST_REQUIRE(Tree_13_4_7_A_B.Add(11, 11));
		BOOST_REQUIRE_EQUAL(Tree_13_4_7_A_B.Num(), 6);

		TreeType Tree_2_5_result;
		AppendDifferenceTo(Tree_2_5_result, Tree_123_57_9, Tree_13_4_7_A_B);
		BOOST_REQUIRE_EQUAL(Tree_2_5_result.Num(), 2);
		BOOST_REQUIRE(Tree_2_5_result.Contains(2));
		BOOST_REQUIRE(Tree_2_5_result.Contains(5));
	}

	BOOST_AUTO_TEST_CASE(AppendUnionToTest)
	{
		using TreeType = TRBTree<KVType<int, int>>;

		TreeType const EmptyA, EmptyB;
		TreeType EmptyUnion;
		BOOST_REQUIRE(EmptyUnion.Add(1, 1));
		AppendUnionTo(EmptyUnion, EmptyA, EmptyB);
		BOOST_REQUIRE_EQUAL(EmptyUnion.Num(), 1);
		BOOST_REQUIRE(EmptyUnion.Contains(1));

		TreeType OneOnly;
		BOOST_REQUIRE(OneOnly.Add(1,1));
		TreeType OneUnionEmpty;
		BOOST_REQUIRE(OneUnionEmpty.Add(2, 2));
		AppendUnionTo(OneUnionEmpty, OneOnly, EmptyB);
		BOOST_REQUIRE_EQUAL(OneUnionEmpty.Num(), 2);
		BOOST_REQUIRE(OneUnionEmpty.Contains(1));
		BOOST_REQUIRE(OneUnionEmpty.Contains(2));

		TreeType EmptyUnionOne;
		BOOST_REQUIRE(EmptyUnionOne.Add(2, 2));
		AppendUnionTo(EmptyUnionOne, EmptyB, OneOnly);
		BOOST_REQUIRE_EQUAL(EmptyUnionOne.Num(), 1);
		BOOST_REQUIRE(EmptyUnionOne.Contains(2));

		TreeType OneUnionOne;
		BOOST_REQUIRE(OneUnionOne.Add(3,3));
		AppendUnionTo(OneUnionOne, OneOnly, OneOnly);
		BOOST_REQUIRE_EQUAL(OneUnionOne.Num(), 2);
		BOOST_REQUIRE(OneUnionOne.Contains(1));
		BOOST_REQUIRE(OneUnionOne.Contains(3));

		TreeType Tree_Zero_One;
		BOOST_REQUIRE(Tree_Zero_One.Add(0, 0));
		BOOST_REQUIRE(Tree_Zero_One.Add(1, 1));
		BOOST_REQUIRE_EQUAL(Tree_Zero_One.Num(), 2);
		TreeType OneUnionZeroOne;
		AppendUnionTo(OneUnionZeroOne, OneOnly, Tree_Zero_One);
		BOOST_REQUIRE_EQUAL(OneUnionZeroOne.Num(), 2);
		BOOST_REQUIRE(OneUnionZeroOne.Contains(0));
		BOOST_REQUIRE(OneUnionZeroOne.Contains(1));

		TreeType Tree_Zero_Two;
		BOOST_REQUIRE(Tree_Zero_Two.Add(0, 0));
		BOOST_REQUIRE(Tree_Zero_Two.Add(2, 2));
		TreeType One_Union_ZeroTwo;
		BOOST_REQUIRE(One_Union_ZeroTwo.Add(4, 4));
		AppendUnionTo(One_Union_ZeroTwo, OneOnly, Tree_Zero_Two);
		BOOST_REQUIRE_EQUAL(One_Union_ZeroTwo.Num(), 4);
		BOOST_REQUIRE(One_Union_ZeroTwo.Contains(0));
		BOOST_REQUIRE(One_Union_ZeroTwo.Contains(1));
		BOOST_REQUIRE(One_Union_ZeroTwo.Contains(2));
		BOOST_REQUIRE(One_Union_ZeroTwo.Contains(4));

		TreeType Tree_One_Two;
		BOOST_REQUIRE(Tree_One_Two.Add(1, 1));
		BOOST_REQUIRE(Tree_One_Two.Add(2, 2));
		BOOST_REQUIRE_EQUAL(Tree_One_Two.Num(), 2);

		TreeType Tree_0_12_34;
		BOOST_REQUIRE(Tree_0_12_34.Add(0, 0));
		BOOST_REQUIRE(Tree_0_12_34.Add(1, 1));
		BOOST_REQUIRE(Tree_0_12_34.Add(2, 2));
		BOOST_REQUIRE(Tree_0_12_34.Add(3, 3));
		BOOST_REQUIRE(Tree_0_12_34.Add(4, 4));
		BOOST_REQUIRE_EQUAL(Tree_0_12_34.Num(), 5);

		TreeType Result_01234;
		AppendUnionTo(Result_01234, Tree_One_Two, Tree_0_12_34);
		BOOST_REQUIRE(Result_01234.Contains(0));
		BOOST_REQUIRE(Result_01234.Contains(1));
		BOOST_REQUIRE(Result_01234.Contains(2));
		BOOST_REQUIRE(Result_01234.Contains(3));
		BOOST_REQUIRE(Result_01234.Contains(4));
		BOOST_REQUIRE_EQUAL(Result_01234.Num(), 5);

		TreeType Tree_123_57_9;
		BOOST_REQUIRE(Tree_123_57_9.Add(1,1));
		BOOST_REQUIRE(Tree_123_57_9.Add(2,2));
		BOOST_REQUIRE(Tree_123_57_9.Add(3,3));
		BOOST_REQUIRE(Tree_123_57_9.Add(5,5));
		BOOST_REQUIRE(Tree_123_57_9.Add(7,7));
		BOOST_REQUIRE(Tree_123_57_9.Add(9,9));
		BOOST_REQUIRE_EQUAL(Tree_123_57_9.Num(), 6);

		TreeType Tree_13_4_7_A_B;
		BOOST_REQUIRE(Tree_13_4_7_A_B.Add(1, 1));
		BOOST_REQUIRE(Tree_13_4_7_A_B.Add(3, 3));
		BOOST_REQUIRE(Tree_13_4_7_A_B.Add(4, 4));
		BOOST_REQUIRE(Tree_13_4_7_A_B.Add(7, 7));
		BOOST_REQUIRE(Tree_13_4_7_A_B.Add(10, 10));
		BOOST_REQUIRE(Tree_13_4_7_A_B.Add(11, 11));
		BOOST_REQUIRE_EQUAL(Tree_13_4_7_A_B.Num(), 6);

		TreeType Result_1234579AB;
		AppendUnionTo(Result_1234579AB, Tree_123_57_9, Tree_13_4_7_A_B);
		BOOST_REQUIRE_EQUAL(Result_1234579AB.Num(), 9);
		BOOST_REQUIRE(Result_1234579AB.Contains(1));
		BOOST_REQUIRE(Result_1234579AB.Contains(2));
		BOOST_REQUIRE(Result_1234579AB.Contains(3));
		BOOST_REQUIRE(Result_1234579AB.Contains(4));
		BOOST_REQUIRE(Result_1234579AB.Contains(5));
		BOOST_REQUIRE(Result_1234579AB.Contains(7));
		BOOST_REQUIRE(Result_1234579AB.Contains(9));
		BOOST_REQUIRE(Result_1234579AB.Contains(10));
		BOOST_REQUIRE(Result_1234579AB.Contains(11));
	}

	BOOST_AUTO_TEST_SUITE_END() // SetOperationTestSuite
	BOOST_AUTO_TEST_SUITE_END() // TRBTreeTestSuite
	BOOST_AUTO_TEST_SUITE_END() // Container
	BOOST_AUTO_TEST_SUITE_END() // Core
} // anonymous