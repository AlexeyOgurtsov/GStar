#pragma once

#include "Priv/TRBTreeImpl.h"
#include "../Cont/TVector.h"

/**
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
* Pass instead of value for key-only container.
*/
struct EmptyRBTreePayload {};

/**
* Red-black tree.
*
* WARNING!!! This implementation does NOT automatically frees the node memory.
*/
template<class KeyTypeArg, class ValueTypeArg = EmptyRBTreePayload>
class TRBTree
{
public:
	/**
	* Key type.
	*/
	using KeyType = KeyTypeArg;

	/**
	* Value type.
	*/
	using ValueType = ValueTypeArg;

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
	* Finds a node by the given criteria.
	*
	* @Returns: Pointer to the found node data (or nullptr, if NOT found).
	*/

	/**
	* Copies all key-value pairs to the given Buffer.
	*/

private:
	using NodeType = TRBTreeImpl::Node<KeyType, ValueType>;

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
			if (KeyLess(InKey, Curr->Key))
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
			else if (KeyLess(Curr->Key, InKey))
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
			BOOST_ASSERT_MSG(KeyEqual(Curr->Key, Key), "TRBTree: AddNewNode: At this point current node key must equal to the key we search for");
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