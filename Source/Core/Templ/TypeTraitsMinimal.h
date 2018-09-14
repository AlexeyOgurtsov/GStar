#pragma once

/**
* Minimal type traits that are used ever in the implementation of the TypeTraits itself.
*/

#include "Priv/TypeTraitsMinimalImpl.h"

template<class T>
using RemoveRef = TypeTraitsImpl::WrapTypeTypeTrait<std::remove_reference, T>;

template<class T>
using RemoveCV = TypeTraitsImpl::WrapTypeTypeTrait<std::remove_cv, T>;

template<class T>
using RemoveConst = TypeTraitsImpl::WrapTypeTypeTrait<std::remove_const, T>;

template<class T>
using RemoveVolatile = TypeTraitsImpl::WrapTypeTypeTrait<std::remove_volatile, T>;

template<class T>
struct RemoveRefCV
{
	using Type = typename RemoveCV<typename RemoveRef<T>::Type>::Type;
};