#pragma once

#include <memory>
#include "Core/Templ/Pointer.h"

template<class T>
using TSharedPtr = std::shared_ptr<T>;

template<class T>
using TWeakPtr = std::weak_ptr<T>;

template<class T> struct DereferenceType<TSharedPtr<T>>
{
	using Type = T;
};

template<class T> struct IsSmartPointer<TSharedPtr<T>>
{
	static constexpr bool Value = true;
};

template<class T> struct IsSmartPointer<TWeakPtr<T>>
{
	static constexpr bool Value = true;
};

template<class T> struct IsSharedPointer<TSharedPtr<T>>
{
	static constexpr bool Value = true;
};

template<class T> struct IsWeakPointer<TWeakPtr<T>>
{
	static constexpr bool Value = true;
};

template<class T> struct WeakDereferenceType<TWeakPtr<T>>
{
	using Type = T;
};