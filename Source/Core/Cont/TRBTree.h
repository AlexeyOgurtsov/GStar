#pragma once

#include "Priv/TRBTreeImpl.h"
#include "../Cont/TVector.h"

/**
* TODO Minimal test:
* 1. Contains operation;
* 2. First
*
* TODO CopyTo operation.
* 1. NoValue for absent value of a key.
* 2. Use KVType as argument (instead of separate Key and Value);
* 3. Create the KeyValue structure, change the Node to use it.
* 4. Create tests;
* 5. Create the operation;
*
* TODO Add operation:
* 1. Create the ToArray function.
* 2. Write the tests for the Add operation;
* 3. Parametrize the Node, so that children are represented as an array;
* 4. Create the TryInsertChild(ParentIdx, bool bRight, Key, Value) function;
*
* TODO First:
* 1. Add.
* 2. Remove.
* 3. Find
*
* TODO Second:
* 1. Iteration (begin/end etc.)
*
* TODO Third:
* 1. Clear()
* 2. 
*
* TODO Generalize:
* 1. Provide the means to select the key comparison function.
*/

/**
* Red-black tree.
*
* WARNING!!! This implementation does NOT automatically frees the node memory.
*/
template<class KVTypeArg>
class TRBTree
{
public:
	/**
	* KeyValue
	*/
	using KeyValueType = TKeyValue<KVTypeArg>;

	/**
	* Key type.
	*/
	using KeyType = typename KeyValueType::KeyType;

	/**
	* Value type.
	*/
	using ValueType = typename KeyValueType::ValueType;

	/**
	* Capacity to be used for the buffer by default.
	*/
	constexpr static int DEFAULT_CAPACITY = 1024;

	/**
	* Constructs with the default capacity.
	*/
	TRBTree() : TRBTree{ DEFAULT_CAPACITY } {}

	/**
	* Constructs with the given capacity.
	*/
	explicit TRBTree(int InCapacity) :
		RootIdx{INDEX_NONE}
	,	Count{0}
	{
		Buffer.ReserveGrow(InCapacity);
	}

	/**
	* Count of nodes.
	*/
	__forceinline int Num() const { return Count; }

	/**
	* Returns true if buffer is empty.
	*/
	__forceinline bool Empty() const { return 0 == Count; }

	/**
	* First.
	*/
	__forceinline const KeyValueType& First() const
	{
		BOOST_ASSERT( ! Empty() );
		return GetNode(RootIdx)->KV;
	}

	/**
	* Returns true, if contains the given key.
	*/
	bool Contains(const KeyType& InKey) const
	{
		return nullptr != Find(InKey);
	}

	/**
	* Searches Key-Value pair by the given key.
	*
	* @Returns: Pointer to the Key-Value pair (or nullptr, if NOT found).
	*/
	const KeyValueType* Find(const KeyType& InKey) const
	{
		BOOST_ASSERT_MSG(false, "TRBTree::Find: NOT YET IMPL"); return nullptr;
	}

	/**
	* Searches Value by the given key.
	*
	* @Returns: Pointer to the Value (or nullptr, if NOT found).
	*/
	const ValueType* FindValue(const KeyType& InKey) const
	{
		BOOST_ASSERT_MSG(false, "TRBTree::FindValue: NOT YET IMPL"); return nullptr;
	}

	/**
	* Searches Value by the given key (mutable version).
	*
	* @returns: Pointer to the Value (or nullptr, if NOT found).
	* @see: Non-const version
	*/
	ValueType* FindValue(const KeyType& InKey)
	{
		BOOST_ASSERT_MSG(false, "TRBTree::FindValue: NOT YET IMPL"); return nullptr;
	}

	/**
	* Adds a new node to the tree.
	*
	* @Returns: true if was added (or false if already was in the tree).
	*/
	bool Add(const KeyType& InKey, const ValueType& InValue)
	{
		int NewNodeIdx;
		return AddNewNode(InKey, InValue, /*Out*/ NewNodeIdx);
	}

	/**
	* Removes a node from the tree.
	*
	* @Returns: true if was deleted (or false if value was not found).
	*/

	/**
	* Copies all key-value pairs to the given Buffer.
	*/

	/*
	* Copies all key-value pairs to the given Buffer without preserving order.
	*
	* Buffer must be capable to store all the values.
	*/
	void CopyUnorderedTo(KeyValueType* pInBuffer)
	{
		// TODO Optimization: 
		// Iteration over all the nodes may be not the best way.
		BOOST_ASSERT(pInBuffer);
		for (int i = 0; i < Count; i++)
		{
			const NodeType* pSrcNode = GetNode(i);
			if (pSrcNode->bExists)
			{
				pInBuffer[i] = pSrcNode->KV;
			}
		}
	}

private:
	using NodeType = TRBTreeImpl::Node<KVTypeArg>;

	/**
	* Iterates nodes.
	*/
	class NodeIterator
	{
	public:
		/**
		* Constructor
		*/
		NodeIterator(TRBTree *pInTree, TRBTreeImpl::ChildNodeRef InChildRef) :
			pTree(pInTree)
		,	ChildRef(InChildRef) {}

	private:
		/**
		* Pointer to tree.
		*/
		TRBTree *pTree;

		/**
		* Reference to node relative to parent 
		* (null in the case of the root node).
		*/
		TRBTreeImpl::ChildNodeRef ChildRef;
	};

	/**
	* Returns true if left key less than right according to the used compare function.
	*/
	static bool KeyLess(const KeyType& A, const KeyType& const B)
	{
		// TODO: Provide the means to customize the compare function.
		return A < B;
	}

	/**
	* Returns true if left key equals right according to the used compare function.
	*/
	static bool KeyEqual(const KeyType& A, const KeyType& B)
	{
		return ( ! KeyLess(A,B) ) && ( ! KeyLess(B, A) );
	}

	/**
	* Searches node with the given key.
	*
	* Returns true, if the node is found.
	*/
	bool FindNode(const KeyType& InKey, int& OutIdx, int& OutChildIndex)
	{
		BOOST_ASSERT_MSG(false, "TRBTree: FindNode: Not Yet impl"); return false;
	}

	/**
	* Creates and adds a new node to the tree.
	*
	* @Returns: true if was added (or false if already was in the tree).
	*/
	bool AddNewNode(const KeyType& InKey, const ValueType& InValue, int& OutIdx)
	{
		BOOST_ASSERT_MSG(false, "TRBTree: AddNewNode: Not yet impl.");

		OutIdx = RootIdx;
		if (INDEX_NONE == RootIdx)
		{
			Buffer.Add(NodeType{InKey, InValue, /*ParentIdx*/INDEX_NONE});
			RootIdx = 0;
			return true;
		}

		int OutIdx = RootIdx;
		while (true)
		{
			BOOST_ASSERT_MSG(CurrIdx != INDEX_NONE, "AddNewNode: We never should encounter INDEX_NONE at this point");
			NodeType* Curr = GetNode(OutIdx);
			if (KeyLess(InKey, Curr->GetKey()))
			{
				/*
				if (TryInsertLeftChild(OutIdx, InKey, InValue))
				{
					return true;
				}
				*/
				OutIdx = Curr->LeftChildIdx;
				continue;
			}
			else if (KeyLess(Curr->GetKey(), InKey))
			{
				/*
				if (TryInsertRightChild(OutIdx, InKey, InValue))
				{
					return true;
				}
				*/
				OutIdx = Curr->RightChildIdx;
				continue;
			}
			BOOST_ASSERT_MSG(KeyEqual(Curr->GetKey(), Key), "TRBTree: AddNewNode: At this point current node key must equal to the key we search for");
			return false;
		}
	}

	/**
	* Gets node by index.
	*/
	__forceinline const NodeType* GetNode(int InIdx) const { return &Buffer[InIdx]; };

	/**
	* Gets node by index.
	*/
	__forceinline NodeType* GetNode(int InIdx) { return &Buffer[InIdx]; };

	/**
	* Buffer for storing nodes.
	* Each slot of the buffer may store a node or may not.
	*/
	TVector<NodeType> Buffer;

	/**
	* Index of the root node (or INDEX_NONE if empty).
	*/
	int RootIdx;

	/**
	* Count of nodes.
	*/
	int Count;
};