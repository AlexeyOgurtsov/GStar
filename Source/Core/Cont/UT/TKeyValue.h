#pragma once

#include "Core/CoreSysMinimal.h"
#include <boost/functional/hash_fwd.hpp>
#include <type_traits>

/**
* TODO:
* 1. Serialization
* 1.1. Do NOT provide serialization member if either key or value are pointers.
*/

/**
* Class to be passed when NO value is associated with a key.
*
* All operators check both key and value always:
* - Equality comparison tests both key and value;
* - Hash code comparison uses both key and value;
* - operator< is NOT provided;
*/
class NoValue
{
public:
	/**
	* Boost serialization support.
	*/
	template<class ArArg> void serialize(ArArg& Ar, const unsigned int Version)
	{
		// Nothing is to be done here
	}
};
inline bool operator==(const NoValue& A, const NoValue& B) { return true; }
inline bool operator!=(const NoValue& A, const NoValue& B) { return ! operator==(A, B); }

namespace std
{
	template<>
	struct hash<NoValue>
	{
		size_t operator() (const NoValue&) const { return 1; }
	};
} // std

template<class KeyTypeArg, class ValueTypeArg>
struct KVType
{
	/**
	* Type of the key.
	*/
	using KeyType = KeyTypeArg;

	/**
	* Type of the value.
	*/
	using ValueType = ValueTypeArg;
};

template<class KVTypeArg>
struct TKeyValue
{
	/**
	* Value type
	*/
	using KVType = KVTypeArg;

	/**
	* Type of the key.
	*/
	using KeyType = typename KVTypeArg::KeyType;

	/**
	* Type of the value.
	*/
	using ValueType = typename KVTypeArg::ValueType;

	/**
	* Key.
	*/
	KeyType Key;

	/**
	* Value.
	*/
	ValueType Value;

	/**
	* Initializes both key and value with a default ctor.
	*/
	TKeyValue() = default;

	/**
	* Copy-constructs.
	*/
	TKeyValue(const TKeyValue& InOther) = default;

	/**
	* Copies.
	*/
	TKeyValue& operator=(const TKeyValue& InOther) = default;

	/**
	* Move-constructs.
	*/
	TKeyValue(TKeyValue&& InOther) = default;

	/**
	* Moves.
	*/
	TKeyValue& operator=(TKeyValue&& InOther) = default;

	/**
	* boost serialization support. 
	*/
	template<class ArType>
	void serialize(ArType& Ar, const unsigned int Version)
	{
		Ar & Key;
		Ar & Value;
	}

	/**
	* Initializes key and value with the given values.
	*/
	TKeyValue(const KeyType& InKey, const ValueType& InValue) :
		Key{InKey}
	,	Value{InValue} {}
};

/*
// TODO: Does NOT compile: Why?
template<class KeyType, class ValueType> 
TKeyValue<KVType<KeyType, ValueType>> MakeKeyValue(const KeyType& K, const ValueType& V)
{
	return TKeyValue<KVType<KeyType, ValueType>>(K, V);
}
*/

template<class Strm, class KVTypeArg>
Strm& operator<<(Strm& S, const TKeyValue<KVTypeArg>& KV)
{
	if constexpr (std::is_base_of_v<NoValue, typename KVTypeArg::ValueType>)
	{
		return S << KV.Key;
	}
	else
	{
		return S << KV.Key << ":" << KV.Value;
	}
}

template<class KVTypeArg>
bool operator==(const TKeyValue<KVTypeArg>& A, const TKeyValue<KVTypeArg>& B)
{
	return A.Key == B.Key && A.Value == B.Value;
}

template<class KVTypeArg>
bool operator!=(const TKeyValue<KVTypeArg>& A, const TKeyValue<KVTypeArg>& B)
{
	return ! (operator==(A, B));
}

namespace std
{
	template<class KVTypeArg>
	struct hash<TKeyValue<KVTypeArg>>
	{
		size_t operator() (const TKeyValue<KVTypeArg>& KV) const
		{
			size_t H = 0;
			boost::hash_combine(H, KV.Key);
			boost::hash_combine(H, KV.Value);
			return H;
		}
	};
} // std