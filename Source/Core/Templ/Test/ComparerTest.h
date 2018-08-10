#include <boost/test/included/unit_test.hpp>

#include "../TComparer.h"
#include <string>

namespace
{

BOOST_AUTO_TEST_SUITE(Core)
BOOST_AUTO_TEST_SUITE(Templ)
BOOST_AUTO_TEST_SUITE(TComparerSuite)
BOOST_AUTO_TEST_SUITE(DefaultComparer)

BOOST_AUTO_TEST_CASE(MainTest)
{
	TComparer<int, int> const DefaultComparer;

	bool const bOneLessTwo = CompareLess(1, 2, DefaultComparer);
	BOOST_REQUIRE(bOneLessTwo);

	bool const bTwoGreaterOne = CompareGreater(2, 1, DefaultComparer);
	BOOST_REQUIRE(bTwoGreaterOne);

	bool const bOneLessOne = CompareLess(1, 1, DefaultComparer);
	BOOST_REQUIRE(!bOneLessOne);

	bool const bOneGreaterOne = CompareGreater(1, 1, DefaultComparer);
	BOOST_REQUIRE(!bOneGreaterOne);

	bool const bOneEqualOne = CompareEqual(1, 1, DefaultComparer);
	BOOST_REQUIRE(bOneEqualOne);

	bool const bOneGreaterEqualOne = CompareGreaterOrEqual(1, 1, DefaultComparer);
	BOOST_REQUIRE(bOneGreaterEqualOne);

	bool const bOneLessEqualOne = CompareLessOrEqual(1, 1, DefaultComparer);
	BOOST_REQUIRE(bOneLessEqualOne);

	bool const bOneEqualTwo = CompareEqual(1, 2, DefaultComparer);
	BOOST_REQUIRE(!bOneEqualTwo);

	bool const bOneLessEqualTwo = CompareLessOrEqual(1, 2, DefaultComparer);
	BOOST_REQUIRE(bOneLessEqualTwo);

	bool const bOneGreaterEqualTwo = CompareLessOrEqual(1, 2, DefaultComparer);
	BOOST_REQUIRE(!bOneGreaterEqualTwo);

	bool const bTwoEqualOne = CompareEqual(1, 2, DefaultComparer);
	BOOST_REQUIRE(!bTwoEqualOne);

	bool const bTwoLessEqualOne = CompareLessOrEqual(2, 1, DefaultComparer);
	BOOST_REQUIRE(bTwoLessEqualOne);

	bool const bTwoGreaterEqualOne = CompareLessOrEqual(2, 1, DefaultComparer);
	BOOST_REQUIRE(!bTwoGreaterEqualOne);
}

BOOST_AUTO_TEST_CASE
(
	DifferentTypesTest,
	*boost::unit_test::depends_on("Core/Templ/TComparerSuite/DefaultComparer/MainTest")
)
{
	TComparer<int, unsigned int> const DefaultComparer;

	bool const bOneLessTwo = CompareLess(1, static_cast<unsigned int>(2), DefaultComparer);
	BOOST_REQUIRE(bOneLessTwo);

	bool const bTwoGreaterOne = CompareGreater(2, static_cast<unsigned int>(1), DefaultComparer);
	BOOST_REQUIRE(bTwoGreaterOne);

	bool const bOneLessOne = CompareLess(1, static_cast<unsigned int>(1), DefaultComparer);
	BOOST_REQUIRE(!bOneLessOne);

	bool const bOneGreaterOne = CompareGreater(1, static_cast<unsigned int>(1), DefaultComparer);
	BOOST_REQUIRE(!bOneGreaterOne);

	bool const bOneEqualOne = CompareEqual(1, static_cast<unsigned>(1), DefaultComparer);
	BOOST_REQUIRE(bOneEqualOne);

	bool const bOneEqualTwo = CompareEqual(1, static_cast<unsigned>(2), DefaultComparer);
	BOOST_REQUIRE(!bOneEqualTwo);

	bool const bTwoEqualOne = CompareEqual(1, static_cast<unsigned>(2), DefaultComparer);
	BOOST_REQUIRE(!bTwoEqualOne);
}

struct TestObjectForComparer
{
	explicit TestObjectForComparer(int InKey) : Key(InKey) {}

	int Key;
};

template<>
struct TComparer<TestObjectForComparer, TestObjectForComparer>
{
	int Compare(const TestObjectForComparer& A, const TestObjectForComparer& B) const
	{
		return DefaultCompare(A.Key, B.Key);
	}
};

BOOST_AUTO_TEST_CASE
(
	ComparerForCustomType,
	*boost::unit_test::depends_on("Core/Templ/TComparerSuite/DefaultComparer/DifferentTypesTest")
)
{
	TComparer<TestObjectForComparer, TestObjectForComparer> const DefaultComparer;
	
	bool const bOneLessTwo = CompareLess(TestObjectForComparer{ 1 }, TestObjectForComparer{ 2 }, DefaultComparer);
	BOOST_REQUIRE(bOneLessTwo);

	bool const bTwoGreaterOne = CompareGreater(TestObjectForComparer{ 2 }, TestObjectForComparer{ 1 }, DefaultComparer);
	BOOST_REQUIRE(bTwoGreaterOne);

	bool const bOneLessOne = CompareLess(TestObjectForComparer{ 1 }, TestObjectForComparer{ 1 }, DefaultComparer);
	BOOST_REQUIRE(!bOneLessOne);

	bool const bOneGreaterOne = CompareGreater(TestObjectForComparer{ 1 }, TestObjectForComparer{ 1 }, DefaultComparer);
	BOOST_REQUIRE(!bOneGreaterOne);

	bool const bOneEqualOne = CompareEqual(TestObjectForComparer{ 1 }, TestObjectForComparer{ 1 }, DefaultComparer);
	BOOST_REQUIRE(bOneEqualOne);

	bool const bOneEqualTwo = CompareEqual(TestObjectForComparer{ 1 }, TestObjectForComparer{ 2 }, DefaultComparer);
	BOOST_REQUIRE(!bOneEqualTwo);

	bool const bTwoEqualOne = CompareEqual(TestObjectForComparer{ 1 }, TestObjectForComparer{ 2 }, DefaultComparer);
	BOOST_REQUIRE(!bTwoEqualOne);
}

BOOST_AUTO_TEST_SUITE_END() // DefaultComparer
BOOST_AUTO_TEST_SUITE_END() // TComparerSuite
BOOST_AUTO_TEST_SUITE_END() // Templ
BOOST_AUTO_TEST_SUITE_END() // Core

} // anonymous namespace