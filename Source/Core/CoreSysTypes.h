#pragma once

#include <cstdint>

using Byte = uint8_t;

/**
* Special tag to be passed to force initialization.
*/
enum class EForceInit 
{
	Value
};

/**
* Special tag to be passed to force moving (for example, when moving pointer).
*/
enum class EForceMove
{
	Value
};