#pragma once

/**
* Contains a set of minimal often-used includes to be used by almost any header.
*
* WARNING!!! Never should include ever-high-level classes, ever strings
*/

#include <initializer_list>

/**
* Assertions are to be usable everywhere.
*/
#include "Assert.h"

/**
* Serialization support is to be provided by default by almost any class.
*/
#include <boost/serialization/access.hpp>

/**
* Needed because of std::hash.
*/
#include <functional>

/**
* Type traits are needed for almost any class.
*/
#include <type_traits>

/**
* Stream traits are needed almost anywhere,
* because any class should implement operator<<.
*/
#include "Stream/StreamTraits.h"

#include "CoreSysTypes.h"

