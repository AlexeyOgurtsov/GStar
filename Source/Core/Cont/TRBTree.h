#pragma once

#include "Priv/TRBTreeImpl.h"
#include "../Cont/TVector.h"

/**
* TODO:
* 1. Create the Stack-overflow unit-test for traverse;
*
*
* TODO Iterator:
* 1. Make const-correct
*
* TODO Operations:
* 1.
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
* 2. CopyTo(TArray) function.
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
	using KeyValueType = typename TKeyValue<KVTypeArg>;

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
		TRBTreeImpl::ChildNodeRef NodeRef = TRBTreeImpl::ChildNodeRef::Invalid();
		return AddNewNode(KeyValueType{InKey,InValue}, /*Out*/ NodeRef);
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
		* Returns true if node does NOT exist.
		*/
		__forceinline bool DoesNotExist() const
		{
			return ( ! pTree->NodeExists(NodeRef) );
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
		
		// TODO: May this implementation overflow the stack?

		TRBTreeImpl::ChildNodeRef const LeftRef = GetChildNodeRef(InRootRef, TRBTreeImpl::LEFT_CHILD_IDX);
		if ( NodeExists(LeftRef) )
		{
			TraverseSubtree(LeftRef, Func);
		}

		Func(GetNode(InRootRef)->KV);

		TRBTreeImpl::ChildNodeRef const RightRef = GetChildNodeRef(InRootRef, TRBTreeImpl::RIGHT_CHILD_IDX);
		if ( NodeExists(RightRef))
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
			if(KeyEqual(InKey, *pCurrKey))
			{
				return true;
			}

			if(KeyLess(InKey, *pCurrKey)) 
			{
				It = It.GetLeft();
			}
			else if(KeyLess(*pCurrKey, InKey))
			{
				It = It.GetRight();
			}

			OutNodeRef = It.GetNodeRef();

			if (It.DoesNotExist())
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
	bool AddNewNode(const KeyValueType& InKV, TRBTreeImpl::ChildNodeRef& OutChildNodeRef)
	{
		if (Empty())
		{
			OutChildNodeRef = TRBTreeImpl::ChildNodeRef::RootNode();
			RootIdx = CreateNewNode(InKV, INDEX_NONE);
			return true;
		}

		if ( FindNode(InKV.Key, /*OutNodeRef*/OutChildNodeRef) )
		{
			return false;
		}

		AddNewNodeAtRef(InKV, OutChildNodeRef);
		return true;
	}

	/**
	* Creates a new node.
	* Automatically increments the count of nodes.
	*/
	TRBTreeImpl::NodeIndex CreateNewNode(const KeyValueType& InKV, TRBTreeImpl::NodeIndex InParentIdx)
	{
		TRBTreeImpl::NodeIndex const NewNodeIndex = TRBTreeImpl::NodeIndex { Buffer.Len() };
		NodeType NewNode { InKV, InParentIdx };
		Buffer.Add(std::move(NewNode));
		Count++;
		return NewNodeIndex;
	}

	/**
	* Creates and appends a node by the specific reference.
	*/
	void AddNewNodeAtRef(const KeyValueType& InKV, TRBTreeImpl::ChildNodeRef Where)
	{
		BOOST_ASSERT_MSG( ! Empty(), "TRBTree::AddNewNodeAtRef: the container must contain at least one node");
		BOOST_ASSERT_MSG( ! Where.IsRoot(), "TRBTree::AddNewNodeAtRef: we cannot append at the root node position" );
		BOOST_ASSERT_MSG( ! NodeExists(Where), "TRBTree::AddNewNodeAtRef: the position must be not occupied" );
		TRBTreeImpl::NodeIndex const NewNodeIdx = CreateNewNode(InKV, Where.ParentIdx);
		NodeType* const pParentNode = GetParentNode(Where);
		pParentNode->SetChild(Where.ChildIdx, NewNodeIdx);
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
	* Gets node by reference impl.
	*/
	__forceinline const NodeType* GetNodeImpl(TRBTreeImpl::ChildNodeRef InRef) const
	{
		BOOST_ASSERT_MSG(InRef.IsValid(), "TRBTree::GetNodeImpl(ChildNodeRef): reference must be valid");
		BOOST_ASSERT_MSG(NodeExists(InRef), "TRBTree::GetNodeImpl(ChildNodeRef): the node must exist");
		if(InRef.IsRoot())
		{
			return GetNode(RootIdx);
		}
		return GetNode(GetParentNode(InRef)->GetChild(InRef.ChildIdx));
	}

	/**
	* Gets node by reference.
	*/
	__forceinline const NodeType* GetNode(TRBTreeImpl::ChildNodeRef InRef) const
	{
		return GetNodeImpl(InRef);
	}

	/**
	* Gets node by reference.
	*/
	__forceinline NodeType* GetNode(TRBTreeImpl::ChildNodeRef InRef)
	{
		return const_cast<NodeType*>(GetNodeImpl(InRef));
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
	* Returns true if node that the given ref references exists.
	*/
	bool NodeExists(TRBTreeImpl::ChildNodeRef NodeRef) const
	{
		if(NodeRef.IsRoot())
		{
			return ! Empty();
		}
		return INDEX_NONE != (GetNode(NodeRef.ParentIdx)->GetChild(NodeRef.ChildIdx));			
	}

	/**
	* Gets child node reference.
	* The node itself and the corresponding child must exist.
	*/
	__forceinline TRBTreeImpl::ChildNodeRef GetChildNodeRef(TRBTreeImpl::ChildNodeRef NodeRef, TRBTreeImpl::NodeChildIndex InChildIdx) const
	{
		BOOST_ASSERT( NodeExists(NodeRef) );
		int NextChildParentIdx = INDEX_NONE;
		if (NodeRef.IsRoot())
		{
			NextChildParentIdx = RootIdx;
		}
		else
		{
			NextChildParentIdx = GetParentNode(NodeRef)->GetChild(NodeRef.ChildIdx);
		}
		BOOST_ASSERT_MSG(INDEX_NONE != NextChildParentIdx, "TRBTree::GetChildNodeRef: the given child must exist");
		
		return TRBTreeImpl::ChildNodeRef{ NextChildParentIdx, InChildIdx };
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
			if ( ! NodeExists(NextNodeRef) )
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