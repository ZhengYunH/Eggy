#pragma once
#include "VectorBase.h"


namespace Eggy
{
	template<typename _ScalarType, typename _FinalType>
	struct TFloatingPointVector2Base
	{

	};

	template<typename _ScalarType, template<typename> class _VectorType>
	struct TFloatingPointVector2
	{
		struct EmptyStruct {};
		using type = EmptyStruct;
	};
	template<template<typename> class _VectorType>
	struct TFloatingPointVector2<float, _VectorType>
	{
		using type = TFloatingPointVector2Base<float, _VectorType<float>>;
	};
	template<template<typename> class _VectorType>
	struct TFloatingPointVector2<double, _VectorType>
	{
		using type = TFloatingPointVector2Base<double, _VectorType<double>>;
	};

	template<typename _ScalarType>
	class TVec2 : public SelectVectorBase<_ScalarType, TVec2, 2, typename TFloatingPointVector2<_ScalarType, TVec2>::type>::type
	{
	public:
		_ScalarType x, y;

		using BaseType = typename SelectVectorBase<_ScalarType, TVec2, 2, typename TFloatingPointVector2<_ScalarType, TVec2>::type>::type;
		using BaseType::operator==;
		using BaseType::operator!=;

	public:
		constexpr FORCEINLINE TVec2()
			: x{ 0 }
			, y{ 0 }

		{
		}

		constexpr FORCEINLINE TVec2(_ScalarType _x, _ScalarType _y) noexcept
			: x{ _x }
			, y{ _y }
		{
		}
	};

	using Vector2 = TVec2<float>;
	using Vector2F = TVec2<float>;
	using Vector2D = TVec2<double>;
}