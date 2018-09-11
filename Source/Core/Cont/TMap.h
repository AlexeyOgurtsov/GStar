#pragma once

#include "TSortedMap.h" // @TODO: Remove after real hash map is implemented.

/**
* Hash map.
*
* @TODO: WARNING!!! We temporarily use TSortedSet until NOT implemented.
*/
template<class KVTypeArg>
using TMap = TSortedMap<KVTypeArg>;