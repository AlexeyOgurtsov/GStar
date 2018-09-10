#pragma once

/**
* Contains common types used by all container classes (e.g. type aliases, enums).
* And includes some common headers.
*/

#include "Core/CoreSysMinimal.h"
#include "UT/IteratorUtils.h"
#include "Core/Serialization/ArchiveTraits.h"
#include "Core/Stream/StreamTraits.h"
#include <cstdint>
#include <cassert>
#include <cstdlib>
#include <utility>
#include <algorithm>
 // These are the string classes that we support now.
#include <string_view>
#include <string>
#include <iomanip>


/**
* Value typically to be returned by container functions to indicate that element does not exist.
*/
constexpr int32_t INDEX_NONE = -1;

/**
* Special tag to be passed to force capacity initialization.
*/
enum class EForceInitCapacity 
{
	Value
};