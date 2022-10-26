#include "Core/Math/Vector2.h"
#include "Core/Math/Vector3.h"
#include "Core/Math/Vector4.h"
#include "Core/Math/Matrix4x3.h"

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
			Vector3F v31(1, 2, 3);
			Vector3F v32(3, 2, 1);

			float dot = v31.Dot(v32);
			Vector3F cross;
			cross.CrossProduct(v31, v32);

			Vector2 v21(1, 2);
			Vector2 v22(3, 4);
			Vector2 v = v21 + v22;

			Vector4 v41(1, 2, 3, 4);
			Vector4 v42(-1, -2, -3, -4);
			Vector4 v43 = v41 + v42;

			Matrix4x3 m43 = Matrix4x3_Identity;
			Vector4 v44 = m43.TransformVector(v41);
		}
	};
}