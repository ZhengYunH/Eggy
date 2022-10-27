#pragma once
#include "VectorBase.h"


namespace Eggy
{
	template<typename _ScalarType, typename _FinalType>
	struct TFloatingPointVector3Base
	{

	};

	template<typename _ScalarType, template<typename> class _VectorType>
	struct TFloatingPointVector3
	{
		struct EmptyStruct {};
		using type = EmptyStruct;
	};
	template<template<typename> class _VectorType> 
	struct TFloatingPointVector3<float, _VectorType>
	{
		using type = TFloatingPointVector3Base<float, _VectorType<float>>;
	};
	template<template<typename> class _VectorType>
	struct TFloatingPointVector3<double, _VectorType>
	{
		using type = TFloatingPointVector3Base<double, _VectorType<double>>;
	};

	template<typename _ScalarType>
	class TVec3 : public SelectVectorBase<_ScalarType, TVec3, 3, typename TFloatingPointVector3<_ScalarType, TVec3>::type>::type
	{
	public:
		_ScalarType x, y, z;
		
		using BaseType = typename SelectVectorBase<_ScalarType, TVec3, 3, typename TFloatingPointVector3<_ScalarType, TVec3>::type>::type;
		using BaseType::operator==;
		using BaseType::operator!=;

	public:
		constexpr FORCEINLINE TVec3()
			: x{ 0 }
			, y{ 0 }
			, z{ 0 }
		{
		}

		constexpr FORCEINLINE TVec3(_ScalarType _x, _ScalarType _y, _ScalarType _z) noexcept
			: x{ _x }
			, y{ _y }
			, z{ _z }
		{
		}

		template<typename _LHS, typename _RHS> void CrossProduct(const TVec3<_LHS>& a, const TVec3<_RHS>& b) noexcept
		{
			_ScalarType _x = (_ScalarType(a.y) * _ScalarType(b.z) - _ScalarType(a.z) * _ScalarType(b.y));
			_ScalarType _y = (_ScalarType(a.z) * _ScalarType(b.x) - _ScalarType(a.x) * _ScalarType(b.z));
			_ScalarType _z = (_ScalarType(a.x) * _ScalarType(b.y) - _ScalarType(a.y) * _ScalarType(b.x));
			this->x = _x;
			this->y = _y;
			this->z = _z;
		}

		template<typename _RHS>TVec3<_ScalarType> operator ^(const TVec3<_RHS>& rhs)
		{
			CrossProduct(*this, rhs);
			return *this;
		}
	};

	using Vector3 = TVec3<float>;
	using Vector3F = TVec3<float>;
	using Vector3D = TVec3<double>;
}