#pragma once

#include <boost/assert.hpp>

/**
* Asserts in normal conditions.
*/
#define TODO_IMPL(InCond, InMsg) { BOOST_ASSERT_MSG((InCond), (InMsg)); } 

/**
* Does not assert in normal conditions (if no special macros defined).
*/
#define TODO_IMPL_SILENT(InCond, InMsg) { } 