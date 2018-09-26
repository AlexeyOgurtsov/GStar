#pragma once

#include "TRBTree/TRBTreeTestSet.h"
#include "TRBTree/TRBTreeTestMoving.h"
#include "TRBTree/TRBTreeTestExtraOps.h"
#include "TRBTree/TRBTreeTestMemorySuite.h"
#include "TRBTree/TRBTreeTestSerialization.h"
#include "TRBTree/TRBTreeTestMinimalMain.h"
#include "TRBTree/TRBTreeTestMinimalAddSuite.h"
#include "TRBTree/TRBTreeTestMinimalRemoveSuite.h"
#include "TRBTree/TRBTreeTestMinimalIterationSuite.h"
#include "TRBTree/TRBTreeTestFindSuite.h"
#include "TRBTree/TRBTreeTestMinimalCopySuite.h"
#include "TRBTree/TRBTreeTestCustomCompare.h"

namespace
{

BOOST_AUTO_TEST_SUITE(Core)
BOOST_AUTO_TEST_SUITE(Container)
BOOST_AUTO_TEST_SUITE(TRBTreeTestSuite)

BOOST_AUTO_TEST_SUITE_END() // TRBTree
BOOST_AUTO_TEST_SUITE_END() // Container
BOOST_AUTO_TEST_SUITE_END() // Core

} // anonymous
