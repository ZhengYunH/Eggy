#pragma once
#include "Core/Foundation.h"
#define UT_EXPECT_EQ(X, ...) if(!X){ LOG(Test, Info) << "UnitTest Fail"; }
#define UT_ASSERT(X) HYBRID_CHECK(X)

namespace Eggy::UnitTest
{

}