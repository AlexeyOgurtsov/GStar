#pragma once

#include "Core/CoreSysTypes.h"

namespace UT
{
	/**
	* Fills the given region of bytes with zeroes.
	*/
	void ZeroBytes(void* ptr, unsigned int InSize);
} // UT