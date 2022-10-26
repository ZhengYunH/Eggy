#pragma once
#include "Core/Config.h"

#include <cmath>
#include <limits>
#include <tuple>


namespace Eggy
{
	static constexpr double MATH_PI = (3.141592653589793238462643383279502884197169399375105820974944592307816406286);
	static constexpr double MATH_HALF_PI = (MATH_PI / 2.0);
	static constexpr double MATH_DOUBLE_PI = (2.0 * MATH_PI);

	static constexpr double MATH_INV_PI = (1.0 / MATH_PI);
	static constexpr double MATH_INV_DOUBLE_PI = (1.0 / MATH_DOUBLE_PI);

	static constexpr double MATH_PI_OVER_180 = (MATH_PI / 180.0);
	static constexpr double MATH_PI_UNDER_180 = (180.0 / MATH_PI);

	static constexpr float EPISILON = std::numeric_limits<float>::epsilon();
	static constexpr float MIN_FLOAT = std::numeric_limits<float>::min();
	static constexpr float MAX_FLOAT = std::numeric_limits<float>::max();
	static constexpr uint32 MAX_UINT = std::numeric_limits<uint32>::max();

	static constexpr uint32 FLOATU32SignMask = (1UL << 31);
	static constexpr inline uint32 FLOATU32(float x) noexcept
	{
		union
		{
			float  f;
			uint32 i = 0;
		} s;
		s.f = x;
		return s.i;
	}
	static constexpr inline float FLOATFROMU32(uint32 x) noexcept
	{
		union
		{
			float  f;
			uint32 i = 0;
		} s;
		s.i = x;
		return s.f;
	}


	template<typename _LHS, typename _RHS>
	static constexpr inline auto Max(_LHS a, _RHS b) noexcept { return (a > b) ? (a) : (b); }

	template<typename _LHS, typename _RHS, typename... _Others>
	static constexpr inline auto Max(_LHS a, _RHS b, _Others... others) noexcept { return Max(a, Max(b, others...)); }

	template<typename _LHS, typename _RHS>
	static constexpr inline auto Min(_LHS a, _RHS b) noexcept { return (a < b) ? (a) : (b); }

	template<typename _LHS, typename _RHS, typename... _Others>
	static constexpr inline auto Min(_LHS a, _RHS b, _Others... others) noexcept { return Min(a, Min(b, others...)); }

	template<typename _Scaler>
	static constexpr inline _Scaler Sqrt(_Scaler a) noexcept { return std::sqrt(a); }

	template<typename _Type> 
	static constexpr FORCEINLINE _Type Clamp(_Type value, _Type minimum, _Type maximum) noexcept
	{
		if (value < minimum)
			return minimum;
		if (value > maximum)
			return maximum;
		return value;
	}

	template<typename _Type> 
	static constexpr FORCEINLINE _Type MapUnitToRange(_Type value, _Type range) noexcept
	{
		return Clamp(value, _Type(0), _Type(1)) * range;
	}

	template<typename _type>
	static constexpr inline std::enable_if_t<std::is_floating_point_v<_type>, _type> Round(const _type& a) noexcept
	{
		auto i = static_cast<int>(a);
		if ((a - _type(i)) < _type(0.5))
			return _type(i);
		return _type(i + 1);
	}
	template<typename _type>
	static constexpr inline std::enable_if_t<std::is_floating_point_v<_type>, int> Ceil(const _type& a) noexcept
	{
		int i = static_cast<int>(a);
		if (a > i)
			return i + 1;
		return i;
	}

	template<typename _type, typename _scalar>
	static constexpr inline _type Lerp(const _type& a, const _type& b, const _scalar& x) noexcept
	{
		return (a * (1 - x)) + (b * x);
	}

	template<typename _type, typename _scalar>
	static constexpr inline _type CubicInterp(const _type& a, const _type& ta, const _type& b, const _type& tb, const _scalar& x) noexcept
	{
		const _scalar x2 = x * x;
		const _scalar x3 = x * x2;
		_scalar		  tmp = 2 * x3 - 3 * x2;
		return (tmp + 1) * a + (x3 - 2 * x2 + x) * ta + (-tmp) * b + (x3 - x2) * tb;
	}

	template<typename _type>
	static inline constexpr std::enable_if_t<std::is_floating_point_v<_type>, bool> IsZero(_type v)
	{
		return v < EPISILON&& v > -EPISILON;
	}

	static constexpr inline float Fabs(float x) noexcept {
		unsigned int i = FLOATU32(x) & (~FLOATU32SignMask);
		return FLOATFROMU32(i);
	}

	static inline float Fmod(float X, float Y)
	{
		if (Fabs(Y) <= EPISILON)
		{
			//error
			return 0.f;
		}
		const float Quotient = static_cast<float>(static_cast<int>(X / Y));
		float		IntPortion = Y * Quotient;

		// Rounding and imprecision could cause IntPortion to exceed X and cause the result to be outside the expected range.
		// For example Fmod(55.8, 9.3) would result in a very small negative value!
		if (Fabs(IntPortion) > Fabs(X))
		{
			IntPortion = X;
		}

		const float Result = X - IntPortion;
		return Result;
	}

	template<typename _Scaler>
	static constexpr inline _Scaler Sin(_Scaler radian) noexcept { return std::sin(radian); }

	template<typename _Scaler>
	static constexpr inline _Scaler Cos(_Scaler radian) noexcept { return std::cos(radian); }

	template<typename _Scaler>
	static inline std::tuple<_Scaler, _Scaler> SinCos(_Scaler radians) noexcept {
		return std::make_tuple(Sin(radians), Cos(radians));
	}

	template<typename _LHS, typename _RHS>
	static constexpr inline auto ATan2(_LHS a, _RHS b) noexcept { return std::atan2(a, b); }

	template<typename _Scaler>
	static constexpr inline _Scaler ACos(_Scaler radian) noexcept { return std::acos(radian); }

	template<typename _Scaler>
	static constexpr inline _Scaler SafeACos(_Scaler radian) noexcept
	{
		if (radian <= (_Scaler)-1)
			return MATH_PI;
		if (radian >= (_Scaler)1)
			return (_Scaler)0;
		return ACos(radian);
	}

	template<typename _Scaler>
	static constexpr inline _Scaler WrapPI(_Scaler radian) noexcept  // clamp theta to (-pi, pi]
	{
		radian += (_Scaler)MATH_PI;
		radian -= std::floor(radian * MATH_INV_DOUBLE_PI) * MATH_DOUBLE_PI;
		radian -= (_Scaler)MATH_PI;
		return radian;
	}

	// Degree-Radian Convert
	template<typename _type>
	static constexpr inline std::enable_if_t<std::is_floating_point_v<_type>, _type> DegreeToRadian(const _type& angle) noexcept
	{
		return angle * MATH_PI_OVER_180;
	}

	template<typename _type>
	static constexpr inline std::enable_if_t<std::is_floating_point_v<_type>, _type> RadianToDegree(const _type& radian) noexcept
	{
		return radian * MATH_PI_UNDER_180;
	}
}