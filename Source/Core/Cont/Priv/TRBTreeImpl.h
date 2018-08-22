#pragma once

#include "../ContainerSystem.h"
#include "../UT/TKeyValue.h"
#include <optional>

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
	* Left child index.
	*/
	constexpr NodeChildIndex LEFT_CHILD_IDX = 0;

	/**
	* Right child index.
	*/
	constexpr NodeChildIndex RIGHT_CHILD_IDX = 1;

	inline NodeChildIndex InvertChildIndex(NodeChildIndex InChildIdx)
	{
		if (InChildIdx == LEFT_CHILD_IDX)
		{
			return RIGHT_CHILD_IDX;
		}
		else
		{
			return LEFT_CHILD_IDX;
		}
	}

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

	private:
		bool bValid;

	public:
		/**
		* Returns true if this reference does point to a node.
		*/
		bool IsValid() const { return bValid; }

		/**
		* Returns true if this reference represents a null pointer.
		*/
		bool IsNull() const { return ! IsValid(); }

		/**
		* Returns true if root node.
		*/
		bool IsRoot() const { return ParentIdx == INDEX_NONE; }

	public:
		/**
		* Creates a reference to the node by parent and child index.
		*/
		ChildNodeRef(NodeIndex InParentIdx, NodeChildIndex InChildIdx) :
			ParentIdx(InParentIdx)
		,	ChildIdx(InChildIdx)
		,	bValid{true} {}

	public:
		/**
		* returns invalid reference.
		*/
		static ChildNodeRef Invalid()
		{
			static const ChildNodeRef InvalidRef; return InvalidRef;
		}

		/**
		* returns root.
		*/
		static ChildNodeRef RootNode()
		{
			static const ChildNodeRef RootRef { INDEX_NONE, 0 }; return RootRef;
		}
	
	private:
		/**
		* Default ctor: initialize and invalid reference.
		*/
		ChildNodeRef() :
			/**
			* We delibarately set parent idx to valid value,
			* So IsRoot() will NOT return true, if the reference is invalid.
			*/
			ParentIdx(0)
		,	bValid{ false } {}
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
		* Returns the Key-value pair.
		*/
		const KeyValueType& GetKV() const	
		{
			BOOST_ASSERT_MSG(Exists(), "TRBTreeImpl::Node: GetKV: Node must exist"); 
			return OptKV.value(); 
		}
		
		/**
		* Returns the Key-value pair.
		*/
		KeyValueType& GetKV() 
		{
			BOOST_ASSERT_MSG(Exists(), "TRBTreeImpl::Node: GetKV: Node must exist");
			return OptKV.value(); 
		}

		/**
		* Key to be used for comparisons.
		*/
		__forceinline const KeyType& GetKey() const { return GetKV().Key; }

		/**
		* Payload data.
		*/
		__forceinline ValueType GetValue() const { return GetKV().Value; }

		/**
		* If false, the node was removed.
		*/
		__forceinline bool Exists() const { return OptKV.has_value(); }

		/**
		* Destroys the key-value pair properly, calling the necessary constructors.
		*/
		void Destroy()
		{
			OptKV.reset();
		}

		/**
		* Index of the parent pointer (INDEX_NONE in the case of the root).
		*/
		NodeIndex ParentIdx;

		bool IsChildFree() const
		{
			return ( ! HasChild(LEFT_CHILD_IDX) ) && ( ! HasChild(RIGHT_CHILD_IDX) );
		}

		/**
		* Has child.
		*/
		__forceinline bool HasChild(NodeChildIndex InChildIndex) const
		{
			return INDEX_NONE != GetChild(InChildIndex);
		}

		/**
		* Index of the child node (0=left; 1=right).
		*
		* @returns: child node index, or INDEX_NONE if NO given child.
		*/
		__forceinline int GetChild(NodeChildIndex InChildIndex) const
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
		* Makes the given node double black
		*/
		void MakeDoubleBlack()
		{
			BOOST_ASSERT_MSG( IsBlack(), "TRBTree::MakeDoubleBlack: the node must already be black" );
			bDoubleBlack = true;
		}

		/**
		* Returns true if the node is double black
		*/
		bool IsDoubleBlack() const { return bDoubleBlack; }

		/**
		* Returns true if the node is black (or double black).
		*/
		__forceinline bool IsBlack() const { return ! bRed; }

		/**
		* Returns true if the node is red.
		*/
		__forceinline bool IsRed() const { return bRed; }

		/**
		* Copies color from the given node to this one.
		*/
		__forceinline void CopyColorFrom(const Node* pInSource)
		{
			BOOST_ASSERT(pInSource);
			bRed = pInSource->bRed;
			bDoubleBlack = pInSource->bDoubleBlack;
		}

		/**
		* Makes black.
		*/
		__forceinline void MakeBlack()
		{
			bRed = false;
			bDoubleBlack = false;
		}

		/**
		* Makes red.
		*/
		__forceinline void MakeRed()
		{
			bRed = true;
			bDoubleBlack = false;
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
			OptKV{InKV}
		,	ParentIdx{ InParentIdx } 
		{
			Children[0] = Children[1] = INDEX_NONE;
		}

		/**
		* Constructs a RED node with a given key and value by moving Key-value.
		*/
		Node(KeyValueType&& InKV, NodeIndex InParentIdx) :
			OptKV{ std::move(InKV) }
		,	ParentIdx{ InParentIdx }
		{
			Children[0] = Children[1] = INDEX_NONE;
		}

	private:
		std::optional<KeyValueType> OptKV;	
		NodeIndex Children[2];
		bool bRed = true;
		bool bDoubleBlack = false;
	};
} // TRBTreeImpl