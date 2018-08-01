#include "Core/Str/IdentStr.h"

#include <cstdio>
#include <boost/functional/hash_fwd.hpp>

bool IdentStr::IsValidIdent(const char* CStr, int& OutLength)
{
	OutLength = strlen(CStr);
	if (OutLength > MAX_LENGTH)
	{
		return false;
	}
	return true;
}

size_t IdentStr::Hash() const
{
	BOOST_ASSERT_MSG(Valid(), "IdentStr::Hash(): Hashing invalid string is forbidden");
	return boost::hash_range(Ptr, Ptr + Length);
}