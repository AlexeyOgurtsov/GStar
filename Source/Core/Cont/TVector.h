#pragma once

#include "ContainerSystem.h"
#include <boost/serialization/access.hpp>
#include <boost/serialization/split_member.hpp>
#include <tuple>
#include "Core/Mem/MemUtils.h"

/**
* TODO WARNING!!!
* 1. Copy constructor and copy-assign implementation!!!
*/

/**
* Resize policy base class non-template part
*/
struct VectorResizePolicyBase
{
};

/**
* Resize policy base class
*/
template<class T>
struct TVectorResizePolicyBase : public VectorResizePolicyBase
{
};

template<class T>
struct IsVectorResizePolicy
{
	static constexpr bool Value = std::is_base_of<VectorResizePolicyBase, T>::value;
};


/**
* Default resize policy
*/
template<class T>
struct DefaultVectorResizePolicy : public TVectorResizePolicyBase<T>
{
	constexpr static const char Name[] = "Default";

	constexpr static int32_t SBO_LENGTH = 32;

	constexpr int32_t CalcNewCapacity(int32_t Length) const
	{
			// Constant is used until the capacity is reached the given value
 			const float GROW_FACTOR                = 1.5F;

			// Constant is used for contains with big size
			const float GROW_FACTOR_BIG            = 1.2F;

			const int32_t BIG_SIZE            = 1024 * 1024;

			if(Length*sizeof(T) < BIG_SIZE)
			{
				return static_cast<int32_t>(Length * GROW_FACTOR);
			}
			else
			{
				return static_cast<int32_t>(Length * GROW_FACTOR_BIG);
			}
		}   
	};

	/************************************************************************************************************
	* Vector
	************************************************************************************************************/
	template<class T, template<class> class ResizePolicy = DefaultVectorResizePolicy>
	class TVector
	{
	public:
		static_assert(IsVectorResizePolicy<ResizePolicy<T>>::Value, "TVector: Resize policy must be inherited from the corresponding base class");

		using ThisType = TVector<T, ResizePolicy>;
		using ResizePolicyType = ResizePolicy<T>;
		using ValueType = T;
		using ElementType = T;

		template<class T, template<class> class OtherResizePolicy> friend class TVector;
		friend class boost::serialization::access;

		template<class T, template<class> class ResizePolicy>
		friend std::string* AppendExtraDebugInfoString(std::string* pOutString, const TVector<T, ResizePolicy>& V);

		template<class ContTypeArg>
		class TGeneralIterator;

		/**
		* Iterator type.
		*/
		using IteratorType = TGeneralIterator<ThisType>;
		using ConstIteratorType = TGeneralIterator<const ThisType>;

		/**
		* Backward iterator types.
		*/
		using BackwardIteratorType = TReverseIterator<IteratorType>;
		using ConstBackwardIteratorType = TReverseIterator<ConstIteratorType>;

		template<class ContTypeArg>
		class TGeneralIterator : public TIteratorBase
		<
			TGeneralIterator<ContTypeArg>,
			std::is_const<ContTypeArg>, EIteratorClass::RandomAccess
		>
		{
		public:
			template<class OtherContType>
			friend class TGeneralIterator;

			using ThisType = TGeneralIterator<ContTypeArg>;
			using ElementType = 
				std::tuple_element_t
				<
					std::is_const<ContTypeArg>::value,
					std::tuple<typename ContTypeArg::ElementType, const typename ContTypeArg::ElementType>
				>;

			/**
			* Default ctor: Creates floating end iterator.
			*/
			TGeneralIterator() :
				pCont(nullptr)
			,	Ind(INDEX_NONE) {}

			/**
			* Constructor: Creates iterator.
			*/
			TGeneralIterator(ContTypeArg* pInCont, int InIndex) :
				pCont(pInCont)
			,	Ind(InIndex)
			{
				BOOST_ASSERT(pCont);
				BOOST_ASSERT_MSG(pCont->IsIndexValid(InIndex), "TVector: Iterator: index must be valid");
			}

			/**
			* Copy-constructs.
			*/
			template<class OtherContType>
			TGeneralIterator(const TGeneralIterator<OtherContType>& InOther)
			{
				static_assert(IteratorAssignableByConst<ThisType, TGeneralIterator<OtherContType>>::Value, "Iterators const mismatch");
				*this = InOther;
			}

			/**
			* Copies
			*/
			template<class OtherContType>
			ThisType operator=(const TGeneralIterator<OtherContType>& InOther)
			{
				static_assert(IteratorAssignableByConst<ThisType, TGeneralIterator<OtherContType>>::Value, "Iterators const mismatch");
				pCont = InOther.pCont;
				Ind = InOther.Ind;
				return *this;
			}

			/**
			* Advances iterator to the next element.
			*/
			TGeneralIterator& operator++()
			{
				BOOST_ASSERT_MSG( ! IsFloatingEnd(), "Iterator: floating end iterator may never be incremented" );
				BOOST_ASSERT_MSG( ! IsEnd(), "TVector: Iterator: increment(++): already at end");
				Ind++;
				return *this;
			}

			/**
			* Advances iterator to the next element.
			*/
			TGeneralIterator operator++(int)
			{
				TGeneralIterator OldIt = *this;
				TGeneralIterator::operator++();
				return OldIt;
			}

			/**
			* Sets to previous element.
			*/
			TGeneralIterator& operator--()
			{
				BOOST_ASSERT_MSG( ! IsFloatingEnd(), "Iterator: floating end iterator may never be decremented");
				BOOST_ASSERT_MSG(Ind > 0, "TVector: Iterator: decrement(--): already at first element");
				Ind--;
				return *this;
			}

			/**
			* Sets to previous element.
			*/
			TGeneralIterator operator--(int)
			{
				TGeneralIterator OldIt = *this;
				TGeneralIterator::operator--();
				return OldIt;
			}

			/**
			* += operator.
			*/
			TGeneralIterator& operator+=(int32_t Count)
			{
				NextIterator.Forward(Count);
				return *this;
			}

			/**
			* -= operator.
			*/
			TGeneralIterator& operator-=(int32_t Count)
			{
				NextIterator.Backward(Count);
				return *this;
			}

			/**
			* Advances iterator forward by the given number of elements and returns a new iterator value.
			*/
			TGeneralIterator operator+(int32_t Count) const
			{
				TGeneralIterator NextIterator { *this };
				NextIterator.Forward(Count);
				return NextIterator;
			}

			/**
			* Advances iterator forward by the given number of elements backward and returns a new iterator value.
			*/
			TGeneralIterator operator-(int32_t Count) const
			{
				TGeneralIterator NextIterator{ *this };
				NextIterator.Backward(Count);
				return NextIterator;
			}

			/**
			* Advances iterator forward by the given number of elements.
			*/
			void Forward(int32_t Count)
			{
				if (Count == 0) { return; }
				BOOST_ASSERT_MSG( Count >= 0, "TVector: Iterator: Forward: count must be positive" );
				BOOST_ASSERT_MSG( ! IsEnd(), "TVector: Iterator: Forward: iterator must be NON-end iterator or count is equal to zero");
				int32_t const NextIndex = Ind + Count;
				if (pCont->IsIndexValid(NextIndex))
				{
					Ind = NextIndex;
				}
				else
				{
					// Making an end iterator
					Ind = pCont->Len();
				}
			}


			/**
			* Advances iterator backward by the given number of elements.
			*/
			void Backward(int32_t Count)
			{
				if (Count == 0) { return; }
				BOOST_ASSERT_MSG(Count >= 0, "TVector: Iterator: Backward: count must be positive");
				BOOST_ASSERT_MSG( ! IsFloatingEnd(), "TVector: Iterator: Backward: iterator must be NOT floating end iterator");
				Ind -= Count;
				BOOST_ASSERT_MSG(Count <= pCont->Len(), "TVector: Iterator: Backward: Count must be <= Length of the array");
			}

			/**
			* Current element index (from zero).
			*/
			__forceinline int32_t Index() const { return Ind; }

			/**
			* Current element.
			*/
			__forceinline decltype(auto) Get() const { return pCont->operator[](Ind); }

			/**
			* Gets ptr.
			*/
			__forceinline decltype(auto) GetPtr() const { return &(pCont->operator[](Ind)); }


			/**
			* Member access by pointer.
			*/
			__forceinline decltype(auto) operator->() const
			{
				return GetPtr();
			}

			/**
			* Current element.
			*/
			decltype(auto) operator*() const { return Get(); }

			/**
			* NOT Is end iterator.
			*/
			__forceinline operator bool() const
			{
				return ! IsEnd();
			}

			/**
			* Is end iterator.
			*/
			__forceinline bool operator!() const
			{
				return IsEnd();
			}

			__forceinline bool IsEnd() const
			{
				return IsFloatingEnd() || (Ind >= pCont->Len());
			}

			__forceinline bool IsFloatingEnd() const
			{
				return Ind == INDEX_NONE;
			}

			__forceinline bool AtStart() const
			{
				return Ind == 0;
			}

			__forceinline bool AtLast() const
			{
				return Ind == (pCont->Len() - 1);
			}

			template<class OtherContType>
			bool operator==(const TGeneralIterator<OtherContType>& InOther) const
			{
				if (IsEnd() && InOther.IsEnd())
				{
					return true;
				}
				else if (IsEnd() || InOther.IsEnd())
				{
					return false;
				}
				BOOST_ASSERT_MSG(pCont == InOther.pCont, "TVector: comparison: iterators must be of the same type");
				return Ind == InOther.Ind;
			}

			template<class OtherContType>
			bool operator!=(const TGeneralIterator<OtherContType>& InOther) const
			{
				return ! (operator==(InOther));
			}

			template<class OtherContType>
			int operator-(const TGeneralIterator<OtherContType>& InOther) const
			{
				return Ind - InOther.Ind;
			}

		private:
			ContTypeArg* pCont;
			int32_t Ind;
		};

		~TVector();

		/**
		* Creates empty vector with SBO only.
		*/
		constexpr TVector() {}

		/**
		* Creates empty vector and initializes capacity for desired length.
		*/
		constexpr TVector(size_t InDesiredLength, EForceInitCapacity) 
		{
			SetupBuffer_ForDesiredLength(InDesiredLength);
		}

		/**
		* Copy constructs.
		*/
		TVector(const TVector<T, ResizePolicy>& Other);

		/**
		* Copy construction.
		*/
		template<template<class> class OtherResizePolicy>
		TVector(const TVector<T, OtherResizePolicy>& Other);

		/**
		* Moves-constructs.
		*/
		TVector(TVector<T, ResizePolicy>&& Other);
		
		/**
		* Moving.
		*/
		template<template<class> class OtherResizePolicy>
		TVector(TVector<T, OtherResizePolicy>&& Other);

		/**
		* Copies.
		*/
		TVector& operator=(const TVector<T, ResizePolicy>& Other);

		/**
		* Copying.
		*/
		template<template<class> class OtherResizePolicy>
		ThisType& operator=(const TVector<T, OtherResizePolicy>& Other);

		/**
		* Moving.
		*/
		TVector& operator=(TVector<T, ResizePolicy>&& Other);

		/**
		* Move-copying.
		*
		* @UNIT-TESTED
		*/
		template<template<class> class OtherResizePolicy>
		ThisType& operator=(TVector<T, OtherResizePolicy>&& Other);

		/**
		* Construction with the given initial length.
		*
		* NO initialization is performed.
		*/
		TVector(int32_t InitialLength);

		/**
		* Construction with the given initial length and forcefully initialized with the default ctor.
		*/
		TVector(int32_t InitialLength, EForceInit);

		/**
		* Construction with the given initiale length, initializing with copy-construction of the given value.
		*/
		TVector(int32_t InitialLength, const T& Templ);

		/**
		* Construction with initializer list
		*/
		TVector(std::initializer_list<T> InitList);

		/**
		* Construction from array
		*/
		TVector(const T* Ptr, int32_t InCount);

		/**
		* Equality test.
		*/
		template<template<class> class OtherResizePolicy>
		bool operator==(const TVector<T,OtherResizePolicy>& Other) const;

		/**
		* Non equality test.
		*/
		template<template<class> class OtherResizePolicy>
		bool operator!=(const TVector<T, OtherResizePolicy>& Other) const;

		template<class Archive>
		void save(Archive& Ar, const unsigned int Version) const
		{
			Ar << Length;
			for (int i = 0; i < Length; i++)
			{
				Ar << pBuf[i];
			}
		}

		template<class Archive>
		void load(Archive& Ar, const unsigned int Version)
		{
			int32_t LoadedLength;
			Ar >> LoadedLength;
			SetLength(LoadedLength);
			for (int i = 0; i < LoadedLength; i++)
			{
				Ar >> pBuf[i];
			}
		}

		BOOST_SERIALIZATION_SPLIT_MEMBER()

		/**
		* Count bytes needed to store this class and its buffer with current capacity. 
		* Includes both the class layout and dynamic memory.
		* TODO: Special case for pointers.
		*/
		int32_t CountTotalBytes() const
		{
			return sizeof(*this) + ElementSize() * MaxLength;
		}

		/**
		* Count bytes needed to store only the minimum size required to store the given buffer.
		*/
		int32_t CountMinimumSizeBytes() const
		{
			return sizeof(*this) + ElementSize() * Length;
		}

		/**
		* Counts the overhead of capacity.
		*/
		int32_t CountCapacityOverhead() const
		{
			return  CountTotalBytes() - CountMinimumSizeBytes();
		}

		/**
		* Count only bytes that are to be serialized.
		*/
		int32_t CountSerializeBytes() const
		{
			return sizeof(Length) + ElementSize() * Length;
		}

		/**
		* Size of a single element instance.
		* NOTE: For pointers accounts only pointer size and not the size of pointed-to content.
		*/
		static constexpr int32_t ElementSize()
		{
			return sizeof(T);
		}

		/**
		* Returns true if index is valid.
		*/
		__forceinline bool IsIndexValid(int32_t Index) const
		{
			return Index >= 0 && Index < Length;
		}

		/**
		* Checks using assert that the given index is valid.
		*/
		__forceinline void IndexValidAssert(int32_t Index) const
		{
			assert(IsIndexValid(Index));
		}

		/**
		* Checks that index range is valid.
		*/
		__forceinline bool IsRangeValid(int32_t Start, int32_t End) const
		{
			return 
				(Start <= End) &&
				(0 <= Start) &&
				// WARNING: End is less or equal than (not simply less) because end is one more than the last element index:
				(End <= Length);
		}

		/**
		* Checks with assert that the range is valid.
		*/
		__forceinline void RangeValidAssert(int32_t Start, int32_t End) const
		{
			assert(IsRangeValid(Start, End));
		}

		/**
		* Counts values equal to the given.
		*/
		int32_t CountValues(const T& val) const
		{
			int32_t result = 0;
			for (int i = 0; i < Length; i++)
			{
				if (pBuf[i] == val)
				{
					result++;
				}
			}
			return result;
		}

		/**
		* Counts values for those the given predicate returns true.
		*/
		template<class Pred>
		int32_t CountPredicate(const Pred& P) const
		{
			int32_t result = 0;
			for (int i = 0; i < Length; i++)
			{
				if (Pred(pBuf[i]))
				{
					result++;
				}
			}
			return result;
		}

		/**
		* Counts values for those the given predicate returns false.
		*/
		template<class Pred>
		int32_t CountNotPredicate(const Pred& P) const 
		{
			int32_t result = 0;
			for (int i = 0; i < Length; i++)
			{
				if (false == Pred(pBuf[i]))
				{
					result++;
				}
			}
			return result;
		}

		/**
		* Checks that the given predicate returns true for all elements.
		*/
		template<class Pred>
		__forceinline int32_t All(const Pred& P) const
		{
			return false == ContainsNotPredicate(P);
		}

		/**
		* Searches for the given element and returns its index.
		*
		* @returns: index of the element. Returns INDEX_NONE if not found.
		*/
		int32_t Find(const T& Val) const
		{
			for (int i = 0; i < Length; i++)
			{
				if (pBuf[i] == Val) { return i; }
			}
			return INDEX_NONE;
		}

		/**
		* Searches for the first value that is NOT equal to the given.
		*
		* @returns: index of the element. Returns INDEX_NONE if not found.
		*/
		int32_t FindNot(const T& Val) const
		{
			for (int i = 0; i < Length; i++)
			{
				if (pBuf[i] != Val) { return i; }
			}
			return INDEX_NONE;
		}

		/**
		* Searches the given value within elements with indices in the given range.
		*
		* @returns: index of the element. Returns INDEX_NONE if not found.
		*/
		int32_t Find_InRange(const T& Val, int32_t Start, int32_t End) const
		{
			RangeValidAssert(Start, End);
			for (int i = Start; i < End; i++)
			{
				if (pBuf[i] == Val) { return i; }
			}
			return INDEX_NONE;
		}

		/**
		* Searches element with value not equal to the given 
		* within elements with indices in the given range.
		*
		* @returns: index of the element. Returns INDEX_NONE if not found.
		*/
		int32_t FindNot_InRange(const T& Val, int32_t Start, int32_t End) const
		{
			RangeValidAssert(Start, End);
			for (int i = Start; i < End; i++)
			{
				if (pBuf[i] != Val) { return i; }
			}
			return INDEX_NONE;
		}

		/**
		* Searches for the given element from the container's last element and returns its index.
		*
		* @returns: index of the element. Returns INDEX_NONE if not found.
		*/
		int32_t FindBackward(const T& Val) const
		{
			for (int i = Length; --i >= 0; )
			{
				if (pBuf[i] == Val) { return i; }
			}
			return INDEX_NONE;
		}

		/**
		* Searches for the element not equal to the given starting from the container's last element and returns its index.
		*
		* @returns: index of the element. Returns INDEX_NONE if not found.
		*/
		int32_t FindNotBackward(const T& Val) const
		{
			for (int i = Length; --i >= 0; )
			{
				if (pBuf[i] != Val) { return i; }
			}
			return INDEX_NONE;
		}

		/**
		* Searches for the given element not equal to the given backward in the given range.
		*
		* WARNING: Range is to be specified as ordinary "forward" indices,
		* they're not counted from the end.
		*
		* @returns: index of the element. Returns INDEX_NONE if not found.
		*/
		int32_t FindNotBackward_InRange(const T& Val, int32_t Start, int32_t End) const
		{
			RangeValidAssert(Start, End);
			for (int i = End; --i >= 0; )
			{
				if (pBuf[i] != Val) { return i; }
			}
			return INDEX_NONE;
		}

		/**
		* Returns true if the given element exists, false otherwise.
		*/
		__forceinline bool Contains(const T& Val) const
		{
			return IsIndexValid(Find(Val));
		}

		/**
		* Returns true if the given element exists within the given range of indices, false otherwise.
		*/
		__forceinline bool Contains_InRange(const T& Val, int32_t Start, int32_t End) const
		{
			IndexValidAssert(Start, End);
			return IsValidIndex(Find_InRange(Val, Start, Count));
		}

		/**
		* Returns true if an element for those predicate returns true exists.
		*/
		template<class Pred>
		__forceinline bool ContainsPredicate(const Pred& P) const
		{
			return IsIndexValid(FindByPredicate(P));
		}

		/**
		* Returns true if an element for those predicate returns false exists.
		*/
		template<class Pred>
		__forceinline bool ContainsNotPredicate(const Pred& P) const
		{
			return IsIndexValid(FindNotByPredicate(P));
		}

		/**
		* Returns true if an element for those predicate returns true exists in the given range
		*/
		template<class Pred>
		__forceinline bool ContainsPredicate_InRange(const Pred& P, int32_t Start, int32_t End) const
		{
			SearchRangeValidAssert(Start, End);
			return IsValidIndex(FindByPredicate_InRange(P, Start, Count));
		}

		/**
		* Returns true if an element for those predicate returns false exists in the given range
		*/
		template<class Pred>
		__forceinline bool ContainsNotPredicate_InRange(const Pred& P, int32_t Start, int32_t End) const
		{
			SearchRangeValidAssert(Start, End);
			return IsValidIndex(FindNotByPredicate_InRange(P, Start, Count));
		}

		/**
		* Searches until the given predicate returns true.	
		*
		* @returns: index of the element. Returns INDEX_NONE if not found.
		*/
		template<class Pred>
		int32_t FindByPredicate(const Pred& P) const
		{
			for (int i = 0; i < Length; i++)
			{
				if (P(pBuf[i])) 
				{
					return i;
				}
			}
			return INDEX_NONE;
		}

		/**
		* Searches until the given predicate returns false.
		*
		* @returns: index of the element. Returns INDEX_NONE if not found.
		*/
		template<class Pred>
		int32_t FindNotByPredicate(const Pred& P) const
		{
			for (int i = 0; i < Length; i++)
			{
				if (false == P(pBuf[i]))
				{
					return i;
				}
			}
			return INDEX_NONE;
		}

		/**
		* Searches until the given predicate returns true.	
		* Only elements in the given range are taken into account.
		*
		* @returns: index of the element. Returns INDEX_NONE if not found.
		*/
		template<class Pred>
		int32_t FindByPredicate_InRange(const Pred& P, int32_t Start, int32_t End) const
		{
			SearchRangeValidAssert(Start, End);
			for (int i = Start; i < End; i++)
			{
				if (P(pBuf[i]))
				{
					return i;
				}
			}
			return INDEX_NONE;
		}

		/**
		* Searches until the given predicate returns false.
		* Only elements in the given range are taken into account.
		*
		* @returns: index of the element. Returns INDEX_NONE if not found.
		*/
		template<class Pred>
		int32_t FindNotByPredicate_InRange(const Pred& P, int32_t Start, int32_t End) const
		{
			SearchRangeValidAssert(Start, End);
			for (int i = Start; i < End; i++)
			{
				if (false == P(pBuf[i]))
				{
					return i;
				}
			}
			return INDEX_NONE;
		}

		/**
		* Searches until the given predicate returns true.
		* Starts search from the last element.
		*
		* @returns: index of the element. Returns INDEX_NONE if not found.
		*/
		template<class Pred>
		int32_t FindBackwardByPredicate(const Pred& P) const
		{
			for (int i = Length; --i >= 0; )
			{
				if (P(pBuf[i]))
				{
					return i;
				}
			}
			return INDEX_NONE;
		}

		/**
		* Searches until the given predicate returns false.
		* Starts search from the last element.
		*
		* @returns: index of the element. Returns INDEX_NONE if not found.
		*/
		template<class Pred>
		int32_t FindNotBackwardByPredicate(const Pred& P) const
		{
			for (int i = Length; --i >= 0; )
			{
				if (false == P(pBuf[i]))
				{
					return i;
				}
			}
			return INDEX_NONE;
		}

		/**
		* Searches until the given predicate returns true.
		* Starts search from the last element.
		*
		* @returns: index of the element. Returns INDEX_NONE if not found.
		* @param: Start, End: start and end indices of the range to search in
		* Warning: These indices are ordinary "foward" indices, the're not counted from the end of the array.
		*/
		template<class Pred>
		int32_t FindBackwardByPredicate_InRange(const Pred& P, int32_t Start, int32_t End) const
		{
			SearchRangeValidAssert(Start, End);
			for (int i = End; --i >= Start; )
			{
				if (P(pBuf[i]))
				{
					return i;
				}
			}
			return INDEX_NONE;
		}

		/**
		* Searches until the given predicate returns false.
		* Starts search from the last element.
		*
		* @returns: index of the element. Returns INDEX_NONE if not found.
		* @param: Start, End: start and end indices of the range to search in
		* Warning: These indices are ordinary "foward" indices, the're not counted from the end of the array.
		*/
		template<class Pred>
		int32_t FindNotBackwardByPredicate_InRange(const Pred& P, int32_t Start, int32_t End) const
		{
			SearchRangeValidAssert(Start, End);
			for (int i = End; --i >= Start; )
			{
				if (false == P(pBuf[i]))
				{
					return i;
				}
			}
			return INDEX_NONE;
		}

		/**
		* Clear the vector completely
		*/
		void Clear();

		/**
		* STL-complient iterator to the first element
		*/
		__forceinline const T* begin() const { return pBuf; }
		__forceinline T* begin() { return pBuf; }

		/**
		* STL-complient iterator to the end element
		*/
		__forceinline const T* end() const { return pBuf + Length; }
		__forceinline T* end() { return pBuf + Length; }

		/**
		* Returns iterator to the first element.
		*/
		ConstIteratorType Iterator() const
		{
			BOOST_ASSERT_MSG(!Empty(), "TVector: Getting Iterator: Container must be NON-empty");
			return ConstIteratorAt(0);
		}

		/**
		* Returns iterator to the first element.
		*/
		ConstIteratorType ConstIterator() const { return Iterator(); }

		/**
		* Returns iterator to the first element.
		*/
		IteratorType Iterator()
		{
			BOOST_ASSERT_MSG( ! Empty(), "TVector: Getting Iterator: Container must be NON-empty" );
			return IteratorAt(0);
		}

		/**
		* Returns backward iterator to the last element.
		*/
		ConstBackwardIteratorType BackwardIterator() const
		{
			BOOST_ASSERT_MSG( ! Empty() , "TVector: Getting backward iterator: Container must be NON-empty");
			return ConstBackwardIteratorType( LastIndex() );
		}

		/**
		* Returns iterator to the first element.
		*/
		ConstBackwardIteratorType ConstBackwardIterator() const { return BackwardIterator(); }

		/**
		* Returns backward iterator to the last element.
		*/
		BackwardIteratorType BackwardIterator()
		{
			BOOST_ASSERT_MSG( ! Empty(), "TVector: Getting backward iterator: Container must be NON-empty");
			return BackwardIteratorType{ IteratorAt(LastIndex()) };
		}

		/**
		* Returns iterator to the element with the given index.
		* The index must be valid.
		*/
		IteratorType IteratorAt(int32_t Index)
		{
			BOOST_ASSERT_MSG( IsIndexValid(Index), "TVector: Getting Iterator: Index must be valid");
			return IteratorType(this, Index);
		}

		/**
		* Returns constant iterator to the element with the given index.
		* The index must be valid.
		*/
		ConstIteratorType ConstIteratorAt(int32_t Index) const
		{
			BOOST_ASSERT_MSG(IsIndexValid(Index), "TVector: Getting Iterator: Index must be valid");
			return ConstIteratorType(this, Index);
		}

		/**
		* Returns constant iterator to the element with the given index.
		* The index must be valid.
		*/
		ConstIteratorType IteratorAt(int32_t Index) const
		{
			return ConstIteratorAt(Index);
		}

		/**
		* The same as Length in elements.
		*/
		__forceinline constexpr int32_t Num() const { return Length; }
		
		/**
		* Length in elements
		*/
		__forceinline constexpr int32_t Len() const { return Length; }

		/**
		* Data pointer
		*
		* Never nullptr.
		*/
		__forceinline constexpr const T* Data() const { return pBuf; }

		/**
		* Data pointer
		*/
		__forceinline constexpr T* Data() { return pBuf; }

		/**
		* Max length in elements (capacity)
		*/
		__forceinline constexpr int32_t MaxLen() const { return MaxLength; }

		/**
		* Returns true if the container is valid (e.g. NOT moved).
		*/
		bool IsValid() const { return pBuf; }

		/**
		* Accesses by index with checking.
		*/
		__forceinline const T& operator[](int32_t i) const
		{
			IndexValidAssert(i);
			return pBuf[i];
		}

		/**
		* Accesses by index with checking.
		*/
		__forceinline T& operator[](int32_t i)
		{
			IndexValidAssert(i);
			return pBuf[i];
		}

		/**
		* Returns first element in the array.
		* 
		* CAUTION: count of elements must be greater or equal to one
		*/
		__forceinline const T& First() const                 
		{
			assert(false == Empty());
			return pBuf[0];
		}

		/**
		* Returns first element in the array.
		*
		* CAUTION: count of elements must be greater or equal to one
		*/
		__forceinline T&  First()
		{
			assert(false == Empty());
			return pBuf[0];
		}

		/**
		* Returns nth-last element in the array.
		*
		* CAUTION: count of elements must be greater or equal to one.
		*/
		__forceinline const T& Last(int32_t IndexFromEnd = 0) const
		{
			assert(false == Empty());
			return pBuf[Len() - IndexFromEnd - 1];
		}

		/**
		* Returns nth-last element in the array.
		*
		* CAUTION: count of elements must be greater or equal to one.
		*/
		__forceinline T& Last(int32_t IndexFromEnd = 0)
		{
			assert(false == Empty());
			return pBuf[Len() - IndexFromEnd - 1];
		}

		/**
		* Returns index of the nth last element.
		*/
		__forceinline int32_t LastIndex(int32_t IndexFromEnd = 0)
		{
			assert(false == Empty());
			const int index = Length - IndexFromEnd - 1;
			IndexValidAssert(index);
			return index;
		}

		/**
		* Returns true if empty.
		*/
		__forceinline bool Empty() const
		{
			return Length == 0;
		}

		/**
		* Adds element by assignment.
		*
		* Returns pointer to element, BUT typically you should NOT save it, because it will be invalidated in the next realloc: 
		* (If you need to save pointers to vector element, rather use StaticVector).
		*/
		__forceinline T* Add(const T& Val);

		/**
		* The same as Add, but never performs reallocation.
		*/
		__forceinline T* AddBuffer(const T& Val);

		/**
		* Adds right element by moving
		*/
		__forceinline T* Add(T&& Val);

		/**
		* The same as Add, but never performs reallocation 
		*/
		__forceinline T* AddBuffer(T&& Val);

		/**
		* Adds elements and fills it with zeroes.
		*/
		T* AddZeroed(int32_t Count = 1);

		/**
		* The same as AddZeroed, but never performs reallocation.
		*/
		__forceinline T* AddBufferZeroed(int32_t Count = 1);

		/**
		* Adds elements by default constructing them.
		*/
		T* AddDefaulted(int32_t Count = 1);

		/**
		* The same as AddDefaulted by never performs reallocation.
		*/
		__forceinline T* AddBufferDefaulted(int32_t Count = 1);

		/**
		* Emplaces element.
		*/
		template<typename... ArgsType>
		__forceinline T* Emplace(ArgsType&&... Args);

		/**
		* The same as emplace by never performs reallocations.
		*/
		template<typename... ArgsType>
		__forceinline T* EmplaceBuffer(ArgsType&&... Args);

		/**
		* Emplaces element at the given position.
		*/
		template<typename... ArgsType>
		__forceinline T* EmplaceAt(int32_t Position, ArgsType&&... Args);

		/**
		* The same as EmplaceAt but never performs reallocations.
		*/
		template<typename... ArgsType>
		__forceinline T* EmplaceBufferAt(int32_t Position, ArgsType&&... Args);

		/**
		* Add the given number of uninitialized elements to the end.
		*/
		__forceinline T* AddUninitialized(int Count = 1);

		/**
		* The same as AddUninitialized, but never performs reallocations.
		*/
		__forceinline T* AddBufferUninitialized(int Count = 1);

		/**
		* Appends the given array at the end.
		*/
		T* Append(const T* pSource, int32_t Count)
		{
			assert(Count >= 0);
			const int32_t OldLength = Length;
			T* const ptr = AddUninitialized(Count);
			CopyConstructFrom(pSource, Count, OldLength);
			return ptr;
		};

		/**
		* Appends the given array by moving
		*/
		T* AppendMove(T* pSource, int32_t Count)
		{
			assert(Count >= 0);
			const int32_t OldLength = Length;
			T* const ptr = AddUninitialized(Count);
			MoveFrom(pSource, Count, OldLength);
			return ptr;
		}

		/**
		* Appends the given vector at the end.
		*/
		template<template<class> class OtherResizePolicy>
		__forceinline T* Append(const TVector<T, OtherResizePolicy>& Source)
		{
			return Append(Source.Data(), Source.Len());
		}

		/**
		* Appends the given vector at the end by moving.
		*/
		template<template<class> class OtherResizePolicy>
		T* Append(TVector<T, OtherResizePolicy>&& Source)
		{
			return AppendMove(Source.Data(), Source.Len());
		}

		/**
		* Appends the given initializer list at the end.
		*/
		T* Append(std::initializer_list<T> Source)
		{
			const int32_t OldLength = Length;
			T* const pAppended = AddUninitialized(Source.size());
			int i = 0;
			for (const T& val : Source)
			{
				new(pAppended + OldLength + i) T(val);
				i++;
			}
			return pAppended;
		}


		/**
		* Returns true if insert position is valid.
		* Position is different from index in that term that can be equal to Length
		* (maybe relevant for inserts).
		*/
		__forceinline bool IsInsertPositionValid(int32_t Position) const
		{
			return Position >= 0 && Position <= Length;
		}

		/**
		* Asserts that the insert position is valid.
		*/
		__forceinline void InsertPositionValidAssert(int32_t Position) const
		{
			assert(IsInsertPositionValid(Position));
		}

		/**
		* Returns true if the given range can be used inside the search functions.
		*/
		__forceinline bool IsSearchRangeValid(int32_t Start, int32_t End) const
		{
			// - We do NOT need to check that Start <= End, because otherwise the search loop will be aborted immediately;
			return End <= Length;
		}

		/**
		* Asserts that the given range can be used inside the search functions.
		*/
		__forceinline void SearchRangeValidAssert(int32_t Start, int32_t End) const
		{
			assert(IsSearchRangeValid(Start, End));
		}

		/**
		* Insert value before the give position.
		*
		* @UNIT_TESTED
		*/
		T* Insert(int32_t Position, const T& Val);

		/**
		* The same as Insert, but never performs reallocations.
		*/
		T* InsertBuffer(int32_t Position, const T& Val);

		/**
		* Insert value before the give position.
		*
		* @UNIT_TESTED
		*/
		T* Insert(int32_t Position, T&& Val);

		/**
		* The same as Insert, but never performs reallocations.
		*/
		T* InsertBuffer(int32_t Position, T&& Val);

		/**
		* Insert a given count of of zeroed elements before the given position.
		*/
		T* InsertZeroed(int32_t Position, int32_t Count = 1);

		/**
		* The same as InsertZeroed, but never performs reallocations.
		*/
		T* InsertBufferZeroed(int32_t Position, int32_t Count = 1);

		/**
		* Inserts a given count of default elements before the given position.
		*/
		T* InsertDefaulted(int32_t Position, int32_t Count = 1);

		/**
		* The same as InsertDefaulted, but never performs reallocations.
		*/
		T* InsertBufferDefaulted(int32_t Position, int32_t Count = 1);

		/**
		* Inserts a given count of elements by the given pointer before the given position.
		*/
		T* Insert(const T* pSource, int32_t Count, int32_t Position);

		/**
		* The same as Insert, but never performs reallocations.
		*/
		T* InsertBuffer(const T* pSource, int32_t Count, int32_t Position);

		/**
		* Inserts the given vector before the given position.
		*/
		template<template<class> class OtherResizePolicy>
		T* Insert(int32_t Position, const TVector<T, OtherResizePolicy>& Source);

		/**
		* The same as insert, but never performs reallocations.
		*/
		template<template<class> class OtherResizePolicy>
		T* InsertBuffer(int32_t Position, const TVector<T, OtherResizePolicy>& Source);

		/**
		* Inserts the given vector before the given position by moving.
		*/
		template<template<class> class OtherResizePolicy>
		T* Insert(int32_t Position, TVector<T, OtherResizePolicy>&& Source);

		/**
		* The same as Insert, but never performs reallocations.
		*/
		template<template<class> class OtherResizePolicy>
		T* InsertBuffer(int32_t Position, TVector<T, OtherResizePolicy>&& Source);

		/**
		* Inserts the given initializer list elements before the given position
		*/
		T* Insert(int32_t Position, std::initializer_list<T> InitList);

		/**
		* The same as Insert, but never performs reallocations.
		*/
		T* InsertBuffer(int32_t Position, std::initializer_list<T> InitList);

		/**
		* Inserts a given count of uninitialized values before the given position.
		*/
		T* InsertUninitialized(int32_t Position, int32_t Count = 1);

		/**
		* The same as InsertUninitialized, but never performs reallocations.
		*/
		T* InsertBufferUninitialized(int32_t Position, int32_t Count = 1);

		/**
		* Removes element at a given position.
		*
		* @UNIT_TESTED
		*/
		void RemoveAt(int32_t Position);

		/**
		* Same as RemoveAt, but never performs reallocations.
		*/
		__forceinline void RemoveBufferAt(int32_t Position)
		{
			return RemoveAt(Position);
		}

		/**
		* Remove a given count of element from a given position.
		*
		* @UNIT_TESTED
		*/
		void RemoveAt(int32_t Position, int32_t Count);

		/**
		* Same as RemoveAt, but never performs reallocations.
		*/
		__forceinline void RemoveBufferAt(int32_t Position, int32_t Count)
		{
			return RemoveAt(Position,Count);
		}

		/**
		* Removes a given value at the given index by swapping it with the last element.
		*
		* WARNING!!! The order is not preserved.
		*
		* @UNIT_TESTED
		*/
		void RemoveAtSwap(int32_t Position);

		/**
		* Same as RemoveAtSwap, but never performs reallocations.
		*/
		__forceinline void RemoveBufferAtSwap(int32_t Position)
		{
			return RemoveAtSwap(Position);
		}

		/**
		* Removes a given count of elements at the given index by swapping them with elements at the last.
		*
		* @UNIT_TESTED
		*/
		void RemoveAtSwap(int32_t Position, int32_t Count);

		/**
		* Same as RemoveAtSwap, but never performs reallocations
		*/
		__forceinline void RemoveBufferAtSwap(int32_t Position, int32_t Count)
		{
			return RemoveAtSwap(Position, Count);
		}

		/**
		* Remove the first found element with the given value.
		* Performs real removing, order is maintained the same.
		*
		* @returns: index of the removed element, INDEX_NONE if not found.
		*/
		int32_t RemoveFirst(const T& Val)
		{
			int32_t i = Find();
			if (i == INDEX_NONE) { return INDEX_NONE; }
			Remove(i);
			return i;
		}

		/**
		* Same as RemoveFirst, but never performs reallocations.
		*/
		__forceinline int32_t RemoveBufferFirst(const T& val)
		{
			return RemoveFirst(val);
		}

		/**
		* Removes the first found element by swapping with the last.
		*/
		int32_t RemoveSwapFirst(const T& Val)
		{
			int32_t i = Find(val);
			if (i == INDEX_NONE) { return INDEX_NONE; }
			RemoveAtSwap(i);
			return i;
		}

		/**
		* Same as RemoveSwapFirst, but never performs reallocations.
		*/
		__forceinline void RemoveBufferSwapFirst(const T& Val)
		{
			return RemoveSwapFirst(Val);
		}

		/**
		* Removes all elements equal to the given value.
		*
		* @returns: number of removed elements.
		* @UNIT_TESTED
		*/
		int32_t RemoveAll(const T& val)
		{
			// index of the destination
			int32_t i = 0;
			while (i < Length && pBuf[i] != val) { i++; }
			for (int j = i; j < Length; j++)
			{
				if (pBuf[j] != val)
				{
					pBuf[i] = std::move(pBuf[j]);
					i++;
				}
			}
			int32_t countRemoved = Length - i;
			Length = i;
			return countRemoved;
		}

		/**
		* Same as RemoveAll, but never performs reallocations.
		*/
		__forceinline void RemoveBufferAll(const T& val)
		{
			return RemoveAll(val);
		}

		/**
		* Removes all elements equal to the given value by swapping.
		*
		* WARNING!!! The order of elements in not maintained!!!.
		*
		* @returns: number of removed elements.
		* @UNIT_TESTED
		*/
		int32_t RemoveAllSwap(const T& Val)
		{
			const int32_t OldLength = Length;

			// index of the current "hole" (always points to element equal to the element we remove)
			int32_t i = 0;
			// Find the first hole
			i = Find_InRange(Val, i, Length);
			if (i == INDEX_NONE)
			{
				return 0;
			}
			// Find the first element and trim holes from the end.
			Length = FindBackwardByPredicate_InRange([&, Val](const T& PlasterCandidate) { return PlasterCandidate != Val; }, 0, Length);
			if (Length == INDEX_NONE)
			{
				Length = 0;
				// Entire vector consists of holes.
				return OldLength;
			}
			while (true)
			{
				pBuf[i] = std::move(pBuf[Length]);
				i++;
				
				if (i >= Length)
				{
					break;
				}

				i = Find_InRange(Val, i, Length);
				if (i == INDEX_NONE)
				{
					break;
				}
				Length = FindBackwardByPredicate_InRange([&, Val](const T& PlasterCandidate) { return PlasterCandidate != Val; }, 0, Length);

				if (i >= Length)
				{
					Length++;
					break;
				}

			}

			return OldLength - Length;
		}

		/**
		* Same as RemoveAllSwap, but never performs reallocations.
		*/
		__forceinline void RemoveBufferAllSwap(const T& Val)
		{
			return RemoveAllSwap(Val);
		}

		/**
		* Removes all elements that satisfy the given predicate by swapping.
		*
		* @see: RemoveAllSwap
		* @UNIT_TESTED
		*/
		template<class Pred>
		int32_t RemoveAllSwapPredicate(const Pred& P)
		{
			const int32_t OldLength = Length;

			// index of the current "hole" (always points to element equal to the element we remove)
			int32_t i = 0;
			// Find the first hole
			i = FindByPredicate_InRange(P, i, Length);
			if (i == INDEX_NONE)
			{
				return 0;
			}
			// Find the first element and trim holes from the end.
			Length = FindNotBackwardByPredicate_InRange(P, 0, Length);
			if (Length == INDEX_NONE)
			{
				Length = 0;
				// Entire vector consists of holes.
				return OldLength;
			}
			while (true)
			{
				pBuf[i] = std::move(pBuf[Length]);
				i++;

				if (i >= Length)
				{
					break;
				}

				i = FindByPredicate_InRange(P, i, Length);
				if (i == INDEX_NONE)
				{
					break;
				}
				Length = FindNotBackwardByPredicate_InRange(P, 0, Length);

				if (i >= Length)
				{
					Length++;
					break;
				}

			}

			return OldLength - Length;
		}

		/**
		* Same as RemoveAllSwapPredicate, but never performs reallocations.
		*/
		template<class Pred>
		__forceinline int32_t RemoveBufferAllSwapPredicate(const Pred& P)
		{
			return RemoveAllSwapPredication(P);
		}

		/**
		* Removes all elements that satisfy the given predicate.
		*
		* @see: RemoveAll
		* @UNIT_TESTED
		*/
		template<class Pred>
		int32_t RemoveAllPredicate(const Pred& P)
		{
			// index of the destination
			int32_t i = 0;
			while (i < Length && false == P(pBuf[i])) { i++; }
			for (int j = i + 1; j < Length; j++)
			{
				if (false == P(pBuf[j]))
				{
					pBuf[i] = std::move(pBuf[j]);
					i++;
				}
			}
			int32_t countRemoved = Length - i;
			Length = i;
			return countRemoved;
		}

		/**
		* Same as RemoveAllPredicate, but never performs reallocations.
		*/
		template<class Pred>
		__forceinline int32_t RemoveBufferAllPredicate(const Pred& P)
		{
			return RemoveAllPredicate(P);
		}

		/**
		* Get top element of the stack
		*/
		__forceinline const T& Top() const { return Last(); }
		__forceinline T& Top() { return Last(); }

		/**
		* Add on top of the vector
		*/
		__forceinline T* Push(const T& Val)
		{
			return Add(Val);
		}

		/**
		* Similar to Push, but never performs reallocations.
		*/
		__forceinline T* PushBuffer(const T& Val)
		{
			return AddBuffer(Val);
		}

		/**
		* Add on top of the vector
		*/
		__forceinline T* Push(T&& Val)
		{
			return Add(std::forward<T>(Val));
		}

		/**
		* Similar to Push, but never performs reallocations.
		*/
		__forceinline T* PushBuffer(T&& Val)
		{
			return AddBuffer(std::forward<T>(Val));
		}

		/**
		* Remove from top.
		*/
		__forceinline T Pop()
		{
			assert(false == Empty());
			T last = std::move(Last());
			--Length;
			return last;
		}

		/**
		* Same as Pop but never performs reallocations.
		*/
		__forceinline T PopBuffer()
		{
			return Pop();
		}

		/**
		* Resize to the given number of elements.
		*/
		void SetLength(int32_t DesiredLength);

		/**
		* The same as SetLength, but never performs reallocations.
		* But calls all necessary destructors and constructors as appropriate.
		*/
		__forceinline void SetBufferLength(int32_t DesiredLength);

		/**
		* The same as SetLength, but never performs reallocations
		* and never calls nor destructors, no constructors.
		*/
		__forceinline void SetBufferLengthUninitialized(int32_t DesiredLength);

		/**
		* The same as SetBufferLength, but never calls destructors.
		* (Constructors, however, are called as appropriate).
		*/
		__forceinline void SetBufferLength_NoDestruct(int32_t DesiredLength);


		/**
		* The same as SetBufferLength, but never calls constructors.
		* (Destructors, however, are called as appropriate).
		*/
		__forceinline void SetBufferLength_NoConstruct(int32_t DesiredLength);

		/**
		* Resize to the given length that is greater or equal to the current length.
		*
		* Elements with indices less than old length're kept intact.
		* Extra elements are initialized with default ctor.
		*/
		void ResizeGrow(int32_t DesiredLength);

		/*
		* Resize to the given length that is greater or equal to the current length.
		*
		* Elements with indices less than old length're kept intact.
		* Extra elements are NOT initialized.
		*/
		void ResizeGrow_Uninitialized(int32_t DesiredLength);

		/**
		* Reserve memory by growing.
		*
		* Extra elements are not initialized;
		* WARNING!!! New capacity must be NOT less than size!!
		*/
		void ReserveGrow(int32_t DesiredCapacity);

		/**
		* Shrink to fit minimal required size.
		*/
		void ShrinkToFit();

		/**
		* Adds element if it does not exist in the array.
		* This function searches the entire array until the element is found.
		*
		* @Returns: index of the added or existing element.
		*/
		int32_t AddUnique(const T& Val)
		{
			return AddUniqueImpl(Val);
		}

		/**
		* The same as AddUnique, but never performs reallocations.
		*/
		int32_t AddBufferUnique(const T& Val)
		{
			return AddBufferUniqueImpl(Val);
		}

		/**
		* Move version of the AddUnique function.
		*
		* @Returns: index of the added or existing element.
		*/
		int32_t AddUnique(T&& val)
		{
			return AddUniqueImpl(std::forward<T>(val));
		}

		/**
		* Move version of the AddBufferUnique function.
		*/
		int32_t AddBufferUnique(T&& val)
		{
			return AddBufferUniqueImpl(std::forward<T>(val));
		}

		/**
		* Non-stable sort.
		*/
		void Sort()
		{
			std::sort(begin(), end());
		}

		/**
		* Non-stable sort by a given predicate.
		*/
		template<class Pred>
		void Sort(const Pred& P)
		{
			std::sort(begin(), end(), P);
		}

		/**
		* Stable sort.
		*/
		void StableSort()
		{
			std::stable_sort(begin(), end());
		}

		/**
		* Stable sort by a given predicate.
		*/
		template<class Pred>
		void StableSort(const Pred& P)
		{
			std::stable_sort(begin(), end(), P);
		}

	private:
		template<class TVal>
		int32_t AddUniqueImpl(TVal Val)
		{
			int32_t i = Find(Val);
			if (i == INDEX_NONE) { return i; }
			Add(std::forward<TVal>(Val));
			return LastIndex();
		}

		template<class TVal>
		int32_t AddBufferUniqueImpl(TVal Val)
		{
			int32_t i = Find(Val);
			if (i == INDEX_NONE) { return i; }
			AddBuffer(std::forward<TVal>(Val));
			return LastIndex();
		}

		constexpr static ResizePolicy<T>      Policy{};
		
		__forceinline int32_t  CalcNewCapacity(int32_t DesiredLength) const;

		/**
		* Reallocs dynamic buffer and switches the buffer pointer to the buffer.
		* Reallocates buffer always from the old buffer
		* (the old buffer maybe either SBO or dynamic buffer);
		*/
		void ReAllocDynamicBuffer(int32_t ExactCapacity);

		/**
		* Setup initial dynamic buffer with the given desired length.
		*
		* Buffer is never initialized.
		*/
		void SetupDynamicBuffer_ForDesiredLength(int32_t DesiredLength);

		/**
		* Initializes length and sets up buffer (dynamic or SBO) for the desired length.
		*
		* Buffer is never initialized.
		*/
		void SetupBufferAndLength_ForDesiredLength(int32_t DesiredLength);

		/**
		* Sets up buffer (dynamic or SBO) for the desired length.
		*
		* Buffer is never initialized.
		*
		* Length is NEVER initialized!!!
		*/
		void SetupBuffer_ForDesiredLength(int32_t DesiredLength);

		// Setup initial dynamic buffer with the given exact capacity
		void SetupDynamicBuffer(int32_t ExactCapacity);

		/**
		* Call destructors for all elements.
		*/
		static void DestructAll(T* pBuf, int32_t Len);
		static void DestructRange(T* pBuf, int32_t StartIndex, int32_t EndIndex)
		{
			if constexpr(false == std::is_trivially_destructible<T>::value)
			{
				for (int i = StartIndex; i < EndIndex; i++)
				{
					pBuf[i].~T();
				}
			}
		}

		static void DefaultConstructAll(T* pBuf);
		static void DefaultConstructRange(T* pBuf, int32_t StartIndex, int32_t EndIndex)
		{
			if constexpr(false == std::is_trivially_default_constructible<T>::value)
			{
				for (int i = StartIndex; i < EndIndex; i++)
				{
					new(pBuf + i) T();
				}
			}
		}

		/**
		* Calls copy constructors for all elements.
		*/
		void CopyFrom(const T* pInSource, int32_t Count, int32_t Start = 0)
		{
			if constexpr(false == std::is_trivially_copy_assignable<T>::value)
			{
				for (int i = 0; i < Count; i++)
				{
					pBuf[Start + i] = pInSource[i];
				}
			}
			else
			{
				std::memcpy(pBuf + Start, pInSource, Count * sizeof(T));
			}
		}

		void CopyConstructFrom(const T* pInSource, int32_t Count, int32_t Start = 0)
		{
			if constexpr(false == std::is_trivially_copy_constructible<T>::value)
			{
				for (int i = 0; i < Count; i++)
				{
					new (pBuf + Start + i) T(pInSource[i]);
				}
			}
			else
			{
				std::memcpy(pBuf + Start, pInSource, sizeof(T) * Count);
			}
		}
		

		void CopyConstructTempl(const T& InTempl, int32_t Count, int32_t Start = 0)
		{
			static_assert(std::is_copy_constructible<T>::value);
			for (int i = 0; i < Count; i++)
			{
				new (pBuf + Start + i) T(InTempl);
			}
		}

		void MoveFrom(T* pSource, int32_t Count, int32_t Start = 0)
		{
			if constexpr(false == std::is_trivially_move_constructible<T>::value)
			{
				for (int i = 0; i < Count; i++)
				{
					new (pBuf + Start + i) T(std::move(pSource[i]));
				}
			}
			else
			{
				memmove(pBuf + Start, pSource, sizeof(T) * Count);
			}
		}

		void MoveShiftRightFrom(T* pOldBuf, int32_t Position, int32_t ShiftedPosition, int32_t Count)
		{
			assert(pOldBuf);
			if constexpr(false == std::is_trivially_move_assignable<T>::value)
			{
				for (int i = Count; --i >= 0; )
				{
					pBuf[ShiftedPosition + i] = std::move(pOldBuf[Position + i]);
				}
			}
			else
			{
				memmove(pBuf + ShiftedPosition, pOldBuf + Position, sizeof(T) * Count);
			}
		}
		void MoveShiftLeftFrom(T* pOldBuf, int32_t Position, int32_t ShiftedPosition, int32_t Count)
		{
			assert(pOldBuf);
			if constexpr(false == std::is_trivially_move_assignable<T>::value)
			{
				for (int i = 0; i < Count; i++)
				{
					pBuf[ShiftedPosition + i] = std::move(pOldBuf[Position + i]);
				}
			}
			else
			{
				memmove(pBuf + ShiftedPosition, pOldBuf + Position, sizeof(T) * Count);
			}
		}

		/**
		* Invalidates the objects.
		*
		* Typically used for move operation implementations.
		*/
		void Invalidate();
		

		template<template<class> class OtherResizePolicy>
		void CopyConstruct(const TVector<T, OtherResizePolicy>& Other);

		template<template<class> class OtherResizePolicy>
		void MoveConstruct(TVector<T, OtherResizePolicy>&& Other);

		template<class T, template<class> class OtherResizePolicy>
		void MoveAssign(TVector<T, OtherResizePolicy>&& Other)
		{
			// We must destruct extra elements (Before assigning new length!!!)
			DestructRange(pBuf, Other.Length, Length);

			// Other uses dynamic buf:
			if (Other.pDynamicBuf == Other.pBuf)
			{
				// Destroying the rest of the elements
				DestructRange(pBuf, 0, Length);
				if (pDynamicBuf)
				{
					free(pDynamicBuf);
				}

				// Move buffer
				pDynamicBuf = Other.pDynamicBuf;
				pBuf = pDynamicBuf;
				MaxLength = Other.MaxLength;

				// Length should be updated properly (it's still not updated for this particular case).
				Length = Other.Length;

				Other.Invalidate();
				return;
			}

			// Assign new length (we must do it after handling the "other is dynamic buffer" case,
			// because the old length is needed there to properly destruct the buffer elements.
			Length = Other.Length;

			// We must destroy dynamic buffer if it's incapable to store its length or SBO will be used
			const bool bUseSBO = Other.Length < ResizePolicy<T>::SBO_LENGTH;
			if (pDynamicBuf && (bUseSBO || Other.Length <= MaxLength))
			{
				free(pDynamicBuf);
				pDynamicBuf = nullptr;
			}

			// Other uses SBO:
			// We must prepare dynamic buffer if this SBO is NOT capable to store other's length
			if (pDynamicBuf && (false == bUseSBO))
			{
				SetupDynamicBuffer_ForDesiredLength(Other.Length);
			}
			MoveFrom(Other.pBuf, Other.Length, 0);

			// Warning!!! We should NOT perform invalidation of the Other here manually,
			// because the elements will automatically invalidated during the move operation.
		}
		
		int32_t                     Length              = 0;
		unsigned char               SmallBuf            [ResizePolicy<T>::SBO_LENGTH * sizeof(T)];
		T*                          pDynamicBuf         = nullptr;
		T*                          pBuf                = static_cast<T*>(static_cast<void*>(SmallBuf));
		int32_t                     MaxLength           = ResizePolicy<T>::SBO_LENGTH;
	};

	template<class T, template<class> class ResizePolicy>
	__forceinline const T* begin(const TVector<T, ResizePolicy>& V) { return V.begin(); }

	template<class T, template<class> class ResizePolicy>
	__forceinline T* begin(TVector<T, ResizePolicy>& V) { return V.begin(); }

	template<class T, template<class> class ResizePolicy>
	__forceinline const T* end(const TVector<T, ResizePolicy>& V) { return V.end(); }

	template<class T, template<class> class ResizePolicy>
	__forceinline T* end(TVector<T, ResizePolicy>& V) { return V.end(); }

	/**
	* Operator<< oveload.
	*
	* IMPLEMENTATION NOTE: Because of serialization archive usage 
	* we must exclude non-output stream types from the compilation by the std::enable_if.
	*/
	template
	<
		class Strm, class T, 
		template<class> class ResizePolicy, 
		typename = typename std::enable_if<IsOutputStream<typename Strm>::Value>::type
	>
	Strm& operator<<(Strm& strm, const TVector<T, ResizePolicy>& V)
	{
		Write(strm, V);
		return strm;
	}


/**
* Returns string with memory info.
*/
template<class T, template<class> class ResizePolicy>
std::string* AppendMemoryInfoString(std::string* pOutString, const TVector<T, ResizePolicy>& V)
{	
	BOOST_ASSERT(pOutString);

	// Length
	pOutString->append("Len=");
	pOutString->append(V.Len());

	// Capacity
	pOutString->append("; MaxLen=");
	pOutString->append(V.MaxLen());

	// TotalBytes
	pOutString->append("; TotalBytes=");
	pOutString->append(V.CountTotalBytes());

	// MinumumBytes
	pOutString->append("; MinimumBytes=");
	pOutString->append(V.CountMinimumSizeBytes());

	return pOutString;
}

/**
* Returns string with extra debug info
*/
template<class T, template<class> class ResizePolicy>
std::string* AppendExtraDebugInfoString(std::string* pOutString, const TVector<T, ResizePolicy>& V)
{
	BOOST_ASSERT(pOutString);

	// buffer
	pOutString->append("pBuf=");
	pOutString->append(V.pBuf ? V.pBuf : "nullptr");

	// dynamic buffer
	pOutString->append("; pDynBuf=");
	pOutString->append(V.pDynamicBuf ? V.pDynamicBuf : "nullptr");

	// resize policy
	pOutString->append("; resizePolicy=");
	pOutString->append(ResizePolicy<T>::Name);

	// memory info
	pOutString->append("; ");
	AppendMemoryInfoString(pOutString, V);

	// info about serialization
	int32_t const countSerializeBytes = V.CountSerializeBytes();
	pOutString->append("; countSerializeBytes=");
	pOutString->append(std::to_string(countSerializeBytes));

	return pOutString;
}

/**
* Conversion to string with extra debug info
*/
template<class T, template<class> class ResizePolicy>
std::string ToDebugString(const TVector<T, ResizePolicy>& V, const char sep = ' ')
{
	std::string result;
	result.append("{");

	// Contents
	result.append(ToString(V));

	// Extra debug info
	result.append(";");
	AppendExtraDebugInfoString(&result, V);

	result.append("}");
	return result;
}


template<class Strm, class T, template<class> class ResizePolicy>
void Write(Strm& strm, const TVector<T, ResizePolicy>& V, char sep = ' ')
{
	for (int i = 0; i < V.Len(); i++)
	{
		if (i > 0) { strm << " "; }
		strm << V[i];
	}
}

template<class Strm, class T, template<class> class ResizePolicy>
void WriteLn(Strm& strm, const TVector<T, ResizePolicy>& V, char sep = ' ')
{
	Write(Strm, V, sep);
	strm << std::endl;
}

template<class Strm, class T, template<class> class ResizePolicy>
void WriteDebug(Strm& strm, const TVector<T, ResizePolicy>& V, char sep = ' ')
{
	strm << ToDebugString(Strm, V, sep);
}

template<class Strm, class T, template<class> class ResizePolicy>
void WriteDebugLn(Strm& strm, const TVector<T, ResizePolicy>& V, char sep = ' ')
{
	WriteDebug(Strm, V, sep);
	strm << std::endl;
}

/**
* Conversion to string
*/
template<class T, template<class> class ResizePolicy>
std::string ToString(const TVector<T, ResizePolicy>& V, const char sep = ' ')
{
	std::string result;
	for (int i = 0; i < V.Len(); i++)
	{
		if (i > 0) { result.append(sep); }
		result.append(std::to_string(V[i]));
	}
	return result;
}

template<class T, template<class> class ResizePolicy>
TVector<T, ResizePolicy>::TVector(const TVector<T, ResizePolicy>& Other)
{
	CopyConstruct(Other);
}

template<class T, template<class> class ResizePolicy>
template<template<class> class OtherResizePolicy>
TVector<T, ResizePolicy>::TVector(const TVector<T, OtherResizePolicy>& Other)
{
	CopyConstruct(Other);
}
		
template<class T, template<class> class ResizePolicy>
template<template<class> class OtherResizePolicy>
void TVector<T,ResizePolicy>::CopyConstruct(const TVector<T, OtherResizePolicy>& Other)
{
	Length = Other.Length;
	SetupBuffer_ForDesiredLength(Other.Len());
	CopyConstructFrom(Other.Data(), Other.Len());
}

template<class T, template<class> class ResizePolicy>
TVector<T, ResizePolicy>::TVector(TVector<T, ResizePolicy>&& Other)
{
	MoveConstruct(std::move(Other));
}

template<class T, template<class> class ResizePolicy>
template<template<class> class OtherResizePolicy>
void TVector<T, ResizePolicy>::MoveConstruct(TVector<T, OtherResizePolicy>&& Other)
{
	Length = Other.Length;
	// Other uses dynamic buf
	if (Other.pDynamicBuf == Other.pBuf)
	{
		pDynamicBuf = Other.pDynamicBuf;
		pBuf = pDynamicBuf;
		MaxLength = Other.MaxLength;

		Other.Invalidate();
		return;
	}

	// Other uses SBO:
	// We must prepare dynamic buffer if this SBO is NOT capable to store other's length
	if (Other.Length > ResizePolicy<T>::SBO_LENGTH)
	{
		SetupDynamicBuffer_ForDesiredLength(Other.Length);
	}
	// Then we must move from the other buffer to this buffer
	MoveFrom(Other.pBuf, Other.Length);
}

template<class T, template<class> class ResizePolicy>
template<template<class> class OtherResizePolicy>
TVector<T, ResizePolicy>::TVector(TVector<T, OtherResizePolicy>&& Other)
{
	MoveConstruct(std::move(Other));
}

template<class T, template<class> class ResizePolicy>
TVector<T,ResizePolicy>& TVector<T,ResizePolicy>::operator=(const TVector<T, ResizePolicy>& Other)
{
	// @TODO
	return *this;
}

template<class T, template<class> class ResizePolicy>
template<template<class> class OtherResizePolicy>
typename TVector<T, ResizePolicy>::ThisType& TVector<T, ResizePolicy>::operator=(const TVector<T, OtherResizePolicy>& Other)
{
	if (Length < Other.Length)
	{
		SetupBuffer_ForDesiredLength(Other.Length);
	}
	CopyAssignFrom(Other);
	Length = Other.Length;
	return *this;
}

template<class T, template<class> class ResizePolicy>
void TVector<T, ResizePolicy>::Invalidate()
{
	pDynamicBuf     = nullptr;
	pBuf            = nullptr;
	Length          = 0;
}

template<class T, template<class> class ResizePolicy>
TVector<T,ResizePolicy>& TVector<T,ResizePolicy>::operator=(TVector<T, ResizePolicy>&& Other)
{
	MoveAssign(std::move(Other));
	return *this;
}

template<class T, template<class> class ResizePolicy>
template<template<class> class OtherResizePolicy>
typename TVector<T, ResizePolicy>::ThisType& TVector<T, ResizePolicy>::operator=(TVector<T, OtherResizePolicy>&& Other)
{
	MoveAssign(std::move(Other));
	return *this;
}

template<class T, template<class> class ResizePolicy>
TVector<T, ResizePolicy>::TVector(int32_t InitialLength) :
	Length(InitialLength)
{
	SetupBuffer_ForDesiredLength(InitialLength);
}

template<class T, template<class> class ResizePolicy>
TVector<T, ResizePolicy>::TVector(int32_t InitialLength, EForceInit) :
	Length(InitialLength)
{
	SetupBuffer_ForDesiredLength(InitialLength);
	DefaultConstructAll();
}
	
template<class T, template<class> class ResizePolicy>
TVector<T, ResizePolicy>::TVector(int32_t InitialLength, const T& Templ) :
	Length(InitialLength)
{
	SetupBuffer_ForDesiredLength(InitialLength);
	CopyConstructTempl(Templ, Length);
}


template<class T, template<class> class ResizePolicy>
TVector<T, ResizePolicy>::TVector(std::initializer_list<T> InitList) :
	Length(InitList.size())
{
	SetupBuffer_ForDesiredLength(Length);
	auto itSource = InitList.begin();
	for (int i = 0; i < Length; i++)
	{
		pBuf[i] = *itSource;
		++itSource;
	}
}

template<class T, template<class> class ResizePolicy>
TVector<T, ResizePolicy>::TVector(const T* Ptr, int32_t InCount) :
	Length(InCount)
{
	assert(Ptr);
	SetupBuffer_ForDesiredLength(Length);
	CopyConstructFrom(Ptr, InCount);
}

template<class T, template<class> class ResizePolicy>
template<template<class> class OtherResizePolicy>
bool TVector<T, ResizePolicy>::operator==(const TVector<T, OtherResizePolicy>& Other) const
{
	if (Length != Other.Length)
	{
		return false;
	}
	for (int i = 0; i < Length; i++)
	{
		if(pBuf[i] != Other.pBuf[i]) return false;
	}
	return true;
}

template<class T, template<class> class ResizePolicy>
template<template<class> class OtherResizePolicy>
bool TVector<T,ResizePolicy>::operator!=(const TVector<T, OtherResizePolicy>& Other) const
{
	if (Length != Other.Length)
	{
		return true;
	}
	for (int i = 0; i < Length; i++)
	{
		if (pBuf[i] != Other.pBuf[i]) return true;
	}
	return false;
}

template<class T, template<class> class ResizePolicy>
void TVector<T, ResizePolicy>::Clear()
{
	DestructAll(pBuf, Length);
	Length = 0;
}


template<class T, template<class> class ResizePolicy>
void TVector<T, ResizePolicy>::SetBufferLength(int32_t DesiredLength)
{
	assert(DesiredLength <= MaxLength);
	// If desired length is bigger than current, then nothing will be destroyed,
	// so function will work correct.
	// The same for the DefaultConstructRange.
	DestructRange(pBuf, DesiredLength, Length);
	DefaultConstructRange(pBuf, Length, DesiredLength);
	Length = DesiredLength;
}

template<class T, template<class> class ResizePolicy>
void TVector<T, ResizePolicy>::SetBufferLengthUninitialized(int32_t DesiredLength)
{
	assert(DesiredLength <= MaxLength);
	Length = DesiredLength;
}
	
template<class T, template<class> class ResizePolicy>
void TVector<T, ResizePolicy>::SetBufferLength_NoDestruct(int32_t DesiredLength)
{
	assert(DesiredLength <= MaxLength);
	DefaultConstructRange(pBuf, Length, DesiredLength);
	Length = DesiredLength;
}

template<class T, template<class> class ResizePolicy>
void TVector<T, ResizePolicy>::SetBufferLength_NoConstruct(int32_t DesiredLength)
{
	assert(DesiredLength <= MaxLength);
	DestructRange(pBuf, DesiredLength, Length);
	Length = DesiredLength;
}


template<class T, template<class> class ResizePolicy>
void TVector<T, ResizePolicy>::SetLength(int32_t DesiredLength)
{
	if (DesiredLength > Length)
	{
		ResizeGrow(DesiredLength);
	}
	else
	{
		SetBufferLength_NoConstruct(DesiredLength);
	}
}

template<class T, template<class> class ResizePolicy>
void TVector<T, ResizePolicy>::ResizeGrow(int32_t DesiredLength)
{
	int32_t const OldLen = Length;
	ResizeGrow_Uninitialized(DesiredLength);
	DefaultConstructRange(pBuf, OldLen, DesiredLength);
}

template<class T, template<class> class ResizePolicy>
void TVector<T, ResizePolicy>::ResizeGrow_Uninitialized(int32_t DesiredLength)
{
	if (DesiredLength > MaxLength)
	{
		ReserveGrow(DesiredLength);
	}
	Length = DesiredLength;
}

template<class T, template<class> class ResizePolicy>
void TVector<T, ResizePolicy>::ReserveGrow(int32_t DesiredCapacity)
{
	assert(DesiredCapacity >= Len());
	if(DesiredCapacity > ResizePolicy<T>::SBO_LENGTH)
	{
		MaxLength = CalcNewCapacity(DesiredCapacity);
		// COMMENT: We will never use small buffer again if dynamic is ever used
		ReAllocDynamicBuffer(MaxLength);
	}
	// COMMENT: If MaxLength is less or equal to the small buffer length, max length is valid (no update needed).
}

template<class T, template<class> class ResizePolicy>
void TVector<T, ResizePolicy>::ShrinkToFit()
{
	if (Length > ResizePolicy<T>::SBO_LENGTH)
	{
		MaxLength = CalcNewCapacity(Length);
		ReAllocDynamicBuffer(MaxLength);
	}
	else
	{
		// SBO is used; 
		// if dynamic buffer was used before, we must move its content to the SBO
		// and switch to the small buffer
		if (pDynamicBuf)
		{
			MoveFrom(pDynamicBuf, Len());
			DestructAll(pDynamicBuf, Len());
			if (pDynamicBuf)
			{
				free(pDynamicBuf);
			}

			MaxLength = ResizePolicy<T>::SBO_LENGTH;
			pBuf = static_cast<T*>(static_cast<void *>(SmallBuf));
			pDynamicBuf = nullptr;
		}
	}
}

template<class T, template<class> class ResizePolicy>
T* TVector<T, ResizePolicy>::AddZeroed(int32_t Count)
{
	T* const ptr = AddUninitialized(Count);
	UT::ZeroBytes(ptr, sizeof(T) * Count);
	return ptr;
}

template<class T, template<class> class ResizePolicy>
T* TVector<T, ResizePolicy>::AddBufferZeroed(int32_t Count)
{
	T* const ptr = AddBufferUninitialized(Count);
	UT::ZeroBytes(ptr, sizeof(T) * Count);
	return ptr;
}

template<class T, template<class> class ResizePolicy>
T* TVector<T, ResizePolicy>::AddDefaulted(int32_t Count)
{
	const int32_t OldLength = Length;
	T* const ptr = AddUninitialized(Count);
	DefaultConstructRange(ptr, OldLength, Length);
	return ptr;
}

template<class T, template<class> class ResizePolicy>
T* TVector<T, ResizePolicy>::AddBufferDefaulted(int32_t Count)
{
	const int32_t OldLength = Length;
	T* const ptr = AddBufferUninitialized(Count);
	DefaultConstructRange(ptr, OldLength, Length);
	return ptr;
}

template<class T, template<class> class ResizePolicy>
T* TVector<T, ResizePolicy>::Add(const T& Val)
{
	T* const ptr = AddUninitialized();
	new(ptr) T(Val);
	return ptr;
}

template<class T, template<class> class ResizePolicy>
T* TVector<T, ResizePolicy>::AddBuffer(const T& Val)
{
	T* const ptr = AddBufferUninitialized();
	new(ptr) T(val);
	return ptr;
}

template<class T, template<class> class ResizePolicy>
T* TVector<T, ResizePolicy>::Add(T&& Val)
{
	T* const ptr = AddUninitialized();
	new (ptr) T(std::forward<T>(Val));
	return ptr;
}

template<class T, template<class> class ResizePolicy>
T* TVector<T, ResizePolicy>::AddBuffer(T&& Val)
{
	T* const ptr = AddBufferUninitialized();
	new (ptr) T(std::forward<T>(Val));
	return ptr;
}

template<class T, template<class> class ResizePolicy>
template<typename... ArgsType>
T* TVector<T, ResizePolicy>::Emplace(ArgsType&&... Args)
{
	T* const ptr = AddUninitialized();
	new(ptr) T(std::forward<ArgsType>(Args)...);
	return ptr;
}

template<class T, template<class> class ResizePolicy>
template<typename... ArgsType>
T* TVector<T, ResizePolicy>::EmplaceBuffer(ArgsType&&... Args)
{
	T* const ptr = AddBufferUninitialized();
	new(ptr) T(std::forward<ArgsType>(Args)...);
	return ptr;
}

template<class T, template<class> class ResizePolicy>
T* TVector<T, ResizePolicy>::AddUninitialized(int Count)
{
	// Save old length
	const int32_t OldLength = Length;
	const int32_t DesiredLength = Length + Count;
	ResizeGrow_Uninitialized(DesiredLength);
	// Compute end ptr (WARNING!!! Never move this line before the ResizeGrow!!!)
	T* const ptr = pBuf + OldLength;

	return ptr;
}

template<class T, template<class> class ResizePolicy>
T* TVector<T, ResizePolicy>::AddBufferUninitialized(int Count)
{
	T* const ptr = pBuf + Length;
	SetBufferLength_NoDestruct(Length + Count);
	return ptr;
}

template<class T, template<class> class ResizePolicy>
template<typename... ArgsType>
T* TVector<T, ResizePolicy>::EmplaceAt(int32_t Position, ArgsType&&... Args)
{
	InsertPositionValidAssert(Position);
	T* const ptr = InsertUninitialized(Position);
	new(ptr) T(std::forward<Args>(val));
	return ptr;
}

template<class T, template<class> class ResizePolicy>
template<typename... ArgsType>
T* TVector<T, ResizePolicy>::EmplaceBufferAt(int32_t Position, ArgsType&&... Args)
{
	InsertPositionValidAssert(Position);
	T* const ptr = InsertBufferUninitialized(Position);
	new(ptr) T(std::forward<Args>(val));
	return ptr;
}

template<class T, template<class> class ResizePolicy>
T* TVector<T, ResizePolicy>::Insert(int32_t Position, const T& Val)
{
	InsertPositionValidAssert(Position);
	T* const ptr = InsertUninitialized(Position);
	new(ptr) T(Val);
	return ptr;
}

template<class T, template<class> class ResizePolicy>
T* TVector<T, ResizePolicy>::InsertBuffer(int32_t Position, const T& Val)
{
	InsertPositionValidAssert(Position);
	T* const ptr = InsertBufferUninitialized(Position);
	new(ptr) T(Val);
	return ptr;
}

template<class T, template<class> class ResizePolicy>
T* TVector<T, ResizePolicy>::Insert(int32_t Position, T&& Val)
{
	InsertPositionValidAssert(Position);
	T* const ptr = InsertUninitialized(Position);
	new(ptr) T(std::move(Val));
	return ptr;
}

template<class T, template<class> class ResizePolicy>
T* TVector<T, ResizePolicy>::InsertBuffer(int32_t Position, T&& Val)
{
	InsertPositionValidAssert(Position);
	T* const ptr = InsertBufferUninitialized(Position);
	new(ptr) T(std::move(Val));
	return ptr;
}

template<class T, template<class> class ResizePolicy>
T* TVector<T, ResizePolicy>::InsertZeroed(int32_t Position, int32_t Count)
{
	InsertPositionValidAssert(Position);
	T* const ptr = InsertUninitialized(Position, Count);
	UT::ZeroBytes(ptr, sizeof(T) * Count);
	return ptr;
}

template<class T, template<class> class ResizePolicy>
T* TVector<T, ResizePolicy>::InsertBufferZeroed(int32_t Position, int32_t Count)
{
	InsertPositionValidAssert(Position);
	T* const ptr = InsertBufferUninitialized(Position, Count);
	UT::ZeroBytes(ptr, sizeof(T) * Count);
	return ptr;
}

template<class T, template<class> class ResizePolicy>
T* TVector<T, ResizePolicy>::InsertDefaulted(int32_t Position, int32_t Count)
{
	InsertPositionValidAssert(Position);
	T* const ptr = InsertUninitialized(Position, Count);
	DefaultConstructRange(pBuf, Position, Position + Count);
	return ptr;
}

template<class T, template<class> class ResizePolicy>
T* TVector<T, ResizePolicy>::InsertBufferDefaulted(int32_t Position, int32_t Count)
{
	InsertPositionValidAssert(Position);
	T* const ptr = InsertBufferUninitialized(Position, Count);
	DefaultConstructRange(pBuf, Position, Position + Count);
	return ptr;
}

template<class T, template<class> class ResizePolicy>
T* TVector<T, ResizePolicy>::Insert(const T* pSource, int32_t Count, int32_t Position)
{
	InsertPositionValidAssert(Position);
	T* const ptr = InsertUninitialized(Position, Count);
	CopyConstructFrom(pSource, Count, Position);
	return ptr;
}

template<class T, template<class> class ResizePolicy>
T* TVector<T, ResizePolicy>::InsertBuffer(const T* pSource, int32_t Count, int32_t Position)
{
	InsertPositionValidAssert(Position);
	T* const ptr = InsertBufferUninitialized(Position, Count);
	CopyConstructFrom(pSource, Count, Position);
	return ptr;
}

template<class T, template<class> class ResizePolicy>
template<template<class> class OtherResizePolicy>
T* TVector<T, ResizePolicy>::Insert(int32_t Position, TVector<T, OtherResizePolicy>&& Source)
{
	InsertPositionValidAssert(Position);
	T* const ptr = InsertUninitialized(Position, Count);
	MoveFrom(Source.Data(), Source.Len(), Position);
	return ptr;
}

template<class T, template<class> class ResizePolicy>
template<template<class> class OtherResizePolicy>
T* TVector<T, ResizePolicy>::InsertBuffer(int32_t Position, TVector<T, OtherResizePolicy>&& Source)
{
	InsertPositionValidAssert(Position);
	T* const ptr = InsertBufferUninitialized(Position, Count);
	MoveFrom(Source.Data(), Source.Len(), Position);
	return ptr;
}

template<class T, template<class> class ResizePolicy>
template<template<class> class OtherResizePolicy>
T* TVector<T, ResizePolicy>::Insert(int32_t Position, const TVector<T, OtherResizePolicy>& Source)
{
	return Insert(Source.Data(), Source.Len(), Position);
}

template<class T, template<class> class ResizePolicy>
template<template<class> class OtherResizePolicy>
T* TVector<T, ResizePolicy>::InsertBuffer(int32_t Position, const TVector<T, OtherResizePolicy>& Source)
{
	return InsertBuffer(Source.Data(), Source.Len(), Position);
}

template<class T, template<class> class ResizePolicy>
T* TVector<T, ResizePolicy>::Insert(int32_t Position, std::initializer_list<T> InitList)
{
	InsertPositionValidAssert(Position);
	T* const ptr = InsertUninitialized(Position, InitList.size());
	int32_t i = 0;
	for (const T& val : InitList)
	{
		new (ptr + i) T(val);
		i++;
	}
	return ptr;
}

template<class T, template<class> class ResizePolicy>
T* TVector<T, ResizePolicy>::InsertBuffer(int32_t Position, std::initializer_list<T> InitList)
{
	InsertPositionValidAssert(Position);
	T* const ptr = InsertBufferUninitialized(Position, InitList.size());
	int32_t i = 0;
	for (const T& val : InitList)
	{
		new (ptr + i) T(val);
		i++;
	}
	return ptr;
}

template<class T, template<class> class ResizePolicy>
T* TVector<T, ResizePolicy>::InsertUninitialized(int32_t Position, int32_t Count)
{
	InsertPositionValidAssert(Position);

	const int32_t ExpandedLength = Length + Count;

	// Old buffer: always points to buffer that was set before the operation
	T* pOldBuf = pBuf;
	// Prefix of buffer that is NOT shifted
	const int32_t UnshiftedPrefixLength = Position;
	const bool bRealloc = (MaxLength < ExpandedLength);
	if (bRealloc)
	{
		SetupDynamicBuffer_ForDesiredLength(ExpandedLength);

		// Copy unshifted prefix
		MoveFrom(pOldBuf, UnshiftedPrefixLength);
		DestructRange(pOldBuf, 0, UnshiftedPrefixLength);
	}
		
	const int32_t CountToShift = ExpandedLength - Position;
	MoveShiftRightFrom(pOldBuf, Position, Position + Count, CountToShift);
		
	// Destroy postfix, if buffer was reallocated
	if (bRealloc)
	{
		DestructRange(pOldBuf, UnshiftedPrefixLength, Length);
	}

	// Perform the shift
	Length = ExpandedLength;
	const auto pInserted = pBuf + Position;
	return pInserted;
}

template<class T, template<class> class ResizePolicy>
T* TVector<T, ResizePolicy>::InsertBufferUninitialized(int32_t Position, int32_t Count)
{
	InsertPositionValidAssert(Position);

	const int32_t ExpandedLength = Length + Count;
	assert(ExpandedLength <= MaxLength);

	const int32_t CountToShift = ExpandedLength - Position;
	MoveShiftRightFrom(pOldBuf, Position, Position + Count, CountToShift);

	return pBuf + Position;
}

template<class T, template<class> class ResizePolicy>
void TVector<T, ResizePolicy>::RemoveAt(int32_t Position)
{
	IndexValidAssert(Position);

	const int32_t CountToShift = (Length - Position - 1);
	MoveShiftLeftFrom(pBuf, Position + 1, Position, CountToShift);
	--Length;
}

template<class T, template<class> class ResizePolicy>
void TVector<T, ResizePolicy>::RemoveAt(int32_t Position, int32_t Count)
{
	IndexValidAssert(Position + Count - 1);
	const int32_t CountToShift = (Length - Position - Count);
	MoveShiftLeftFrom(pBuf, Position + Count, Position, CountToShift);
	Length -= Count;
}

template<class T, template<class> class ResizePolicy>
void TVector<T, ResizePolicy>::RemoveAtSwap(int32_t Position)
{
	IndexValidAssert(Position);
	const int32_t iLast = LastIndex();
	if (iLast != Position)
	{
		pBuf[Position] = std::move(pBuf[iLast]);
	}
	--Length;
}

template<class T, template<class> class ResizePolicy>
void TVector<T, ResizePolicy>::RemoveAtSwap(int32_t Position, int32_t Count)
{
	const int32_t iStartToRemove = Length - Count;
	RangeValidAssert(Position, Position + Count);
	MoveShiftLeftFrom(pBuf, iStartToRemove, Position, Count);
	Length -= Count;
}

template<class T, template<class> class ResizePolicy>
int32_t TVector<T, ResizePolicy>::CalcNewCapacity(int32_t DesiredLength) const
{
	return Policy.CalcNewCapacity(DesiredLength);
}

template<class T, template<class> class ResizePolicy>
void TVector<T, ResizePolicy>::ReAllocDynamicBuffer(int32_t ExactCapacity)
{
	T* const pOldBuf = pBuf;
	T* const pOldDynamicBuf = pDynamicBuf;
	SetupDynamicBuffer(ExactCapacity);
	MoveFrom(pOldBuf, Len());
	DestructAll(pOldBuf, Len());
	if (pOldDynamicBuf)
	{
		free(pOldDynamicBuf);
	}
}

template<class T, template<class> class ResizePolicy>
void TVector<T, ResizePolicy>::SetupDynamicBuffer_ForDesiredLength(int32_t DesiredLength)
{
	MaxLength = CalcNewCapacity(DesiredLength);
	return SetupDynamicBuffer(MaxLength);
}

template<class T, template<class> class ResizePolicy>
void TVector<T, ResizePolicy>::SetupBufferAndLength_ForDesiredLength(int32_t DesiredLength)
{
	Length = DesiredLength;
	SetupBuffer_ForDesiredLength(DesiredLength);
}

template<class T, template<class> class ResizePolicy>
void TVector<T, ResizePolicy>::SetupBuffer_ForDesiredLength(int32_t DesiredLength)
{
	if (DesiredLength <= ResizePolicy<T>::SBO_LENGTH)
	{
		MaxLength = ResizePolicy<T>::SBO_LENGTH;
		pBuf = static_cast<T*>(static_cast<void*>(SmallBuf));
	}
	else
	{	
		SetupDynamicBuffer_ForDesiredLength(DesiredLength);
	}
}

template<class T, template<class> class ResizePolicy>
void TVector<T, ResizePolicy>::SetupDynamicBuffer(int32_t ExactCapacity)	
{
	pDynamicBuf = static_cast<T*>(malloc(sizeof(T) * ExactCapacity));
	pBuf = pDynamicBuf;
}

template<class T, template<class> class ResizePolicy>
void TVector<T, ResizePolicy>::DestructAll(T* pBuf, int32_t Len)
{
	DestructRange(pBuf, 0, Len);
}

template<class T, template<class> class ResizePolicy>
void TVector<T, ResizePolicy>::DefaultConstructAll(T* pBuf)
{
	DefaultConstructRange(0, Length);
}

template<class T, template<class> class ResizePolicy>
TVector<T, ResizePolicy>::~TVector()
{
	DestructAll(pBuf, Length);
	if (pDynamicBuf) 
	{
		free(pDynamicBuf);
		pDynamicBuf = nullptr;
	}
}

/**
* TODO:
* 1. Change unsigned int to int32 everywhere (to prevent signed/unsigned mismatch warning) (+FIXED)
* 2. Use move vs. copy everywhere, where possible.
* 3. POD type optimization.
* 3.1. Optimize copies from initializer lists.
* 4. Serialization: Boost::serialization
* 5. SBO:
* 5.1. Maybe it's better to pass SBO size as an extra template paramter that defaults to 32
* (it could be helpful to prevent stack overflows by specifying a lesser SBO size).
* 6. Add/Insert operations:
* 6.1. Provide special NON-reserve versions.
* 7. Handle special cases for pointer arrays.
* 7.1. Serialization/CountSerializeBytes/CountTotalBytes
* 7.2. Search
* 7.2. Sort
* 8. Iterator support
* 8.1. Reverse iterators
* 8.2. Difference between iterators
*  
* TODO Operations:
* 1. RemoveAtSwap (+DONE)
* 2. Find (+DONE)
* 2.1. FindNot (+DONE)
* 2.2. All return INDEX_NONE. Is it more efficient to return Length?
* 2.3. All check indices, but maybe it is more efficient to check only start < end?
* 3. Comparison (+DONE)
* 4. Insert/Remove:
* 4.1. It must be possible to set position equal to Len() to insert elements and the end.
* 5. AddUnique (+DONE)
* 6. Sort (+DONE)
* 7. BinarySearch
* 8. Swap
*/
