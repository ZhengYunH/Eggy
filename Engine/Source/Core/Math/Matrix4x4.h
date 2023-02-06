#pragma once
#include "MathUtil.h"
#include "Matrix4x3.h"


namespace Eggy
{
	class Matrix4x4
	{
	public:
		float m00, m01, m02, m03;
		float m10, m11, m12, m13;
		float m20, m21, m22, m23;
		float m30, m31, m32, m33;

		inline Matrix4x4() noexcept = default;
		inline Matrix4x4(const Matrix4x4&) noexcept = default;
		inline Matrix4x4& operator=(const Matrix4x4&) noexcept = default;

		Matrix4x4(const Matrix4x3& mat) noexcept
			: m00{ mat.m00 }, m01{ mat.m01 }, m02{ mat.m02 }, m03{ 0 }
			, m10{ mat.m10 }, m11{ mat.m11 }, m12{ mat.m12 }, m13{ 0 }
			, m20{ mat.m20 }, m21{ mat.m21 }, m22{ mat.m22 }, m23{ 0 }
			, m30{ mat.m30 }, m31{ mat.m31 }, m32{ mat.m32 }, m33{ 1 }
		{

		}

		Matrix4x4(
			float in00, float in01, float in02, float in03,
			float in10, float in11, float in12, float in13,
			float in20, float in21, float in22, float in23,
			float in30, float in31, float in32, float in33
		) noexcept
			: m00{ in00 }, m01{ in01 }, m02{ in02 }, m03{ in03 }
			, m10{ in10 }, m11{ in11 }, m12{ in12 }, m13{ in13 }
			, m20{ in20 }, m21{ in21 }, m22{ in22 }, m23{ in23 }
			, m30{ in30 }, m31{ in31 }, m32{ in32 }, m33{ in33 }
		{
		}

		void SetIdentity()
		{
			m00 = 1; m01 = 0; m02 = 0; m03 = 0;
			m10 = 0; m11 = 1; m12 = 0; m13 = 0;
			m20 = 0; m21 = 0; m22 = 1; m23 = 0;
			m30 = 0; m31 = 0; m32 = 0; m33 = 1;
		}

		void Transpose()
		{
			std::swap(m01, m10);
			std::swap(m02, m20);
			std::swap(m03, m30);
			std::swap(m12, m21);
			std::swap(m13, m31);
			std::swap(m23, m32);
		}


		inline void SetRotationX(const float rad, const Vector3& t) noexcept
		{
			Matrix3x3 m;
			m.SetRotationX(rad);
			*this = Matrix4x3(m);
			SetTranslation(t);
		}
		inline void SetRotationY(const float rad, const Vector3& t) noexcept
		{
			Matrix3x3 m;
			m.SetRotationY(rad);
			*this = Matrix4x3(m);
			SetTranslation(t);
		}

		inline void SetTranslation(const Vector3& translation)
		{
			m30 = translation.x;
			m31 = translation.y;
			m32 = translation.z;
		}
	};
}