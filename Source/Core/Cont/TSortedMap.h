#pragma once

#include "TRBTree.h"

template<class KVType, class ComparerArg = TComparer<typename KVTypeArg::KeyType>>
using TSortedMap = TRBTree<KVType, ComparerArg>; 