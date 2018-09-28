#pragma once

#include "Vector/TVectorTestUtils.h"
#include "Vector/TVectorTestIteratorSuite.h"
#include "Vector/TVectorTestSerializationSuite.h"
#include "Vector/TVectorTestPODMain.h"
#include "Vector/TVectorTestPODRemove.h"
#include "Vector/TVectorTestPODConstructOrAssign.h"
#include "Vector/TVectorTestPODMemory.h"
#include "Vector/TVectorTestExtraOps.h"

#include "Vector/TVectorTestMoveOnlyMain.h"
#include "Vector/TVectorTestMoveOnlyConstructOrAssign.h"

namespace
{

BOOST_AUTO_TEST_SUITE(Core)
BOOST_AUTO_TEST_SUITE(Container)
BOOST_AUTO_TEST_SUITE(TVectorTestSuite)


BOOST_AUTO_TEST_SUITE_END() // Container
BOOST_AUTO_TEST_SUITE_END() // TVectorTestSuite
BOOST_AUTO_TEST_SUITE_END() // Core

} // anonymous