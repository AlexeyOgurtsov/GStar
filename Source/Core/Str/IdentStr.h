#pragma once

#include <cstdio>
#include <cstdlib>
#include <boost/serialization/access.hpp>
#include <boost/assert.hpp>

/**
* TO-BE-CHECKED-COMPILED.
* TO-BE-TESTED.
*
* TODO: Provide the storage class for storing IdentStr instances and the default instance of it.
*/

/**
* Identifier string.
* Immutable ANSI string. 
*
* WARNING 1: NOT a C-style str (NO terminating zero)!!!
* WARNING 2: NOT automatically copied to some internal storage by the constructor!!!
*
* Reference semantics:
* - To be passed by value;
* - Compares with itself by reference, NOT by value;
* - Copy operation is allowed but copies only pointer to string, never copies the string itself;
*/
class IdentStr
{
public:
	/**
	* Maximal length allowed for the identifier.
	* We use such a great value, so filesystem paths can also be treated as identifiers.
	*/
	constexpr static int MAX_LENGTH = 1024;

	/**
	* Constructs an invalid identifier (equivalent of the nullptr).
	*/
	__FORCEINLINE constexpr IdentStr() : Ptr(nullptr), Length(0) {}

	/**
	* Constructs an identifier by the given C-str ptr.
	*/
	__FORCEINLINE explicit IdentStr(const char* InCStr) : IdentStr(InCStr), strlen(InCStr) {}

	/**
	* Constructs an identifier by the given pointer and length.
	*/
	constexpr IdentStr(const char* InPtr, int InLength) : Ptr(InPtr), Length(static_cast<short>(InLength))
	{
		BOOST_ASSERT_MSG(InPtr, "nullptr should NOT be passed to the IdentStr constructor");
		BOOST_ASSERT_MSG(InLength >= 0, "Negative length is passed to the IdentStr constructor");
		BOOST_ASSERT_MSG(InLength <= MAX_LENGTH, "IdentStr is too long");
	}

	/**
	* Copy constructs by reference.
	*/
	IdentStr(const IdentStr InOther) : Ptr(InOther.Ptr), Length(InOther.Length) {}

	/**
	* Copies by reference.
	*/
	IdentStr& operator=(const IdentStr InOther)
	{
		Ptr = InOther.Ptr;
		Length = InOther.Length; 
		return *this; 
	}

	/**
	* Checks whether this instance represents a valid identifier.
	*/
	__FORCEINLINE bool Valid() const { return nullptr != Ptr; }

	/**
	* Returns length.
	*
	* WARNING!!! We delibarately convert to int to avoid additional compiler warnings.
	*/
	__FORCEINLINE constexpr int Length() const { return Length; }

	/**
	* Checks for emptiness.
	*/	
	__FORCEINLINE constexpr bool Empty() const { return 0 == Length; }

	/**
	* Copies the substring to buffer.
	* WARNING!!! C-str termintator is NOT appended automatically!!!
	*/
	void CopySubstrToBuffer(int InStartSrcIndex, char* pOutBuffer, int InMaxBufferLength, int InLengthToCopy = Length)
	{
		BOOST_ASSERT_MSG(InStartIndex >= 0, "IdentStr: SubstrToBuffer: Negative start src index");
		BOOST_ASSERT(pOutBuffer);
		BOOST_ASSERT_MSG(InStartSrcIndex + InLengthToCopy <= Length, "IdentStr: SubstrToBuffer: substring to copy is beyond the length");
		BOOST_ASSERT_MSG(InLengthToCopy <= InMaxBufferLength);
		memcpy(pOutBuffer, Ptr + InStartSrcIndex, InLengthToCopy);
	}

	friend class boost::serialization::access;

	/**
	* boost::serialization support.
	*/
	template<class Ar>
	void serialize(Ar& InAr, const unsigned int InVersion)
	{
		InAr & Length;
		InAr & Ptr;
	}
	
private:
	const char* Ptr;
	short Length;
};

/**
* Checking for equality by comparing references.
*/
inline bool operator==(const IdentStr A, const IdentStr B)
{
	return A.ptr == B.ptr;
}

/**
* Checking for NON=equality by comparing references.
*/
inline bool operator!=(const IdentStr A, const IdentStr B)
{
	return ! operator==(A, B);
}

/**
* std streams output
*/
template<class Strm> Strm& operator<<(Strm& Strm, const IdentStr S)
{
	return Strm << S;
}