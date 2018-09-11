#pragma once

#include <type_traits>

template<class T>
struct RemoveRef
{
	using Type = typename std::remove_reference<T>::type;
};

template<class T>
struct RemoveCV
{
	using Type = typename std::remove_cv<T>::type;
};

template<class T>
struct RemoveRefCV
{
	using Type = typename RemoveCV<typename RemoveRef<T>::Type>::Type;
};