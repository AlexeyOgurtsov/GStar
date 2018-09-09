#pragma once

#include <type_traits>

/**
* Returns a type of a derefence operator (*) of the given type
* (void if dereference is not supported).
*
* WARNING!!! Void for weak pointer.
*/
template<class T> struct DereferenceType
{
	using Type = void;
};

/**
* Are two base types of pointers are assignable.
*/
template<class T, class OtherT> struct ArePointerBaseTypesAssignable
{
	static constexpr bool Value = (std::is_const<T>::value || ( ! std::is_const<OtherT>::value ) );
};

/**
* Returns a type of the weak pointer dereference.
* (void if not a weak pointer).
*/
template<class T> struct WeakDereferenceType
{
	using Type = void;
};

template<class T> struct IsCPointer
{
	static constexpr bool Value = false;
};

template<class T> struct IsCPointerToConst
{
	static constexpr bool Value = false;
};

/**
* WARNING!!! Weak pointer is ALSO smart pointer!!!
*/
template<class T> struct IsSmartPointer
{
	static constexpr bool Value = false;
};

template<class T> 
struct IsSmartPointerToConst
{
	static constexpr bool Value = 
		IsSmartPointer<T>::Value && 
		( std::is_const<DereferenceType<T>::Type>::value || std::is_const<WeakDereferenceType<T>::Type>::value);
};

template<class T> struct IsAnyPointer
{
	static constexpr bool Value = IsCPointer<T>::Value || IsSmartPointer<T>::Value;
};

template<class T> struct IsAnyPointerToConst
{
	static constexpr bool Value = IsCPointerToConst<T>::Value || IsSmartPointerToConst<T>::Value;
};

template<class T> struct IsCPointerToMutable
{
	static constexpr bool Value = ( ! IsCPointerToConst<T>::Value );
};

template<class T> struct IsSmartPointerToMutable
{
	static constexpr bool Value = IsSmartPointer<T>::Value &&  ( ! IsSmartPointerToConst<T>::Value );
};

template<class T> struct IsAnyPointerToMutable
{
	static constexpr bool Value = ( ! IsAnyPointerToConst<T>::Value );
};

template<class T> struct IsUniquePointer
{
	static constexpr bool Value = false;
};

template<class T> struct IsSharedPointer
{
	static constexpr bool Value = false;
};

template<class T> struct IsWeakPointer
{
	static constexpr bool Value = false;
};

/**
* Is smart pointer, but NOT weak.
*/
template<class T> struct IsStrongSmartPointer
{
	static constexpr bool Value = IsSmartPointer<T>::Value && ( ! IsWeakPointer<T>::Value );
};

template<class T> struct IsStrongSmartPointerToConst
{
	static constexpr bool Value = IsSmartPointerToConst<T>::Value && ( ! IsWeakPointer<T>::Value );
};

template<class T> struct IsAnyStrongPointer
{
	static constexpr bool Value = IsAnyPointer<T>::Value && ( ! IsWeakPointer<T>::Value );
};

template<class T> struct IsStrongSmartPointerToMutable
{
	static constexpr bool Value = IsSmartPointerToMutable<T>::Value && ( ! IsWeakPointer<T>::Value );
};

template<class T> struct DereferenceType<T*>
{
	using Type = T;
};

template<class T> struct IsCPointer<T*>
{
	static constexpr bool Value = true;
};

template<class T> struct IsCPointerToConst<const T*>
{
	static constexpr bool Value = true;
};