#pragma once

#include "TVectorTestUtils.h"

namespace
{

BOOST_AUTO_TEST_SUITE(Core)
BOOST_AUTO_TEST_SUITE(Container)
BOOST_AUTO_TEST_SUITE(TVectorTestSuite)
BOOST_AUTO_TEST_SUITE
(
	IterationSuite
)

BOOST_AUTO_TEST_CASE(IteratorElementType)
{
	static_assert(std::is_same<TVector<int>::IteratorType::ElementType, int>::value);
	static_assert(std::is_same<const TVector<int>::IteratorType::ElementType, const int>::value);
}


BOOST_AUTO_TEST_CASE
(
	ReverseIteratorMath,
	*boost::unit_test::depends_on("Core/Container/TVectorTestSuite/IterationSuite/ReverseIterationTest")
)
{
	BOOST_TEST_CHECKPOINT("Initialization");
	TVector<int> V;
	for (int i = 0; i < 10; i++)
	{
		V.Add(i);
	}

	BOOST_TEST_CHECKPOINT("Addition test");
	TVector<int>::BackwardIteratorType const ItFirstElement = V.BackwardIterator() + (V.Len() - 1);
	BOOST_REQUIRE(ItFirstElement == V.Iterator());

	BOOST_REQUIRE_EQUAL(V.BackwardIterator() + 0, V.BackwardIterator());
	BOOST_REQUIRE_EQUAL(*(V.BackwardIterator() + 2), *(V.Iterator() + (V.Len() - 1) - 2));
	BOOST_REQUIRE_EQUAL(*(V.BackwardIterator() + 3), *(V.Iterator() + (V.Len() - 1) - 3));

	BOOST_TEST_CHECKPOINT("Addition end test");
	BOOST_REQUIRE( ! (V.BackwardIterator() + V.Len()) );
	BOOST_REQUIRE( ! ((V.BackwardIterator() + V.Len()) + 0));

	BOOST_TEST_CHECKPOINT("Subtraction test");
	BOOST_REQUIRE_EQUAL(ItFirstElement - 0, ItFirstElement);
	BOOST_REQUIRE_EQUAL(V.BackwardIterator() - 0, V.BackwardIterator());
	BOOST_REQUIRE_EQUAL(*(ItFirstElement - 2), *(V.Iterator() + 2));
	BOOST_REQUIRE_EQUAL(*(ItFirstElement - 3), *(V.Iterator() + 3));
}

BOOST_AUTO_TEST_CASE
(
	ReverseIterationDecrementTest,
	*boost::unit_test::depends_on("Core/Container/TVectorTestSuite/IterationSuite/ReverseIterationTest")
)
{
	BOOST_TEST_CHECKPOINT("Initialization");
	TVector<int> V;
	for (int i = 0; i < 10; i++)
	{
		V.Add(i);
	}

	BOOST_TEST_CHECKPOINT("Iteration");
	{
		int RefIndex = 0;

		TVector<int>::BackwardIteratorType ItInitial = V.BackwardIterator() + (V.Len() - 1);

		TVector<int>::BackwardIteratorType It = ItInitial;
		for (; ; --It)
		{
			BOOST_REQUIRE(!It.IsEnd());
			BOOST_REQUIRE(!!It);
			BOOST_REQUIRE_EQUAL(It.ToBaseIterator().Index(), RefIndex);
			BOOST_REQUIRE_EQUAL(It.Get(), V[RefIndex]);
			BOOST_REQUIRE_EQUAL(*It, V[RefIndex]);
			BOOST_REQUIRE(nullptr != It.GetPtr());
			BOOST_REQUIRE_EQUAL(*It.GetPtr(), V[RefIndex]);

			RefIndex++;

			if (It == V.BackwardIterator())
			{
				break;
			}
		}
		BOOST_REQUIRE(It == V.BackwardIterator());
	}
}

BOOST_AUTO_TEST_CASE
(
	ReverseIterationTest,
	*boost::unit_test::depends_on("Core/Container/TVectorTestSuite/IterationSuite/SimpleIterationTest")
)
{
	BOOST_TEST_CHECKPOINT("Initialization");
	TVector<int> V;
	for (int i = 0; i < 10; i++)
	{
		V.Add(i);
	}

	BOOST_TEST_CHECKPOINT("Iteration");
	{
		int RefIndex = V.LastIndex();
		for (TVector<int>::BackwardIteratorType It = V.BackwardIterator(); It; ++It)
		{
			BOOST_REQUIRE(!It.IsEnd());
			BOOST_REQUIRE(!!It);
			BOOST_REQUIRE_EQUAL(It.ToBaseIterator().Index(), RefIndex);
			BOOST_REQUIRE_EQUAL(It.Get(), V[RefIndex]);
			BOOST_REQUIRE_EQUAL(*It, V[RefIndex]);
			BOOST_REQUIRE(nullptr != It.GetPtr());
			BOOST_REQUIRE_EQUAL(*It.GetPtr(), V[RefIndex]);

			RefIndex--;
		}
	}
}

BOOST_AUTO_TEST_CASE(SimpleIterationTest)
{
	BOOST_TEST_CHECKPOINT("Initialization");
	TVector<int> V;
	for (int i = 0; i < 10; i++)
	{
		V.Add(i);
	}

	BOOST_TEST_CHECKPOINT("Iteration");
	{
		int RefIndex = 0;
		for (TVector<int>::IteratorType It = V.Iterator(); It; ++It)
		{
			BOOST_REQUIRE(!It.IsEnd());
			BOOST_REQUIRE(!!It);
			BOOST_REQUIRE_EQUAL(It.Index(), RefIndex);
			BOOST_REQUIRE_EQUAL(It.Get(), V[RefIndex]);
			BOOST_REQUIRE_EQUAL(*It, V[RefIndex]);
			BOOST_REQUIRE(nullptr != It.GetPtr());
			BOOST_REQUIRE_EQUAL(*It.GetPtr(), V[RefIndex]);

			RefIndex++;
		}
	}

	BOOST_TEST_CHECKPOINT("Equality");
	{
		BOOST_REQUIRE(V.Iterator() == V.Iterator());
		TVector<int>::IteratorType ItCurr = V.Iterator();
		++ItCurr;
		BOOST_REQUIRE(V.Iterator() != ItCurr);

		TVector<int>::IteratorType ItEnd = V.Iterator();
		while (++ItEnd) {}
		BOOST_REQUIRE(ItEnd == ItEnd);
		BOOST_REQUIRE(ItCurr != ItEnd);
	}

	BOOST_TEST_CHECKPOINT("Copy-construction");
	TVector<int>::IteratorType ItCopy = V.Iterator();
	BOOST_REQUIRE(ItCopy == V.Iterator());

	BOOST_TEST_CHECKPOINT("Copy-assignment");
	ItCopy = V.Iterator();
	BOOST_REQUIRE(ItCopy == V.Iterator());

	BOOST_TEST_CHECKPOINT("Move-construction");
	TVector<int>::IteratorType ItMovedCopy = std::move(V.Iterator());
	BOOST_REQUIRE(ItMovedCopy == V.Iterator());

	BOOST_TEST_CHECKPOINT("Move-assignment");
	ItMovedCopy = std::move(V.Iterator());
	BOOST_REQUIRE(ItMovedCopy == V.Iterator());

	BOOST_TEST_CHECKPOINT("Add");
	TVector<int>::IteratorType const AdvancedIt = V.Iterator() + V.Len();
	TVector<int>::IteratorType FowardedIt = V.Iterator();

	FowardedIt.Forward(0);
	BOOST_REQUIRE(FowardedIt == V.Iterator());

	FowardedIt.Forward(1);
	BOOST_REQUIRE_EQUAL(*FowardedIt, V[1]);

	FowardedIt.Forward(V.Len() - 2);
	BOOST_REQUIRE_EQUAL(*FowardedIt, V[V.LastIndex()]);
	FowardedIt.Forward(1);
	BOOST_REQUIRE( ! FowardedIt );

	BOOST_REQUIRE( ! AdvancedIt );
	BOOST_REQUIRE( V.Iterator() + 0 == V.Iterator());
	BOOST_REQUIRE_EQUAL(*(V.Iterator() + 1), V[1]);
	BOOST_REQUIRE_EQUAL(*(V.Iterator() + 2), V[2]);

	BOOST_TEST_CHECKPOINT("Subtract");
	BOOST_REQUIRE(V.Iterator() - 0 == V.Iterator());
	BOOST_REQUIRE(AdvancedIt - 0 == AdvancedIt);
	BOOST_REQUIRE(*(AdvancedIt - V.Len()) == V.First());
	BOOST_REQUIRE(*(AdvancedIt - 1) == V.Last(0));

	BOOST_REQUIRE((AdvancedIt - 1).AtLast());

	BOOST_TEST_CHECKPOINT("BackwardIteration");
	{
		int RefIndex = V.LastIndex();
		for (TVector<int>::IteratorType It = V.Iterator() + RefIndex; ; It--)
		{			
			BOOST_REQUIRE(!It.IsEnd());
			BOOST_REQUIRE(!!It);
			BOOST_REQUIRE_EQUAL(It.Index(), RefIndex);
			BOOST_REQUIRE_EQUAL(It.Get(), V[RefIndex]);
			BOOST_REQUIRE_EQUAL(*It, V[RefIndex]);
			BOOST_REQUIRE(nullptr != It.GetPtr());
			BOOST_REQUIRE_EQUAL(*It.GetPtr(), V[RefIndex]);

			if (It.AtStart())
			{
				break;
			}

			RefIndex--;
		}
	}

	BOOST_TEST_CHECKPOINT("ConstIteration");
	{
		int RefIndex = 0;
		for (TVector<int>::ConstIteratorType It = V.ConstIterator(); It; ++It)
		{
			BOOST_REQUIRE(!It.IsEnd());
			BOOST_REQUIRE(!!It);
			BOOST_REQUIRE_EQUAL(It.Index(), RefIndex);
			BOOST_REQUIRE_EQUAL(It.Get(), V[RefIndex]);
			BOOST_REQUIRE_EQUAL(*It, V[RefIndex]);
			BOOST_REQUIRE(nullptr != It.GetPtr());
			BOOST_REQUIRE_EQUAL(*It.GetPtr(), V[RefIndex]);

			RefIndex++;
		}
	}

	BOOST_TEST_CHECKPOINT("Updating by iterator");
}

BOOST_AUTO_TEST_SUITE_END() // IterationSuit
BOOST_AUTO_TEST_SUITE_END() // TVectorTestSuite
BOOST_AUTO_TEST_SUITE_END() // Container
BOOST_AUTO_TEST_SUITE_END() // Core
} 