#pragma once
#include "Core/Foundation.h"
#define UT_EXPECT_EQ(X, ...) LOG("UnitTest Fail", ##__VA_ARGS__)
#define UT_ASSERT(X) HYBRID_CHECK(X)

namespace Eggy::UnitTest
{

}