#pragma once

#include "TRBTreeTestUtils.h"

namespace
{
	BOOST_AUTO_TEST_SUITE(Core)
	BOOST_AUTO_TEST_SUITE(Container)
	BOOST_AUTO_TEST_SUITE(TRBTreeTestSuite)
	BOOST_AUTO_TEST_SUITE(Minimal)

		BOOST_AUTO_TEST_SUITE
		(
			CopyingSuite,
			*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/AddSuite")
			*boost::unit_test::depends_on("Core/Container/TRBTreeTestSuite/Minimal/RemoveSuite")
		)



		BOOST_AUTO_TEST_CASE(TemplateCopyConstruct)
	{
		const IntStringRBTree::KeyValueType KV_1{ 1, std::string("one") };
		const IntStringRBTree::KeyValueType KV_2{ 2, std::string("two") };
		const IntStringRBTree::KeyValueType KV_3{ 3, std::string("three") };

		IntStringRBTree Source;
		Source.Add(KV_1);
		Source.Add(KV_2);
		Source.Add(KV_3);

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
		BOOST_REQUIRE(Source.Add(KV_1));
		BOOST_REQUIRE(Source.Add(KV_2));
		BOOST_REQUIRE(Source.Add(KV_3));

		TRBTree<KVType<int, std::string>, TemplateCopyTestIntComparer> T;
		const IntStringRBTree::KeyValueType OTHER_KV_1{ 11, std::string("eleven") };
		const IntStringRBTree::KeyValueType OTHER_KV_2{ 12, std::string("twelve") };
		BOOST_REQUIRE(T.Add(OTHER_KV_1));
		BOOST_REQUIRE(T.Add(OTHER_KV_2));


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
		Source.Add(KV_1);
		Source.Add(KV_2);
		Source.Add(KV_3);

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
		BOOST_REQUIRE(Source.Add(KV_1));
		BOOST_REQUIRE(Source.Add(KV_2));
		BOOST_REQUIRE(Source.Add(KV_3));

		TRBTree<KVType<int, std::string>, TemplateCopyTestIntComparer> T;
		const IntStringRBTree::KeyValueType OTHER_KV_1{ 11, std::string("eleven") };
		const IntStringRBTree::KeyValueType OTHER_KV_2{ 12, std::string("twelve") };
		BOOST_REQUIRE(T.Add(OTHER_KV_1));
		BOOST_REQUIRE(T.Add(OTHER_KV_2));

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
		Source.Add(KV_1);
		Source.Add(KV_2);
		Source.Add(KV_3);

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
		BOOST_REQUIRE(Source.Add(KV_1));
		BOOST_REQUIRE(Source.Add(KV_2));
		BOOST_REQUIRE(Source.Add(KV_3));

		IntStringRBTree T;
		const IntStringRBTree::KeyValueType OTHER_KV_1{ 11, std::string("eleven") };
		const IntStringRBTree::KeyValueType OTHER_KV_2{ 12, std::string("twelve") };
		BOOST_REQUIRE(T.Add(OTHER_KV_1));
		BOOST_REQUIRE(T.Add(OTHER_KV_2));

		T = std::move(Source);
		BOOST_REQUIRE_EQUAL(T.Num(), 3);
		BOOST_REQUIRE(T.Contains(KV_1.Key));
		BOOST_REQUIRE(T.Contains(KV_2.Key));
		BOOST_REQUIRE(T.Contains(KV_3.Key));
		BOOST_REQUIRE(!T.Contains(OTHER_KV_1.Key));
		BOOST_REQUIRE(!T.Contains(OTHER_KV_2.Key));

		BOOST_REQUIRE(Source.Empty());
	}

	BOOST_AUTO_TEST_CASE(SimpleCopyConstruct)
	{
		const IntStringRBTree::KeyValueType KV_1{ 1, std::string("one") };
		const IntStringRBTree::KeyValueType KV_2{ 2, std::string("two") };
		const IntStringRBTree::KeyValueType KV_3{ 3, std::string("three") };

		IntStringRBTree Source;
		Source.Add(KV_1);
		Source.Add(KV_2);
		Source.Add(KV_3);

		IntStringRBTree T{ Source };
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
		BOOST_REQUIRE(Source.Add(KV_1));
		BOOST_REQUIRE(Source.Add(KV_2));
		BOOST_REQUIRE(Source.Add(KV_3));

		IntStringRBTree T;
		const IntStringRBTree::KeyValueType OTHER_KV_1{ 11, std::string("eleven") };
		const IntStringRBTree::KeyValueType OTHER_KV_2{ 12, std::string("twelve") };
		BOOST_REQUIRE(T.Add(OTHER_KV_1));
		BOOST_REQUIRE(T.Add(OTHER_KV_2));


		T = Source;
		BOOST_REQUIRE_EQUAL(T.Num(), 3);
		BOOST_REQUIRE(T.Contains(KV_1.Key));
		BOOST_REQUIRE(T.Contains(KV_2.Key));
		BOOST_REQUIRE(T.Contains(KV_3.Key));
		BOOST_REQUIRE(!T.Contains(OTHER_KV_1.Key));
		BOOST_REQUIRE(!T.Contains(OTHER_KV_2.Key));
	}
	BOOST_AUTO_TEST_SUITE_END()

	BOOST_AUTO_TEST_SUITE_END() // Minimal
	BOOST_AUTO_TEST_SUITE_END() // Container
	BOOST_AUTO_TEST_SUITE_END() // TRBTreeTestSuite
	BOOST_AUTO_TEST_SUITE_END() // Core
}