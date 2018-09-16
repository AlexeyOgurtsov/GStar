#pragma once

/**
* Minimal implementation tools for the TypeTraits.
*/

#include <type_traits>

namespace TypeTraitsImpl
{
	/**
	* Single-argument type-trait that returns value.
	*/
	template<template<class> class TraitArg, class T>
	struct WrapValueTypeTrait
	{
		static constexpr decltype(TraitArg<T>::value) Value = TraitArg<T>::value;
	};

	/**
	* Two-argument type-trait that returns value.
	*/
	template<template<class, class> class TraitArg, class T, class OtherArg>
	struct WrapValueTypeTraitTwo
	{
		static constexpr decltype(TraitArg<T, OtherArg>::value) Value = TraitArg<T, OtherArg>::value;
	};

	/**
	* single-argument type-trait that returns type.
	*/
	template<template<class> class TraitArg, class T>
	struct WrapTypeTypeTrait
	{
		using Type = typename TraitArg<T>::type;
	};

	/**
	* single-argument type-trait that takes bool value and returns type.
	*/
	template<template<class> class TraitArg, bool ValueArg>
	struct WrapTypeTypeTrait_TakeBoolValue
	{
		using Type = typename TraitArg<ValueArg>::type;
	};
} // TypeTraitsImpl
