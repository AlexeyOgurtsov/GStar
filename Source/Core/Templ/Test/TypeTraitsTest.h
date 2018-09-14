#pragma once

#include "../TypeTraits.h"
#include <boost/test/included/unit_test.hpp>
#include <vector>
#include <memory>

BOOST_AUTO_TEST_SUITE(Core)
BOOST_AUTO_TEST_SUITE(Templ)
BOOST_AUTO_TEST_SUITE(TypeTraitsSuite)

namespace
{
	struct Test_NonDefaultConstructible
	{
		Test_NonDefaultConstructible() = delete;
	};
} // anonymous namespace

BOOST_AUTO_TEST_CASE(IsConstTypeTest)
{
	static_assert( IsConstType<const int>::Value );
	static_assert( ! IsConstType<int>::Value );
	static_assert( ! IsConstType<const int *>::Value );
	static_assert( ! IsConstType<int *>::Value );
	static_assert( IsConstType<const int&>::Value);
	static_assert( ! IsConstType<int&>::Value);
}

BOOST_AUTO_TEST_CASE(ExactDefaultConstructibleTest)
{
	static_assert(ExactDefaultConstructible<std::vector<int>>::Value );
	static_assert(ExactDefaultConstructible<int>::Value);

	static_assert( ! ExactDefaultConstructible<Test_NonDefaultConstructible>::Value );
}

BOOST_AUTO_TEST_CASE(ExactTrivialDefaultConstructibleTest)
{
	static_assert( ! ExactTrivialDefaultConstructible<std::vector<int>>::Value );
	static_assert(ExactTrivialDefaultConstructible<int>::Value);

	static_assert( ! ExactTrivialDefaultConstructible<Test_NonDefaultConstructible>::Value );
}

BOOST_AUTO_TEST_CASE(ExactTrivialDestructibleTest)
{
	static_assert( ! ExactTrivialDestructible<std::vector<int>>::Value );
	static_assert(ExactTrivialDestructible<int>::Value);

	static_assert(ExactTrivialDestructible<Test_NonDefaultConstructible>::Value);
}

BOOST_AUTO_TEST_CASE(ExactCopyConstructibleTest)
{
	static_assert(ExactCopyConstructible<int>::Value);
	static_assert(ExactCopyConstructible<std::vector<int>>::Value);
	static_assert( ! ExactCopyConstructible<std::unique_ptr<int*>>::Value);
}

BOOST_AUTO_TEST_CASE(ExactTrivialCopyConstructibleTest)
{
	static_assert(ExactTrivialCopyConstructible<int>::Value);
	static_assert( ! ExactTrivialCopyConstructible<std::vector<int>>::Value);
	static_assert( ! ExactTrivialCopyConstructible<std::unique_ptr<int*>>::Value);
}

BOOST_AUTO_TEST_CASE(ExactCopyAssignableTest)
{
	static_assert(ExactCopyAssignable<int>::Value);
	static_assert(ExactCopyAssignable<std::vector<int>>::Value);
	static_assert(!ExactCopyAssignable<std::unique_ptr<int*>>::Value);
}

BOOST_AUTO_TEST_CASE(ExactTrivialCopyAssignableTest)
{
	static_assert(ExactTrivialCopyAssignable<int>::Value);
	static_assert(!ExactTrivialCopyAssignable<std::vector<int>>::Value);
	static_assert(!ExactTrivialCopyAssignable<std::unique_ptr<int*>>::Value);
}

BOOST_AUTO_TEST_CASE(ExactTypeEqualTest)
{
	static_assert(ExactTypeEqual<int, int>::Value);
	static_assert(ExactTypeEqual<const int, const int>::Value);
	static_assert( ! ExactTypeEqual<int, const int>::Value );
	static_assert( ! ExactTypeEqual<const int, int>::Value );
	static_assert( ! ExactTypeEqual<int&, int>::Value);
}

BOOST_AUTO_TEST_CASE(TypeEqualWithConstTest)
{
	static_assert(TypeEqualWithConst<int, int>::Value);
	static_assert(TypeEqualWithConst<const int, const int>::Value);
	static_assert( ! TypeEqualWithConst<int, const int>::Value);
	static_assert(TypeEqualWithConst<const int, const int&>::Value);
	static_assert( ! TypeEqualWithConst<const int, int>::Value);
	static_assert(TypeEqualWithConst<int&, int>::Value);
	static_assert( ! TypeEqualWithConst<int&, const int>::Value);
	static_assert(TypeEqualWithConst<const int&, const int>::Value);
	static_assert(!TypeEqualWithConst<int, int*>::Value);
	static_assert(!TypeEqualWithConst<const int*, int*>::Value);
}

BOOST_AUTO_TEST_CASE(TypeEqualTest)
{
	static_assert(TypeEqual<int, int>::Value);
	static_assert(TypeEqual<const int, const int>::Value);
	static_assert(TypeEqual<int, const int>::Value);
	static_assert(TypeEqual<const int, int>::Value);
	static_assert(TypeEqual<int&, int>::Value);
	static_assert(TypeEqual<int&, const int>::Value);
	static_assert(TypeEqual<const int&, const int>::Value);
	static_assert( ! TypeEqual<int, int*>::Value );
	static_assert( ! TypeEqual<const int*, int*>::Value );
}

BOOST_AUTO_TEST_SUITE_END() // TypeTraitsSuite
BOOST_AUTO_TEST_SUITE_END() // Templ
BOOST_AUTO_TEST_SUITE_END() // Core