#pragma once

#include "Priv/TRBTreeImpl.h"
#include "../Cont/TVector.h"

/**
* TODO Traverse ordered operation:
* 1. Get deepest left/right from node identified by ChildNodeRef;
* 2. Create the Stack-overflow unit-test for traverse;
*
* TODO Helper accessors:
* 1. GetParentNode()
* 1.1. Should we check if we reference to Root?
*
* TODO Iterator:
* 1. Make const-correct
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
		if (Empty())
		{
			return nullptr;
		}

		TRBTreeImpl::ChildNodeRef NodeRef = TRBTreeImpl::ChildNodeRef::Invalid();
		bool bFound = FindNode(InKey, /*OutNodeRef*/ NodeRef);
		if ( ! bFound )
		{
			return nullptr;
		}
		return &GetNode(NodeRef)->KV;
	}

	/**
	* Searches Value by the given key.
	*
	* @Returns: Pointer to the Value (or nullptr, if NOT found).
	*/
	const ValueType* FindValue(const KeyType& InKey) const { return FindValueImpl(InKey); }

	/**
	* Searches Value by the given key (mutable version).
	*
	* @returns: Pointer to the Value (or nullptr, if NOT found).
	* @see: Non-const version
	*/
	ValueType* FindValue(const KeyType& InKey) { return const_cast<ValueType*>(FindValueImpl(InKey)); }

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
	* Preserves order.
	*/
	void CopyTo(KeyValueType* pInBuffer)
	{
		BOOST_ASSERT_MSG(false, "TRBTree::CopyTo: NOT YET IMPL");
		KeyValueType* pCurr = pInBuffer;
		Traverse
		(
			[&pCurr](const KeyValueType& KV) { *pCurr = KV; pCurr++; }
		);
	}

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

	/**
	* Traverses in the order of the keys.
	*
	* @param TraverseFunc: function that takes reference to key-value pair.
	*/
	template<class TraverseFunc>
	void Traverse(TraverseFunc Func)
	{
		if ( Empty() )
		{
			return;
		}

		return TraverseSubtree(TRBTreeImpl::ChildNodeRef::RootNode(), Func);
	}

private:
	friend class NodeIterator;

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
		NodeIterator(const TRBTree *pInTree, TRBTreeImpl::ChildNodeRef InNodeRef) :
			pTree(pInTree)
		,	NodeRef(InNodeRef) 
		{
			BOOST_ASSERT(pTree);
		}

		/**
		* Returns true if node is null.
		*/
		__forceinline bool IsNull() const
		{
			return NodeRef.IsNull();
		}

		/**
		* Reference to node relative to parent 
		* (null in the case of the root node).
		*/
		__forceinline TRBTreeImpl::ChildNodeRef GetNodeRef() const { return NodeRef; }

		/**
		* Parent node.
		*/
		__forceinline const NodeType* GetParentNode() const { return pTree->GetNode(ChildRef.ParentIdx); }

		/**
		* Node.
		*/
		__forceinline const NodeType* GetNode() const { return pTree->GetNode(NodeRef); }

		/**
		* Returns child node reference.
		*/
		__forceinline TRBTreeImpl::ChildNodeRef GetChildRef(TRBTreeImpl::NodeChildIndex ChildIdx) const	
		{
			return pTree->GetChildNodeRef(NodeRef, ChildIdx);
		}

		/**
		* Returns iterator to left.
		*/
		NodeIterator GetLeft() const { return NodeIterator{ pTree, GetChildRef(0) }; }
	
		/**
		* Returns iterator to right.
		*/
		NodeIterator GetRight() const { return NodeIterator{pTree, GetChildRef(1) };
	}

	private:
		/**
		* Pointer to tree.
		*/
		const TRBTree *pTree;
		TRBTreeImpl::ChildNodeRef NodeRef;
	};

	/**
	* Returns true if left key less than right according to the used compare function.
	*/
	static bool KeyLess(const KeyType& A, const KeyType& B)
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
	* Searches Value by the given key.
	*
	* @Returns: Pointer to the Value (or nullptr, if NOT found).
	*/
	const ValueType* FindValueImpl(const KeyType& InKey) const
	{
		const KeyValueType* KeyValue = Find(InKey);
		if (KeyValue == nullptr)
		{
			return nullptr;
		}
		return &KeyValue->Value;
	}

	/**
	* Traverses subtree in the order of the keys.
	*
	* @param TraverseFunc: function that takes reference to key-value pair.
	*/
	template<class TraverseFunc>
	void TraverseSubtree(TRBTreeImpl::ChildNodeRef InRootRef, TraverseFunc Func)
	{
		BOOST_ASSERT_MSG( ! Empty(), "TRBTree::TraverseSubtree: the subtree must be NON-empty" );
		
		TRBTreeImpl::ChildNodeRef const LeftRef = GetChildNodeRef(InRootRef, TRBTreeImpl::LEFT_CHILD_IDX);
		if ( ! LeftRef.IsNull() )
		{
			TraverseSubtree(LeftRef, Func);
		}

		Func(GetNode(InRootRef)->KV);

		TRBTreeImpl::ChildNodeRef const RightRef = GetChildNodeRef(InRootRef, TRBTreeImpl::RIGHT_CHILD_IDX);
		if ( ! RightRef.IsNull() )
		{
			TraverseSubtree(RightRef, Func);
		}
	}

	/**
	* Searches node with the given key in NON-EMPTY (!) container.
	*
	* Returns true, if the node is found.
	*/
	bool FindNode(const KeyType& InKey, TRBTreeImpl::ChildNodeRef& OutNodeRef ) const
	{
		BOOST_ASSERT_MSG( ! Empty(), "TRBTree: FindNode: Container must be non-empty" );

		OutNodeRef = TRBTreeImpl::ChildNodeRef::RootNode();
		NodeIterator It { this, OutNodeRef };

		while (true)
		{
			const KeyType* pCurrKey = &(It.GetNode()->GetKey());
			if(KeyEqual(*pCurrKey, InKey))
			{
				return true;
			}

			if(KeyLess(*pCurrKey, InKey)) 
			{
				It = It.GetLeft();
			}
			else if(KeyLess(InKey, *pCurrKey))
			{
				It = It.GetRight();
			}

			OutNodeRef = It.GetNodeRef();

			if(It.IsNull())
			{
				return false;
			}
		}

		BOOST_ASSERT_MSG(false, "Should NOT get here");
		return false;
	}

	/**
	* Creates and adds a new node to the tree.
	*
	* @Returns: true if was added (or false if already was in the tree).
	*/
	bool AddNewNode(const KeyType& InKey, const ValueType& InValue, int& OutIdx)
	{
		BOOST_ASSERT_MSG(false, "TRBTree: AddNewNode: Not yet impl."); return false;

		/*
		OutIdx = RootIdx;
		if (INDEX_NONE == RootIdx)
		{
			Buffer.Add(NodeType{ KeyValueType {InKey, InValue}, INDEX_NONE });
			RootIdx = 0;
			return true;
		}

		OutIdx = RootIdx;
		while (true)
		{
			BOOST_ASSERT_MSG(CurrIdx != INDEX_NONE, "AddNewNode: We never should encounter INDEX_NONE at this point");
			NodeType* Curr = GetNode(OutIdx);
			if (KeyLess(InKey, Curr->GetKey()))
			{
				if (TryInsertLeftChild(OutIdx, InKey, InValue))
				{
					return true;
				}
				OutIdx = Curr->LeftChildIdx;
				continue;
			}
			else if (KeyLess(Curr->GetKey(), InKey))
			{
				if (TryInsertRightChild(OutIdx, InKey, InValue))
				{
					return true;
				}
				OutIdx = Curr->RightChildIdx;
				continue;
			}
			BOOST_ASSERT_MSG(KeyEqual(Curr->GetKey(), InKey), "TRBTree: AddNewNode: At this point current node key must equal to the key we search for");
			return false;
		}
		*/
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
	* Gets parent node by reference.
	*/
	__forceinline const NodeType* GetParentNode(TRBTreeImpl::ChildNodeRef InRef) const
	{
		BOOST_ASSERT_MSG(InRef.IsValid(), "TRBTree::GetParentNode(ChildNodeRef): reference must be valid");
		if(INDEX_NONE == InRef.ParentIdx)
		{
			return nullptr;
		}
		return GetNode(InRef.ParentIdx);
	}

	/**
	* Gets parent node by reference.
	*/
	__forceinline NodeType* GetParentNode(TRBTreeImpl::ChildNodeRef InRef)
	{
		BOOST_ASSERT_MSG(InRef.IsValid(), "TRBTree::GetParentNode(ChildNodeRef): reference must be valid");
		if(INDEX_NONE == InRef.ParentIdx)
		{
			return nullptr;
		}
		return GetNode(InRef.ParentIdx);
	}

	/**
	* Gets node by reference.
	*/
	__forceinline const NodeType* GetNode(TRBTreeImpl::ChildNodeRef InRef) const
	{
		BOOST_ASSERT_MSG(InRef.IsValid(), "TRBTree::GetNode(ChildNodeRef): reference must be valid");
		if(InRef.IsRoot())
		{
			return GetNode(RootIdx);
		}
		return GetChildNode(GetParentNode(InRef), InRef.ChildIdx);
	}

	/**
	* Gets node by reference.
	*/
	__forceinline NodeType* GetNode(TRBTreeImpl::ChildNodeRef InRef)
	{
		BOOST_ASSERT_MSG(InRef.IsValid(), "TRBTree::GetNode(ChildNodeRef): reference must be valid");
		if(InRef.IsRoot())
		{
			return GetNode(RootIdx);
		}
		return GetChildNode(GetParentNode(InRef), InRef.ChildIdx);
	}

	/**
	* Gets child node by index.
	*/
	__forceinline NodeType* GetChildNode(NodeType* pInParent, TRBTreeImpl::NodeChildIndex InChildIdx)
	{
		BOOST_ASSERT(pInParent);
		return GetNode(pInParent->GetChild(InChildIdx));
	}

	/**
	* Gets child node by index.
	*/
	__forceinline const NodeType* GetChildNode(const NodeType* pInParent, TRBTreeImpl::NodeChildIndex InChildIdx) const
	{
		BOOST_ASSERT(pInParent);
		return GetNode(pInParent->GetChild(InChildIdx));
	}

	/**
	* Gets child node reference.
	*/
	__forceinline TRBTreeImpl::ChildNodeRef GetChildNodeRef(TRBTreeImpl::ChildNodeRef NodeRef, TRBTreeImpl::NodeChildIndex InChildIdx) const
	{
		BOOST_ASSERT(NodeRef.IsValid());
		int const NodeIndexForChild = GetParentNode(NodeRef)->GetChild(NodeRef.ChildIdx);
		if (NodeIndexForChild != INDEX_NONE)
		{
			return TRBTreeImpl::ChildNodeRef{ NodeIndexForChild, InChildIdx };
		}
		else
		{
			return TRBTreeImpl::ChildNodeRef::Invalid();
		}
	}

	/**
	* Get the deepest left or right child node.
	*/
	TRBTreeImpl::ChildNodeRef GetDeepestNodeRef(TRBTreeImpl::ChildNodeRef InSearchRoot, TRBTreeImpl::NodeChildIndex InChildIdx)
	{
		TRBTreeImpl::ChildNodeRef NodeRef = InSearchRoot;
		while (true)
		{
			TRBTreeImpl::ChildNodeRef NextNodeRef = GetChildNodeRef(NodeRef, InChildIdx);
			if (NextNodeRef.IsNull())
			{
				return NodeRef;
			}
			NodeRef = NextNodeRef;
		}
	}
	
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