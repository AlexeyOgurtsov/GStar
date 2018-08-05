#pragma once

#include "../ContainerSystem.h"
#include "../UT/TKeyValue.h"

namespace TRBTreeImpl
{
	/**
	* Index of the Node in the buffer.
	*/
	using NodeIndex = int;

	/**
	* Index of the child of the node 
	* (0 = left; 1 = right).
	*/
	using NodeChildIndex = int;

	/**
	* Unically identifies node relative to its parent.
	*/
	struct ChildNodeRef
	{
		/**
		* Parent index (see NodeIndex).
		*/
		NodeIndex ParentIdx;

		/**
		* Child index (see NodeChildIndex).
		* Undefined if this reference is null.
		*/
		NodeChildIndex ChildIdx;

		/**
		* Returns true if this reference does point to a node.
		*/
		bool IsValid() const { return ParentIdx != INDEX_NONE; }

		/**
		* Returns true if this reference represents a null pointer.
		*/
		bool IsNull() const { return ! IsValid(); }

		/**
		* Default ctor: initialize and invalid reference.
		*/
		ChildNodeRef() :
			ParentIdx(INDEX_NONE) {}

		/**
		* Creates a reference to the node by parent and child index.
		*/
		ChildNodeRef(NodeIndex InParentIdx, NodeChildIndex InChildIdx) :
			ParentIdx(InParentIdx)
		,	ChildIdx(InChildIdx) {}
	};

	/**
	* Represents a node of the tree.
	*/
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
		* If false, the node was removed.
		*/
		bool bExists = true;

		/**
		* Index of the parent pointer (INDEX_NONE in the case of the root).
		*/
		NodeIndex ParentIdx;

		/**
		* Index of the child node (0=left; 1=right).
		*
		* @returns: child node index, or INDEX_NONE if NO given child.
		*/
		int GetChild(NodeChildIndex InChildIndex) const
		{
			BOOST_ASSERT_MSG(InChildIndex >= 0, "TRBTreeImpl::Node: GetChild: ChildIndex must be non-negative");
			BOOST_ASSERT_MSG(InChildIndex <= 1, "TRBTreeImpl::Node: GetChild: ChildIndex is greater than maximal");
			return Children[InChildIndex];
		}

		/**
		* Sets the given child index.
		*
		* @see: GetChild
		*/
		void SetChild(NodeChildIndex InChildIndex, NodeIndex InIdx)
		{

			BOOST_ASSERT_MSG(InChildIndex >= 0, "TRBTreeImpl::Node: SetChild: ChildIndex must be non-negative");
			BOOST_ASSERT_MSG(InChildIndex <= 1, "TRBTreeImpl::Node: SetChild: ChildIndex is greater than maximal");
			Children[InChildIndex] = InIdx;
		}

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
		__forceinline void SetParent(NodeIndex InIdx)
		{
			ParentIdx = InIdx;
		}

		/**
		* Sets left child.
		*/
		__forceinline void SetLeftChild(NodeIndex InIdx)
		{
			SetChild(0, InIdx);
		}

		/**
		* Sets right child.
		*/
		__forceinline void SetRightChild(NodeIndex InIdx)
		{
			SetChild(1, InIdx);
		}

		/**
		* Constructs a RED node with a given key and value.
		*/
		Node(const KeyValueType& InKV, NodeIndex InParentIdx) :
			KV{InKV}
		,	ParentIdx{ InParentIdx } 
		{
			Children[0] = Children[1] = INDEX_NONE;
		}

	private:	
		/**
		* Children.
		*/
		NodeIndex Chilren[2];
	};
} // TRBTreeImpl