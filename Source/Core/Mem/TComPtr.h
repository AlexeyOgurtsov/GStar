#pragma once

#include "Core/CoreSysMinimal.h"
#include "Core/Templ/Pointer.h"

/**
* Pointer to COM object.
*
* Copyable and movable.
*/
template<class T>
class TComPtr
{
public:
	static_assert(std::is_base_of<IUnknown, T>::value);

	using ElementType = T;
	using IsConst = std::is_const<T>;

	friend class TComPtr;

	~TComPtr()
	{
		Release();
	}

	/**
	* Default ctor: creates pointer to nullptr.
	*/
	TComPtr() = default;

	/**
	* Constructs from pointer.
	*/
	explicit TComPtr(T* InPtr) : Ptr{InPtr} {}

	/**
	* Copies.
	*/
	TComPtr(const TComPtr& InOther) :
		Ptr(InOther.Ptr) 
	{
		AddRef();	
	}

	/**
	* Copies.
	*/
	template<class OtherT>
	TComPtr(const TComPtr<OtherT>& InOther) :
		Ptr(InOther.Ptr)
	{
		static_assert(ArePointerBaseTypesAssignable<T, OtherT>::Value, "Com ptr must be assignable");

		AddRef();
	}

	/**
	* Copy-assigns.
	*/
	TComPtr& operator=(const TComPtr& InOther)
	{
		Ptr = InOther.Ptr; 
		AddRef();
		return *this;	
	}

	template<class OtherT>
	TComPtr& operator=(const TComPtr<OtherT>& InOther)
	{
		static_assert( ArePointerBaseTypesAssignable<T, OtherT>::Value, "Com ptr must be assignable");
	
		Ptr = InOther.Ptr;
		AddRef();
		return *this;
	}

	/**
	* Moves.
	*/
	TComPtr(TComPtr&& InOther) :
		Ptr(InOther.Ptr) 
	{
		InOther.Ptr = nullptr;	
	}

	/**
	* Moves.
	*/
	template<class OtherT>
	TComPtr(TComPtr<OtherT>&& InOther) :
		Ptr(InOther.Ptr)
	{
		InOther.Ptr = nullptr;
	}

	/**
	* Move-assigns.
	*/
	TComPtr& operator=(TComPtr&& InOther)
	{
		Ptr = InOther.Ptr;
		InOther.Ptr = nullptr;
		return *this;	
	}

	/**
	* Move-assigns.
	*/
	template<class OtherT>
	TComPtr& operator=(TComPtr<OtherT>&& InOther)
	{
		Ptr = InOther.Ptr;
		InOther.Ptr = nullptr;
		return *this;
	}

	/**
	* Returns true if pointer points to existing object.
	*/
	bool IsValid() const
	{
		return ! IsNull();
	}

	/**
	* Returns true if pointer points to already-deleted object.
	*/
	bool IsNull() const
	{
		return ( nullptr == Ptr );
	}

	/**
	* Converts to bool.
	*/
	operator bool() const 
	{
		return IsValid();	
	}

	/**
	* Returns reference to the pointed-to object.
	*/
	T& Get() const
	{
		return *Ptr;
	}

	/**
	* Returns pointer to the pointed-to object.
	*/
	T* GetPtr() const
	{
		return Ptr;
	}

	/**
	* Dereference operator
	*/
	T& operator*() const 
	{
		return Get(); 
	}

	T* operator->() const
	{
		return GetPtr();
	}

	void Reset(T* InValue)
	{
		Release();
		Ptr = InValue;
	}

private:

	/**
	* Adds a new reference to the object.
	*/
	void AddRef()
	{
		if(Ptr)
		{
			const_cast<MutableType*>(Ptr)->AddRef();
		}
	}

	/**
	* Releases the object
	*/
	void Release()
	{
		if(Ptr)
		{
			if(0 == (const_cast<MutableType*>(Ptr))->Release())
			{
				Ptr = nullptr;
			}
		}
	}

	/**
	* We need mutable type to AddRef or Release.
	*/
	using MutableType = typename std::remove_const<T>::type;
	T* Ptr = nullptr;
};

template<class Strm, class T, typename = typename IsOutputStream<Strm>::Value>
Strm& operator<<(Strm& S, const TComPtr<T>& Ptr)
{
	if(Ptr.IsValid())
	{
		S << Ptr.Get();
	}
	else
	{
		S << "NULL";
	}
	return S;
}

template<class T> struct DereferenceType<TComPtr<T>>
{
	using Type = T;
};

template<class T> struct IsSmartPointer<TComPtr<T>>
{
	static constexpr bool Value = true;
};


template<class T>
bool operator==(const TComPtr<T>& A, const TComPtr<T>& B)
{
	return A.GetPtr() == B.GetPtr();
}

template<class T>
bool operator!=(const TComPtr<T>& A, const TComPtr<T>& B)
{
	return ! ( A == B );
}

template<class T>
bool operator<(const TComPtr<T>& A, const TComPtr<T>& B)
{
	return A.GetPtr() < B.GetPtr();
}

template<class T>
bool operator<=(const TComPtr<T>& A, const TComPtr<T>& B)
{
	return A.GetPtr() <= B.GetPtr();
}

template<class T>
bool operator>(const TComPtr<T>& A, const TComPtr<T>& B)
{
	return A.GetPtr() > B.GetPtr();
}

template<class T>
bool operator>=(const TComPtr<T>& A, const TComPtr<T>& B)
{
	return A.GetPtr() >= B.GetPtr();
}

namespace std
{
	template<class T>
	class hash<TComPtr<T>>
	{
	public:
		size_t operator() (const TComPtr<T>& InValue) const	
		{
			return hash<T*>() (InValue.GetPtr());
		}
	};
} // std