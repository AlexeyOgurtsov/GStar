#pragma once

#include "Priv/TRBTreeImpl.h"
#include "../Cont/TVector.h"
#include "../Templ/TComparer.h"
#include <tuple>
#include <type_traits>

/**
* TODO Balancing:
* 1. Color the root node appropriately in the AddNewNode
* 2. Helper operations
* 2.1. MoveSubtree
* 2.2.  
* 3. Implement Rotate operations
* 4. Balancing function
* 5. Checker for tree balance state.
* 6. Write tests
*
* TODO Key/Value iterator:
* 1. Insertion while iterating
* 2. Backward iteration
*
* TODO:
* 1. Create the Stack-overflow unit-test for traverse;
*
* TODO First:
* 1. Add.
* 1.1. Implement balancing
* 2. Remove.
*
* TODO Third:
* 1. Clear()
* 2. CopyTo(TArray) function.
*/

/**
* Red-black tree.
*
* WARNING!!! This implementation does NOT automatically frees the node memory.
*/
template<class KVTypeArg, class ComparerArg = TComparer<typename KVTypeArg::KeyType>>
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
	class TGeneralIterator;

	using IteratorType = TGeneralIterator<ThisType>;
	using ConstIteratorType = TGeneralIterator<const ThisType>;
	
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
		return Find(InKey, ComparerArg());
	}

	/**
	* Searches Key-Value pair by the given key using the given comparer.
	*
	* @Returns: Pointer to the Key-Value pair (or nullptr, if NOT found).
	*/
	template<class SearchKeyType, class ComparerType>
	const KeyValueType* Find(const SearchKeyType& InKey, ComparerType InComparer) const
	{
		if (Empty())
		{
			return nullptr;
		}

		TRBTreeImpl::ChildNodeRef NodeRef = TRBTreeImpl::ChildNodeRef::Invalid();
		bool bFound = FindNode(InKey, /*OutNodeRef*/ NodeRef, InComparer);
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
		bool const bAdded = AddNewNode(KeyValueType{InKey,InValue}, /*Out*/ NodeRef);
		if (bAdded && Num() >= 4)
		{
			FixupRedBlackAfterAdd(NodeRef);
		}
		// Uncomment for testing purposes only (will greately slow):
		BOOST_ASSERT_MSG(DebugCheckValid(), "TRBTree::Add: tree state must be valid");
		return bAdded;
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

	/**
	* Checks that tree is in the correct state.
	* Slow! To be called on the unit-testing stage only.
	*
	* Checks that: 
	* - Tree is correct;
	* - Red-black invariants hold;
	*/
	bool DebugCheckValid()
	{
		if (Num() == 0) { return true; }

		if ( GetNode(RootIdx)->IsRed() ) { return false; }

		struct SubtreeContext
		{
			TRBTreeImpl::ChildNodeRef NodeRef;
			const SubtreeContext* pParent = nullptr;
			const SubtreeContext* pMaxAncestor = nullptr;
			const SubtreeContext* pMinAncestor = nullptr;

			/**
			* Count of black nodes on the path from the root to this node. 
			* Nil nodes are excluded.
			*/
			int BlackPathLength = 0;

			bool IsRoot() const { return NodeRef.IsRoot(); }

			const KeyType* GetMinKey(const TRBTree* pTree) const
			{
				return pMinAncestor ? &pMinAncestor->GetKey(pTree) : nullptr;
			}

			const KeyType* GetMaximKey(const TRBTree* pTree) const
			{
				return pMaxAncestor ? &pMaxAncestor->GetKey(pTree) : nullptr;
			}

			const KeyType& GetKey(const TRBTree* pTree) const
			{
				return GetNode(pTree)->GetKey();
			}

			const KeyType& GetParentKey(const TRBTree* pTree) const
			{
				BOOST_ASSERT(pParent);
				return pParent->GetKey(pTree);
			}
			const NodeType* GetNode(const TRBTree* pTree) const { return pTree->GetNode(NodeRef); }
			const SubtreeContext* GetGrandpa() const
			{
				if (IsRoot() || pParent->IsRoot())
				{
					return nullptr; 
				}
			 	return pParent->pParent;
			}

			SubtreeContext
			(
				TRBTreeImpl::ChildNodeRef InNodeRef, const SubtreeContext* pInParent,
				const SubtreeContext* pInMinAncestor,
				const SubtreeContext* pInMaxAncestor,
				int InBlackPathLength
			) :
				NodeRef(InNodeRef)
			,	pParent(pInParent) 
			,	pMaxAncestor(pInMaxAncestor)
			,	pMinAncestor(pInMinAncestor)
			,	BlackPathLength(InBlackPathLength)
			{
				BOOST_ASSERT(pParent || InNodeRef.IsRoot());
				BOOST_ASSERT(pMaxAncestor || pMinAncestor || InNodeRef.IsRoot());
			}
		};

		/**
		* Points to the node that is a leaf of the reference black path node.
		*/
		const SubtreeContext* pReferenceBlackPath = nullptr;
		/**
		* Contains a context for each iterated element.
		*/
		TVector<SubtreeContext> Way;
		Way.ReserveGrow(Num());
		Way.Push(SubtreeContext{ TRBTreeImpl::ChildNodeRef::RootNode(), nullptr, nullptr, nullptr, 0 });
		for (int CurrNodeIdx = 0; CurrNodeIdx < Num(); CurrNodeIdx++)
		{
			const SubtreeContext* pNode = &Way[CurrNodeIdx];

			bool bLeaf = true;
			for (TRBTreeImpl::NodeChildIndex childIdx = TRBTreeImpl::LEFT_CHILD_IDX; childIdx <= TRBTreeImpl::RIGHT_CHILD_IDX; childIdx++)			
			{
				TRBTreeImpl::ChildNodeRef ChildRef = GetChildNodeRef(pNode->NodeRef, childIdx);
				if (NodeExists(ChildRef))
				{
					bLeaf = false;

					const SubtreeContext* pChildMinAncestor = nullptr;
					const SubtreeContext* pChildMaxAncestor = nullptr;
					if (childIdx == TRBTreeImpl::LEFT_CHILD_IDX)
					{
						pChildMaxAncestor = pNode;
						pChildMinAncestor = pNode->pMinAncestor;
					}
					else
					{
						pChildMinAncestor = pNode;
						pChildMaxAncestor = pNode->pMaxAncestor;
					}

					int BlackPathLength = pNode->BlackPathLength;
					if (GetNode(ChildRef)->IsBlack())
					{
						BlackPathLength++;
					}

					SubtreeContext* pChildContext = Way.Push
					(
						SubtreeContext
						{ 
							ChildRef,
							pNode, pChildMinAncestor, pChildMaxAncestor,
							BlackPathLength
						}
					);

					// Check that child context is valid
					const KeyType* pChildKey = &pChildContext->GetKey(this);
					const KeyType* pMinKey = pChildContext->GetMinKey(this);
					const KeyType* pMaximKey = pChildContext->GetMaximKey(this);
					if ( pMinKey && ( *pChildKey  < *pMinKey) )
					{
						return false;
					}

					if ( pMaximKey && (*pChildKey > *pMaximKey) )
					{
						return false;
					}
				}
			}

			if (bLeaf)
			{
				if (pReferenceBlackPath)
				{
					bool bBlackPathValid = (pNode->BlackPathLength == pReferenceBlackPath->BlackPathLength);
					if ( ! bBlackPathValid)
					{
						return false;
					}
				}
				else
				{
					pReferenceBlackPath = pNode;
				}
			}
		}
		
		return true;
	}

private:
	friend class NodeIterator;

	using NodeType = TRBTreeImpl::Node<KVTypeArg>;

public:
	/**
	* Iterates KeyValue pairs in the order of their keys.
	*/
	template<class TreeTypeArg>
	class TGeneralIterator
	{
	public:
		template<class OtherTreeType>
		friend class TGeneralIterator;

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
		TGeneralIterator(TreeTypeArg *pInTree, TRBTreeImpl::ChildNodeRef InNodeRef) :
			pTree{ pInTree }
		,	NodeRef{ InNodeRef }
		{
			BOOST_ASSERT(pTree);
		}

		/**
		* Copy constructs.
		*/
		template<class OtherTreeType>
		TGeneralIterator(const TGeneralIterator<OtherTreeType>& InOther) :
			NodeRef{TRBTreeImpl::ChildNodeRef::Invalid()}
		{
			*this = InOther;
		}

		/**
		* Copies.
		*/
		template<class OtherTreeType>
		TGeneralIterator& operator=(const TGeneralIterator<OtherTreeType>& InOther)
		{
			pTree = InOther.pTree;
			NodeRef = InOther.NodeRef;
			return *this;
		}

		/**
		* Returns End iterator.
		*/
		static TGeneralIterator EndIterator(TreeTypeArg *pInTree)
		{
			return TGeneralIterator(pInTree, TRBTreeImpl::ChildNodeRef::Invalid());
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
		TGeneralIterator& operator++()
		{
			AdvanceNext();
			return *this;
		}

		/**
		* Advances iterator to the next KeyValue pair.
		*/
		TGeneralIterator operator++(int)
		{
			TGeneralIterator OldIt = *this;
			TGeneralIterator::operator++();
			return OldIt;
		}

		/**
		* Advances an iterator to previous KeyValue pair.
		*/
		TGeneralIterator& operator--();

		/**
		* Advances iterator to the previus KeyValue pair.
		*/
		TGeneralIterator operator--(int);

		template<class OtherTreeType>
		bool operator==(TGeneralIterator<OtherTreeType> B)
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
		bool operator!=(TGeneralIterator<OtherTreeType> B)
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
	template<class SearchKeyType, class Comparer>
	bool FindNode
	(
		const SearchKeyType& InKey, TRBTreeImpl::ChildNodeRef& OutNodeRef, Comparer InComparer
	) const
	{
		BOOST_ASSERT_MSG( ! Empty(), "TRBTree: FindNode: Container must be non-empty" );

		OutNodeRef = TRBTreeImpl::ChildNodeRef::RootNode();
		NodeIterator It { this, OutNodeRef };

		while (true)
		{
			const KeyType* pCurrKey = &(It.GetNode()->GetKey());

			int CompareResult = InComparer.Compare(*pCurrKey, InKey);
			
			if (CompareEqual(CompareResult))
			{
				return true;
			}

			if(CompareGreater(CompareResult))
			{
				It = It.GetLeft();
			}
			else if(CompareLess(CompareResult))
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
			GetNode(RootIdx)->MakeBlack();
			return true;
		}

		if ( FindNode(InKV.Key, /*OutNodeRef*/OutChildNodeRef, ComparerArg()) )
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
	* Fixups the properties of the Red-black tree after the new node addition.
	*/
	void FixupRedBlackAfterAdd(TRBTreeImpl::ChildNodeRef NodeRef)
	{
		BOOST_ASSERT_MSG(Num()>= 4, "The tree must already contain at least 4 nodes (including the new added one) before the fixup operation");
		while (true)
		{
			TRBTreeImpl::ChildNodeRef ParentRef = GetParentNodeRef(NodeRef);

			if ( ! NodeExists(ParentRef) )
			{
				return;
			}

			// NOTE: WE skip the check that parent is root, because if root, then by definition black.
			if (/*ParentRef.IsRoot()|| */ GetNode(ParentRef)->IsBlack())
			{
				return;
			}

			// NOTE: Grandpa always exists because parent is not a root node.
			TRBTreeImpl::ChildNodeRef GrandpaRef = GetParentNodeRef(ParentRef);
			TRBTreeImpl::ChildNodeRef UncleRef = GetChildNodeRef(GrandpaRef, TRBTreeImpl::InvertChildIndex(ParentRef.ChildIdx));
			if ( ( ! NodeExists(UncleRef) ) || GetNode(UncleRef)->IsBlack())
			{
				// At this point both parent and the new node are red,
				// and we cannot perform recoloring because uncle is black.
				NodeRef = RotateSubtree(NodeRef, ParentRef, GrandpaRef);
				if (NodeRef.IsRoot())
				{
					return;
				}
			}
			else
			{
				// At this point both parent and the new node are black,
				// and the is uncle is red, so we can perform recoloring.
				GetNode(ParentRef)->MakeBlack();
				GetNode(UncleRef)->MakeBlack();
				if ( ! GrandpaRef.IsRoot() )
				{
					GetNode(GrandpaRef)->MakeRed();
					NodeRef = GrandpaRef;
				}
				else
				{
					return;
				}
			}
		}
	}

	/**
	* Handles all rotation cases.
	* Returns reference to the node to be visited next.
	*/
	TRBTreeImpl::ChildNodeRef RotateSubtree(TRBTreeImpl::ChildNodeRef NodeRef, TRBTreeImpl::ChildNodeRef ParentRef, TRBTreeImpl::ChildNodeRef GrandpaRef)
	{
		int const NodeIndex = GetNodeIndex(NodeRef);
		if (NodeRef.ChildIdx == ParentRef.ChildIdx)
		{
			return Rotate_SameChildIdx(NodeIndex, GrandpaRef, NodeRef.ChildIdx);
		}
		else
		{
			return Rotate_DifferentChildIdx(NodeIndex, GrandpaRef, NodeRef.ChildIdx);
		}
	}

	/**
	* Handles rotation case where new node and its parent
	* are left and right childs or vice versa.
	*/
	TRBTreeImpl::ChildNodeRef Rotate_DifferentChildIdx(TRBTreeImpl::NodeIndex NodeIndex, TRBTreeImpl::ChildNodeRef GrandpaRef, TRBTreeImpl::NodeChildIndex ChildIdx)
	{
		TRBTreeImpl::NodeChildIndex const InvertedChildIdx = TRBTreeImpl::InvertChildIndex(ChildIdx);

		TRBTreeImpl::ChildNodeRef const OldParentRef = GetChildNodeRef(GrandpaRef, InvertedChildIdx);
		TRBTreeImpl::ChildNodeRef const OldNewNodeRef = GetChildNodeRef(OldParentRef, ChildIdx);
		// Child of the new node with childIdx equal to the child idx of the new node itself.
		TRBTreeImpl::ChildNodeRef const ChildOfNewRef = GetChildNodeRef(OldNewNodeRef, ChildIdx);
		TRBTreeImpl::ChildNodeRef const OtherChildOfNewRef = GetChildNodeRef(OldNewNodeRef, InvertedChildIdx);

		int const OldParentIdx = GetNodeIndex(OldParentRef);
		int const OldGrandpaIdx = GetNodeIndex(GrandpaRef);
		int const OldChildOfNewIdx = GetNodeIndex(ChildOfNewRef);
		int const OldOtherChildOfNewIdx = GetNodeIndex(OtherChildOfNewRef);

		GetNode(OldGrandpaIdx)->MakeRed();
		GetNode(NodeIndex)->MakeBlack();

		LinkToNewParentByNewReference(NodeIndex, GrandpaRef);
		LinkToNewParentByNewReference(OldParentIdx, OtherChildOfNewRef);
		LinkToNewParentByNewReference(OldGrandpaIdx, ChildOfNewRef);
		LinkToNewParentByNewReference(OldOtherChildOfNewIdx, OldNewNodeRef);
		LinkToNewParentByNewReference(OldChildOfNewIdx, OldParentRef);

		return GrandpaRef;
	}

	/**
	* Handles rotation case where both the new node and its parent
	* are both left or right childs.
	*/
	TRBTreeImpl::ChildNodeRef Rotate_SameChildIdx(TRBTreeImpl::NodeIndex NodeIndex, TRBTreeImpl::ChildNodeRef GrandpaRef, TRBTreeImpl::NodeChildIndex ChildIdx)
	{
		TRBTreeImpl::ChildNodeRef const OldParentRef = GetChildNodeRef(GrandpaRef, ChildIdx);
		TRBTreeImpl::ChildNodeRef const BrotherRef = GetChildNodeRef(OldParentRef, TRBTreeImpl::InvertChildIndex(ChildIdx));
		int const OldParentIdx = GetNodeIndex(OldParentRef);
		int const OldGrandpaIdx = GetNodeIndex(GrandpaRef);
		int const OldBrotherIdx = GetNodeIndex(BrotherRef);

		GetNode(OldGrandpaIdx)->MakeRed();
		GetNode(OldParentIdx)->MakeBlack();
		
		LinkToNewParentByNewReference(OldParentIdx, GrandpaRef);
		LinkToNewParentByNewReference(OldBrotherIdx, OldParentRef);
		LinkToNewParentByNewReference(OldGrandpaIdx, BrotherRef);

		return GrandpaRef;
	}

	/**
	* Relocates node, so that it can be accessed by a new reference.
	*
	* Correctly setups link from node to the new parent, 
	* and from the new parent's child to the node.
	* However, link from old parent to old child is NOT invalidated.
	*
	* Function works correctly if the NodeIdx is INDEX_NONE (meaning that no node exist).
	*/
	void LinkToNewParentByNewReference(TRBTreeImpl::NodeIndex NodeIdx, TRBTreeImpl::ChildNodeRef NewRef)
	{
		if (NodeIdx != INDEX_NONE)
		{
			NodeType* const pNode = GetNode(NodeIdx);
			pNode->ParentIdx = NewRef.ParentIdx;
		}
		if (NewRef.IsRoot())
		{
			RootIdx = NodeIdx;
		}
		else
		{
			GetNode(NewRef.ParentIdx)->SetChild(NewRef.ChildIdx, NodeIdx);
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
	* Returns index of the node by the given reference.
	*
	* @returns: index of the node, or INDEX_NONE, if no node by the given ref.
	*/
	TRBTreeImpl::NodeIndex GetNodeIndex(const TRBTreeImpl::ChildNodeRef InNodeRef) const
	{
		if (InNodeRef.IsRoot())
		{
			return RootIdx;
		}
		
		return GetParentNode(InNodeRef)->GetChild(InNodeRef.ChildIdx);
	}

	/**
	* Gets parent node reference.
	*/
	TRBTreeImpl::ChildNodeRef GetParentNodeRef(const TRBTreeImpl::ChildNodeRef InNodeRef) const
	{
		const TRBTreeImpl::NodeChildIndex ParentIdx = InNodeRef.ParentIdx;
		const NodeType* const pParent = GetParentNode(InNodeRef);
		BOOST_ASSERT_MSG(pParent, "TRBTree::GetParentNodeRef: parent must exist");
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

