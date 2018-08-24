#pragma once

// Fwd declarations for istream, ostream, ifstream etc.
#include <iosfwd>

template<class T>
struct IsInputStream
{
	static constexpr bool Value = false;
};

template<class T>
struct IsOutputStream
{
	static constexpr bool Value = false;
};

template<>
struct IsInputStream<std::istream>
{
	static constexpr bool Value = true;
};

template<>
struct IsOutputStream<std::ostream>
{
	static constexpr bool Value = true;
};

template<>
struct IsInputStream<std::ifstream>
{
	static constexpr bool Value = true;
};

template<>
struct IsOutputStream<std::ofstream>
{
	static constexpr bool Value = true;
};



template<>
struct IsInputStream<std::stringstream>
{
	static constexpr bool Value = true;
};

template<>
struct IsOutputStream<std::stringstream>
{
	static constexpr bool Value = true;
};