#include "Core/Math/Vector3.h"

namespace Eggy
{
	class MathTester
	{
	public:
		static void Test()
		{
			TestVector();
		}

	private:
		static void TestVector()
		{
			Vector3F v1(1, 2, 3);
			Vector3F v2(3, 2, 1);

			float dot = v1.Dot(v2);
			Vector3F cross;
			cross.CrossProduct(v1, v2);
		}
	};
}