#include "../MemUtils.h"
#include <windows.h>

namespace UT
{
	void ZeroBytes(void* ptr, unsigned int InSize)
	{
		::ZeroMemory(ptr, InSize);
	}
} // UT