#pragma once

#include <memory>
#include "Core/Templ/Pointer.h"

template<class T, class Deleter = std::default_delete<T>>
using TUniquePtr = std::unique_ptr<T, Deleter>;

template<class T> struct DereferenceType<TUniquePtr<T>>
{
	using Type = T;
};

template<class T, class Deleter> struct IsSmartPointer<TUniquePtr<T, Deleter>>
{
	static constexpr bool Value = true;
};

template<class T, class Deleter> struct IsUniquePointer<TUniquePtr<T, Deleter>>
{
	static constexpr bool Value = true;
};

