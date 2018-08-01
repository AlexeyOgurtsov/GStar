#pragma once

/**
* Contains a set of minimal often-used includes to be used by almost any header.
*
* WARNING!!! Never should include ever-high-level classes, ever strings
*/

/**
* Assertions are to be used everywhere.
*/
#include <boost/assert.hpp>

/**
* Serialization support is to be provided by default by any class.
*/
#include <boost/serialization/access.hpp>

/**
* Needed because of std::hash.
*/
#include <functional>

#include "CoreSysTypes.h"

