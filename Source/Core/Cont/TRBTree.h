#pragma once

#include "Priv/TRBTreeImpl.h"
#include "../Cont/TVector.h"
#include <tuple>
#include <type_traits>

/**
* TODO Key/Value iterator:
* 1. Insertion while iterating
* 2. Backward iteration
*
* TODO:
* 1. Create the Stack-overflow unit-test for traverse;
*
* TODO Node Iterator:
* 1. Make const-correct
*
* TODO First:
* 1. Add.
* 2. Remove.
* 3. Find
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

private:
	static auto GetThisTypeHelper() -> std::remove_reference_t<decltype(*this)>;

public:
	/**
	* Type of this tree
	*/
	using ThisType = decltype(GetThisTypeHelper());

	/**
	* Iterator type.
	*/
	template<class TreeTypeArg>
	class TIteratorBase;

	using IteratorType = TIteratorBase<ThisType>;
	using ConstIteratorType = TIteratorBase<const ThisType>;
	
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
	* Returns iterator to the first Key/Value pair.
	*/
	ConstIteratorType Iterator() const
	{
		return GetFirstIteratorImpl<ConstIteratorType>(this);
	}

	/**
	* Returns iterator to the first Key/Value pair.
	*/
	IteratorType Iterator()
	{
		return GetFirstIteratorImpl<IteratorType>(this);
	}

	/**
	* Returns const iterator to the first Key/Value pair.
	*/
	ConstIteratorType ConstIterator() const
	{
		return GetFirstIteratorImpl<ConstIteratorType>(this);
	}

	/**
	* C++ range iteration support.
	*/
	IteratorType begin() { return Iterator(); }

	/**
	* C++ range iteration support.
	*/
	ConstIteratorType begin() const { return ConstIterator(); }

	/**
	* C++ range iteration support.
	*/
	ConstIteratorType cbegin() const { return begin(); }

	/**
	* C++ range iteration support.
	*/
	IteratorType end() { return IteratorType::EndIterator(this); }

	/**
	* C++ range iteration support.
	*/
	ConstIteratorType end() const { return ConstIteratorType::EndIterator(this); }

	/**
	* C++ range iteration support.
	*/
	ConstIteratorType cend() const { return end(); }

	/**
	* Returns KeyValue with a minimal key.
	* Container must be NON-empty.
	*/
	const KeyValueType& Min() const
	{
		BOOST_ASSERT_MSG( ! Empty(), "TRBTree::Min: Container must be NON-empty");
		return GetNode(GetDeepestNodeRef(TRBTreeImpl::ChildNodeRef::RootNode(), TRBTreeImpl::LEFT_CHILD_IDX))->KV;
	}

	/**
	* Returns KeyValue with a maximal key.
	* Container must be NON-empty.
	*/
	const KeyValueType& Max() const
	{
		BOOST_ASSERT_MSG( ! Empty(), "TRBTree::Max: Container must be NON-empty");
		return GetNode(GetDeepestNodeRef(TRBTreeImpl::ChildNodeRef::RootNode(), TRBTreeImpl::RIGHT_CHILD_IDX))->KV;
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

public:
	/**
	* Iterates KeyValue pairs in the order of their keys.
	*/
	template<class TreeTypeArg>
	class TIteratorBase
	{
	public:
		template<class OtherTreeType>
		friend class TIteratorBase;

		/**
		* Key/Value type of this iterator.
		*/
		using KeyValueType = typename TreeTypeArg::KeyValueType;

		/**
		* Key that this iterator iterates.
		*/
		using KeyType = const typename KeyValueType::KeyType;

		/**
		* Value that this iterator iterates.
		*/
		using ValueType = typename KeyValueType::ValueType;

		/**
		* Constructs iterator from reference to the given node of the tree.
		*
		* If node reference is invalid, End iterator is created.
		*/
		TIteratorBase(TreeTypeArg *pInTree, TRBTreeImpl::ChildNodeRef InNodeRef) :
			pTree{ pInTree }
		,	NodeRef{ InNodeRef }
		{
			BOOST_ASSERT(pTree);
		}

		/**
		* Copy constructs.
		*/
		template<class OtherTreeType>
		TIteratorBase(const TIteratorBase<OtherTreeType>& InOther) :
			NodeRef{TRBTreeImpl::ChildNodeRef::Invalid()}
		{
			*this = InOther;
		}

		/**
		* Copies.
		*/
		template<class OtherTreeType>
		TIteratorBase& operator=(const TIteratorBase<OtherTreeType>& InOther)
		{
			pTree = InOther.pTree;
			NodeRef = InOther.NodeRef;
			return *this;
		}

		/**
		* Returns End iterator.
		*/
		static TIteratorBase EndIterator(TreeTypeArg *pInTree)
		{
			return TIteratorBase(pInTree, TRBTreeImpl::ChildNodeRef::Invalid());
		}

		/**
		* Returns current Key/Value pair.
		* Iterator must be a non-end iterator.
		*/
		__forceinline const KeyValueType& GetKeyValue() const
		{
			BOOST_ASSERT(!IsEnd());
			return pTree->GetNode(NodeRef)->KV;
		}

		/**
		* Returns key.
		*/
		__forceinline const KeyType& GetKey() const { return GetKeyValue().Key; }

		/**
		* Returns value.
		*/
		__forceinline const ValueType& GetValue() const { return GetKeyValue().Value; }

		/**
		* Is end iterator.
		*/
		__forceinline bool IsEnd() const
		{
			return NodeRef.IsNull();
		}

		/**
		* Dereference operator.
		* Iterator must be a non-end iterator.
		*/
		__forceinline const KeyValueType& operator*() const { return GetKeyValue(); }

		/**
		* Sets current value
		*/
		void SetValue(const ValueType& InValue)
		{
			GetNode()->KV.Value = InValue;
		}

		/**
		* Advances iterator to the next KeyValue pair.
		*/
		TIteratorBase& operator++()
		{
			AdvanceNext();
			return *this;
		}

		/**
		* Advances iterator to the next KeyValue pair.
		*/
		TIteratorBase operator++(int)
		{
			TIteratorBase OldIt = *this;
			TIteratorBase::operator++();
			return OldIt;
		}

		/**
		* Advances an iterator to previous KeyValue pair.
		*/
		TIteratorBase& operator--();

		/**
		* Advances iterator to the previus KeyValue pair.
		*/
		TIteratorBase operator--(int);

		template<class OtherTreeType>
		bool operator==(TIteratorBase<OtherTreeType> B)
		{
			if (IsEnd() && B.IsEnd())
			{
				return true;
			}
			else if (IsEnd() || B.IsEnd())
			{
				return false;
			}
			return GetKeyValue().Key == B.GetKeyValue().Key;
		}

		template<class OtherTreeType>
		bool operator!=(TIteratorBase<OtherTreeType> B)
		{
			return !(operator==(B));
		}

	protected:
		decltype(auto) GetNode() const { return pTree->GetNode(NodeRef); }

		void AdvanceNext()
		{
			if (GetNode()->HasChild(TRBTreeImpl::RIGHT_CHILD_IDX))
			{
				// Find the minimal value in the right subtree;
				NodeRef = pTree->GetDeepestNodeRef(pTree->GetChildNodeRef(NodeRef, TRBTreeImpl::RIGHT_CHILD_IDX), TRBTreeImpl::LEFT_CHILD_IDX);
			}
			else
			{
				// we have no right child, so getting up
				if (NodeRef.IsRoot())
				{
					// We reached the root and have no right child, so stop the iteration
					NodeRef = TRBTreeImpl::ChildNodeRef::Invalid();
				}
				else
				{
					// we have NOT reached the root yet
					if (NodeRef.ChildIdx == TRBTreeImpl::LEFT_CHILD_IDX)
					{
						// We are on the node that is the left child of its parent,
						// and it has no right child itself (left subtree is already iterated). 
						// That's why our parent is the next in order for iteration.
						NodeRef = pTree->GetParentNodeRef(NodeRef);
					}
					else
					{
						// We are on the node that is the right child of its parent,
						// and it has no right child itself (left subtree is already iterated).
						// So, we have no other way than backtrack.
						bool bEndIteration = pTree->BacktrackRightSubtreeIteration(/*Out*/NodeRef);

						// We may end up iteration by climbing from right parent to the root node. 
						// That would mean that the iteration is over.
						if (bEndIteration)
						{
							NodeRef = TRBTreeImpl::ChildNodeRef::Invalid();
						}
					}
				}
			}
		}

	private:
		TreeTypeArg *pTree;
		TRBTreeImpl::ChildNodeRef NodeRef;
	};

private:
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
	* Traverses non-empty subtree in the order of the keys.
	*
	* @param TraverseFunc: function that takes reference to key-value pair.
	*/
	template<class TraverseFunc>
	void TraverseSubtree(TRBTreeImpl::ChildNodeRef InRootRef, TraverseFunc Func)
	{
		BOOST_ASSERT_MSG( NodeExists(InRootRef), "TRBTree::TraverseSubtree: subtree must be non-empty" );
		
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
	* Gets parent node reference.
	*/
	TRBTreeImpl::ChildNodeRef GetParentNodeRef(const TRBTreeImpl::ChildNodeRef InNodeRef) const
	{
		const TRBTreeImpl::NodeChildIndex ParentIdx = InNodeRef.ParentIdx;
		const NodeType* const pParent = GetParentNode(InNodeRef);
		if (pParent->ParentIdx == INDEX_NONE) 
		{
			// If parent of parent is root, we return the root 
			return TRBTreeImpl::ChildNodeRef::RootNode();
		}
		else
		{
			// if parent of parent is an ordinary node
			const TRBTreeImpl::NodeIndex ParentParentIdx = pParent->ParentIdx;
			const NodeType* const pParentParent = GetNode(ParentParentIdx);
			TRBTreeImpl::NodeChildIndex ChildOfParentParentIndex = INDEX_NONE;
			if (ParentIdx == pParentParent->GetChild(TRBTreeImpl::LEFT_CHILD_IDX))
			{
				ChildOfParentParentIndex = TRBTreeImpl::LEFT_CHILD_IDX;
			}
			else
			{
				BOOST_ASSERT(ParentIdx == pParentParent->GetChild(TRBTreeImpl::RIGHT_CHILD_IDX));
				ChildOfParentParentIndex = TRBTreeImpl::RIGHT_CHILD_IDX;
			}
			return TRBTreeImpl::ChildNodeRef{ ParentParentIdx, ChildOfParentParentIndex };
		}
	}

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
	* Returns iterator to the first element.
	*
	* NOTE: We pass this pointer to make this function act either like const or mutable depending on context,
	* however formally be const.
	*/
	template<class IteratorTypeArg, class ThisTypeArg> IteratorTypeArg GetFirstIteratorImpl(ThisTypeArg InThisPtr) const
	{
		if (Empty())
		{
			return IteratorTypeArg(InThisPtr, TRBTreeImpl::ChildNodeRef::Invalid());
		}
		else
		{
			return IteratorTypeArg(InThisPtr, GetDeepestNodeRef(TRBTreeImpl::ChildNodeRef::RootNode(), TRBTreeImpl::LEFT_CHILD_IDX));
		}
	}

	/**
	* Get the deepest left or right child node.
	*/
	TRBTreeImpl::ChildNodeRef GetDeepestNodeRef(TRBTreeImpl::ChildNodeRef InSearchRoot, TRBTreeImpl::NodeChildIndex InChildIdx) const
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
	* Walks by parent links skipping all right childs and the first left child.
	*
	* Returns true, if we should end up iteration.
	*/
	bool BacktrackRightSubtreeIteration(TRBTreeImpl::ChildNodeRef& InOutNodeRef) const
	{
		BOOST_ASSERT_MSG( ! InOutNodeRef.IsRoot(), "TRBTree: BacktrackRightSubtreeIteration: must be called on NON-root node only");
		BOOST_ASSERT_MSG( InOutNodeRef.ChildIdx == TRBTreeImpl::RIGHT_CHILD_IDX, "TRBTree: BacktrackRightSubtreeIteration: must start on the node that is a right child of its parent");
		while (true)
		{
			// We must climb up by the first left child.
			bool bFromLeftChild = (InOutNodeRef.ChildIdx == TRBTreeImpl::LEFT_CHILD_IDX);
			InOutNodeRef = GetParentNodeRef(InOutNodeRef);
			
			if (InOutNodeRef.IsRoot())
			{
				// We may end up iteration by climbing from right child to the root parent node.
				// That would mean that we need to end up iteration.
				return ! bFromLeftChild;
			}
			else
			{
				if (bFromLeftChild)
				{
					// We may end up iteration by climbing from left child to the root parent node.
					// That would mean that we need to end up iteration.
					return InOutNodeRef.IsRoot();
				}
				// At this point we climbed the right child, so we must proceed iteration further.
				BOOST_ASSERT_MSG( ! bFromLeftChild, "TRBTree: BacktrackRightSubtreeIteration: At this point we must be climbed from the right child");
			}
		}
		BOOST_ASSERT_MSG(false, "TRBTree: BacktrackRightSubtreeIteration: Should NOT get here");
		return false;
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

