#pragma once

#include "TVectorTestUtils.h"

namespace
{

BOOST_AUTO_TEST_SUITE(Core)
BOOST_AUTO_TEST_SUITE(Container)
BOOST_AUTO_TEST_SUITE(TVectorTestSuite)

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


BOOST_AUTO_TEST_SUITE_END() // TVectorTestSuite
BOOST_AUTO_TEST_SUITE_END() // Container
BOOST_AUTO_TEST_SUITE_END() // Core

} 