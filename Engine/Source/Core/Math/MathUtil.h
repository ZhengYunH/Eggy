#pragma once
#include<cmath>


namespace Eggy
{
	static constexpr double MATH_PI = (3.141592653589793238462643383279502884197169399375105820974944592307816406286);
	static constexpr double MATH_PI_OVER_180 = (MATH_PI / 180.0);

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
}