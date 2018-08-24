#pragma once

#include <type_traits>

/**
* Fwd declarations of archives.
*/
namespace boost::serialization
{
	class text_oarchive;
	class text_iarchive;
} // boost::serialization
 

/**
* Contains traits for Serialization archive classes and there specializations for native archives.
*/

/**
* Determines whether this archive class is archive
*/
template<class T>
struct IsArchive
{
	static constexpr bool Value = false;
};


template<> struct IsArchive<boost::serialization::text_oarchive>
{
	static constexpr bool Value = true;
};

template<> struct IsArchive<boost::serialization::text_iarchive>
{
	static constexpr bool Value = true;
};

