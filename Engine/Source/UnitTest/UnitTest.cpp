#include "UnitTest.h"
#include "DP_UnitTest.h"
#include "SharedObject_UnitTest.h"


namespace Eggy::UnitTest
{
	void TestAll()
	{
		DP_UnitTest::Test();
		SharedObject_UnitTest::Test();
	}

}

