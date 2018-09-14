#pragma once

#include "Priv/TypeTraitsImpl.h"

/**
* By default all type traits DO REMOVE all CV qualifiers and Ref.
*
* If you want to keep the qualifiers, you need the Exact* version of the trait.
*/

/**
* Returns true if the given type has const qualifier.
*
* Only ref qualifiers are removed (of course).
*/
template<class T>
using IsConstType = TypeTraitsImpl::WrapValueTypeTrait<std::is_const, typename RemoveRef<T>::Type>;

/**
* Returns true if the given type can be default-constructed as T{}.
*
* Qualifiers are NOT removed!
*/
template<class T>
using ExactDefaultConstructible = TypeTraitsImpl::WrapValueTypeTrait<std::is_default_constructible, T>;


/**
* Returns true if the given type can be default-constructed as T{}
* and It's constructor is trivial.
*
* Qualifiers are NOT removed!
*/
template<class T>
using ExactTrivialDefaultConstructible = TypeTraitsImpl::WrapValueTypeTrait<std::is_trivially_default_constructible, T>;


/**
* Returns true if the given type has a trivial destructor.
*
* Qualifiers are NOT removed!
*/
template<class T>
using ExactTrivialDestructible = TypeTraitsImpl::WrapValueTypeTrait<std::is_trivially_destructible, T>;

/**
* Returns true if the given type can be copy-constructed as T{const T&}.
*
* Qualifiers are NOT removed!
*/
template<class T>
using ExactCopyConstructible = TypeTraitsImpl::WrapValueTypeTrait<std::is_copy_constructible, T>;

/**
* Returns true if the given type can be copy-constructed as T{const T&}
* and It's constructor is trivial.
*
* Qualifiers are NOT removed!
*/
template<class T>
using ExactTrivialCopyConstructible = TypeTraitsImpl::WrapValueTypeTrait<std::is_trivially_copy_constructible, T>;

/**
* Returns true if the given type can be copy-assigned as T={const T&}.
*
* Qualifiers are NOT removed!
*/
template<class T>
using ExactCopyAssignable = TypeTraitsImpl::WrapValueTypeTrait<std::is_copy_assignable, T>;

/**
* Returns true if the given type can be copy-assigned as T={const T&}
* and It's constructor is trivial.
*
* Qualifiers are NOT removed!
*/
template<class T>
using ExactTrivialCopyAssignable = TypeTraitsImpl::WrapValueTypeTrait<std::is_trivially_copy_assignable, T>;

/**
* Compares exact types (qualifiers are NOT removed).
*/
template<class T, class OtherT>
using ExactTypeEqual = TypeTraitsImpl::WrapValueTypeTraitTwo<std::is_same, T, OtherT>;

/**
* Compares types with const qualifiers (but reference qualifiers are removed).
*/
template<class T, class OtherT>
struct TypeEqualWithConst
{
	static constexpr bool Value = ExactTypeEqual
	<
		typename RemoveRef<T>::Type,
		typename RemoveRef<OtherT>::Type
	>::Value;
};

/**
* Compares types with removing their qualifiers (CV qualifiers and ref).
*/
template<class T, class OtherT>
struct TypeEqual
{
	static constexpr bool Value = ExactTypeEqual
	<
		typename RemoveRefCV<T>::Type, 
		typename RemoveRefCV<OtherT>::Type
	>::Value;
};

/**
* TODO:
* 1. Unit-test
*/