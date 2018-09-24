#pragma once

#include "Core/Cont/TRBTree.h"
#include <boost/test/included/unit_test.hpp>
#include <algorithm>
#include <string>
#include <memory>  // unique_ptr

namespace
{
	using MoveOnlyTree = TRBTree<KVType<int, std::unique_ptr<std::string>>>;

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


	struct TemplateCopyTestIntComparer
	{
		int Compare(int A, int B) const
		{
			return DefaultCompare(A, B);
		}
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
		return std::equal(A, A + InSize, B, B + InSize);
	}

	bool ArrayContainsValue(const TKeyValue<KVType<int, NoValue>>* Ptr, int InSize, const int& InKey)
	{
		const TKeyValue<KVType<int, NoValue>>* END_IT = Ptr + InSize;

		BOOST_ASSERT(Ptr);
		BOOST_ASSERT(InSize >= 0);
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
} // anonymous