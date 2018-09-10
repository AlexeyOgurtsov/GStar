#pragma once

#include "../Pointer.h"
#include "Core/Mem/TUniquePtr.h"
#include "Core/Mem/TSharedPtr.h"
#include "Core/Mem/TComPtr.h"
#include <string>
#include <unordered_set>
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(Core)
BOOST_AUTO_TEST_SUITE(Templ)
BOOST_AUTO_TEST_SUITE(PointerSuite)
BOOST_AUTO_TEST_SUITE(TraitsSuite)

BOOST_AUTO_TEST_CASE(ComPtrTest)
{
	using TestComPtr = TComPtr<IUnknown>;
	using TestComPtrToConst = TComPtr<const IUnknown>;

	static_assert( IsSmartPointer<TestComPtr>::Value );
	static_assert( ! IsSmartPointerToConst<TestComPtr>::Value);
	static_assert( IsSmartPointer<TestComPtrToConst>::Value );
	static_assert( IsSmartPointerToConst<TestComPtrToConst>::Value );
	static_assert(IsStrongSmartPointerToMutable<TestComPtr>::Value);
	static_assert( IsStrongSmartPointerToConst<TestComPtrToConst>::Value );

	BOOST_TEST_CHECKPOINT("DefaultConstruction");
	TestComPtr DefaultCom;
	TestComPtrToConst DefaultComToConst;
	TestComPtr CopiedDefaultCom { DefaultCom };
	TestComPtrToConst CopiedDefaultComToConst { DefaultComToConst };

	BOOST_REQUIRE(DefaultCom.IsNull());
	BOOST_REQUIRE(DefaultComToConst.IsNull());

	BOOST_REQUIRE( ! DefaultCom );
	BOOST_REQUIRE( ! DefaultComToConst );

	BOOST_REQUIRE( DefaultCom == DefaultComToConst );
	BOOST_REQUIRE( DefaultCom <= DefaultComToConst );
	BOOST_REQUIRE( DefaultCom >= DefaultComToConst );
	BOOST_REQUIRE( ! ( DefaultCom < DefaultComToConst ) );
	BOOST_REQUIRE( ! ( DefaultCom > DefaultComToConst ) );

	BOOST_TEST_CHECKPOINT("ResetForDefault test");
	DefaultCom.Reset(nullptr);
	DefaultComToConst.Reset(nullptr);
	BOOST_REQUIRE(DefaultCom.IsNull());
	BOOST_REQUIRE(DefaultComToConst.IsNull());

	BOOST_TEST_CHECKPOINT("Copying NonConst to const");
	DefaultComToConst = DefaultCom;
	BOOST_REQUIRE(DefaultComToConst.IsNull());

	BOOST_TEST_CHECKPOINT("Moving NonConst to const");
	DefaultComToConst = std::move(DefaultCom);
	BOOST_REQUIRE(DefaultComToConst.IsNull());

	BOOST_TEST_CHECKPOINT("Hash");
	BOOST_REQUIRE(DefaultCom == DefaultComToConst);
	std::unordered_set<TestComPtrToConst> TS;
	TS.insert(DefaultCom);
	TS.insert(DefaultComToConst);
	BOOST_REQUIRE_EQUAL(TS.size(), 1);
}

BOOST_AUTO_TEST_CASE(PointerTest)
{
	static_assert( ! IsCPointer<int>::Value );
	static_assert( ! IsSmartPointer<int>::Value );
	static_assert( ! IsStrongSmartPointer<int>::Value );
	static_assert( ! IsAnyPointer<int>::Value );
	static_assert( ! IsAnyStrongPointer<int>::Value );
	static_assert( ! IsSmartPointerToConst<int>::Value);
	static_assert( ! IsSmartPointerToMutable<int>::Value );
	static_assert( ! IsStrongSmartPointerToMutable<int>::Value );

	static_assert( IsCPointer<int*>::Value );
	static_assert( IsCPointer<const int*>::Value );
	static_assert( IsAnyPointer<const int*>::Value );
	static_assert( IsAnyPointer<int*>::Value );
	static_assert( ! IsSmartPointer<const int*>::Value );
	static_assert( ! IsSmartPointer<int*>::Value );
	static_assert(!IsSmartPointerToConst<const int *>::Value);
	static_assert(!IsSmartPointerToMutable<const int*>::Value);
	static_assert(!IsStrongSmartPointerToMutable<const int*>::Value);

	static_assert( ! IsUniquePointer<int*>::Value );
	static_assert( ! IsUniquePointer<const int*>::Value);
	static_assert(IsAnyStrongPointer<const int*>::Value);

	using TestUniquePtr = TUniquePtr<int>;
	using TestUniquePtrToConst = TUniquePtr<const int>;

	static_assert( IsSmartPointer<TestUniquePtr>::Value );
	static_assert( IsStrongSmartPointer<TestUniquePtr>::Value );
	static_assert(IsSmartPointer<TestUniquePtrToConst>::Value);

	static_assert(IsStrongSmartPointer<TestUniquePtrToConst>::Value);
	static_assert(IsSmartPointerToConst<TestUniquePtrToConst>::Value);
	static_assert(!IsSmartPointerToMutable<TestUniquePtrToConst>::Value);
	static_assert(!IsStrongSmartPointerToMutable<TestUniquePtrToConst>::Value);

	static_assert( IsUniquePointer<TestUniquePtr>::Value );
	static_assert( ! IsSharedPointer<TestUniquePtr>::Value);
	static_assert(IsSmartPointerToMutable<TestUniquePtr>::Value);
	static_assert(IsStrongSmartPointerToMutable<TestUniquePtr>::Value);

	static_assert(IsAnyPointer<TestUniquePtr>::Value);

	static_assert( IsUniquePointer<TestUniquePtrToConst>::Value );
	static_assert( ! IsSharedPointer<TestUniquePtrToConst>::Value );

	static_assert(IsAnyPointer<TestUniquePtrToConst>::Value);
	static_assert(IsAnyStrongPointer<TestUniquePtrToConst>::Value);

	using TestSharedPtr = TSharedPtr<int>;
	using TestSharedPtrToConst = TSharedPtr<const int>;
	using TestWeakPtr = TWeakPtr<int>;
	using TestWeakPtrToConst = TWeakPtr<const int>;

	static_assert(IsSmartPointer<TestSharedPtr>::Value);
	static_assert(IsStrongSmartPointer<TestSharedPtr>::Value);
	static_assert(IsSharedPointer<TestSharedPtr>::Value);
	static_assert(IsAnyPointer<TestSharedPtr>::Value);
	
	static_assert(IsSmartPointer<TestSharedPtrToConst>::Value);
	static_assert(IsStrongSmartPointer<TestSharedPtrToConst>::Value);
	static_assert(IsSmartPointerToConst<TestSharedPtrToConst>::Value);
	static_assert(IsStrongSmartPointerToConst<TestSharedPtrToConst>::Value);
	static_assert(IsAnyPointer<TestSharedPtrToConst>::Value);
	static_assert(IsAnyStrongPointer<TestSharedPtrToConst>::Value);

	static_assert(IsSmartPointer<TestWeakPtr>::Value );
	static_assert( ! IsStrongSmartPointer<TestWeakPtr>::Value );
	static_assert( ! IsStrongSmartPointerToConst<TestWeakPtr>::Value);
	static_assert( IsWeakPointer<TestWeakPtr>::Value );
	static_assert(IsAnyPointer<TestWeakPtr>::Value);

	static_assert(IsSmartPointer<TestWeakPtrToConst>::Value);
	static_assert( ! IsStrongSmartPointer<TestWeakPtrToConst>::Value );
	static_assert( ! IsStrongSmartPointerToConst<TestWeakPtrToConst>::Value );
	static_assert(IsAnyPointer<TestWeakPtrToConst>::Value);
	static_assert( ! IsCPointer<TestWeakPtrToConst>::Value);
	static_assert( ! IsAnyStrongPointer<TestWeakPtrToConst>::Value );

	static_assert( ! IsSmartPointerToMutable<TestWeakPtrToConst>::Value);
	static_assert( ! IsStrongSmartPointerToMutable<TestWeakPtrToConst>::Value);
}

BOOST_AUTO_TEST_SUITE_END() // TraitsSuite
BOOST_AUTO_TEST_SUITE_END() // PointerSuite
BOOST_AUTO_TEST_SUITE_END() // Templ
BOOST_AUTO_TEST_SUITE_END() // Core