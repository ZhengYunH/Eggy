#pragma once
#include "VectorBase.h"


namespace Eggy
{
	template<typename _ScalarType, typename _FinalType>
	struct TFloatingPointVector4Base
	{

	};

	template<typename _ScalarType, template<typename> class _VectorType>
	struct TFloatingPointVector4
	{
		struct EmptyStruct {};
		using type = EmptyStruct;
	};
	template<template<typename> class _VectorType>
	struct TFloatingPointVector4<float, _VectorType>
	{
		using type = TFloatingPointVector4Base<float, _VectorType<float>>;
	};
	template<template<typename> class _VectorType>
	struct TFloatingPointVector4<double, _VectorType>
	{
		using type = TFloatingPointVector4Base<double, _VectorType<double>>;
	};

	template<typename _ScalarType>
	class TVec4 : public SelectVectorBase<_ScalarType, TVec4, 4, typename TFloatingPointVector4<_ScalarType, TVec4>::type>::type
	{
	public:
		_ScalarType x, y, z, w;

		using BaseType = typename SelectVectorBase<_ScalarType, TVec4, 4, typename TFloatingPointVector4<_ScalarType, TVec4>::type>::type;
		using BaseType::operator==;
		using BaseType::operator!=;

	public:
		constexpr FORCEINLINE TVec4()
			: x{ 0 }
			, y{ 0 }
			, z{ 0 }
			, w{ 0 }
		{
		}

		constexpr FORCEINLINE TVec4(_ScalarType _x, _ScalarType _y, _ScalarType _z, _ScalarType _w) noexcept
			: x{ _x }
			, y{ _y }
			, z{ _z }
			, w{ _w }
		{
		}
	};

	using Vector4 = TVec4<float>;
	using Vector4F = TVec4<float>;
	using Vector4D = TVec4<double>;
}
