#pragma once

#include "../ContainerSystem.h"
#include "../UT/TKeyValue.h"

namespace TRBTreeImpl
{
	template<class KVTypeArg>
	struct Node
	{
		using KeyValueType = typename TKeyValue<KVTypeArg>;	
		using KeyType = typename KeyValueType::KeyType;
		using ValueType = typename KeyValueType::ValueType;

		/**
		* Key/Value.
		*/
		KeyValueType KV;

		/**
		* Key to be used for comparisons.
		*/
		__forceinline KeyType GetKey() const { return KV.Key; }

		/**
		* Payload data.
		*/
		__forceinline ValueType GetValue() const { return KV.Value; }

		/**
		* Both children of the red node are blacks.
		*/
		bool bRed = true;

		/**
		* Index of the parent pointer (INDEX_NONE in the case of the root).
		*/
		int ParentIdx;

		/**
		* Index of the left child (INDEX_NONE if NO left child).
		*/
		int LeftChildIdx = INDEX_NONE;

		/**
		* Index of the right child (INDEX_NONE if NO left child).
		*/
		int RightChildIdx = INDEX_NONE;

		/**
		* Returns true if the node is black.
		*/
		__forceinline bool IsBlack() const { return ! bRed; }

		/**
		* Returns true if the node is red.
		*/
		__forceinline bool IsRed() const { return bRed; }

		/**
		* Makes black.
		*/
		__forceinline void MakeBlack()
		{
			bRed = false;
		}

		/**
		* Sets parent.
		*/
		__forceinline void SetParent(int InIdx)
		{
			ParentIdx = InIdx;
		}

		/**
		* Sets left child.
		*/
		__forceinline void SetLeftChild(int InIdx)
		{
			LeftChildIdx = InIdx;
		}

		/**
		* Sets right child.
		*/
		__forceinline void SetRightChild(int InIdx)
		{
			RightChildIdx = InIdx;
		}

		/**
		* Constructs a RED node with a given key and value.
		*/
		Node(const KeyValueType& InKV, int InParentIdx) :
			KV{InKV}
		,	ParentIdx{ InParentIdx } {}
	};
} // TRBTreeImpl