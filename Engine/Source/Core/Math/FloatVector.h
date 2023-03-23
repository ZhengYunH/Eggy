#pragma once
#include "Core/Foundation.h"
#include "MathUtil.h"
#include "boost/functional/hash.hpp"


namespace Eggy
{
	template<typename _ScalarType, template<typename> class _VectorType, int _Dimension, typename _Traits>
	class TFloatVector : public _Traits
	{
	public:
		using RealType = _VectorType<_ScalarType>;
		static constexpr int DIMENSION = _Dimension;

		constexpr FORCEINLINE _ScalarType* GetPointer() noexcept
		{
			return reinterpret_cast<_ScalarType*>(this);
		}

		constexpr FORCEINLINE const _ScalarType* GetPointer() const noexcept
		{
			return reinterpret_cast<const _ScalarType*>(this);
		}

		constexpr FORCEINLINE _ScalarType GetLengthSquared() const noexcept
		{
			_ScalarType ret = (_ScalarType)0;
			const _ScalarType* pSrc = GetPointer();

			for (int i = 0; i < DIMENSION; ++i)
				ret += pSrc[i] * pSrc[i];

			return ret;
		}

		constexpr FORCEINLINE _ScalarType GetLength() const noexcept
		{
			return Sqrt(GetLengthSquared());
		}

		constexpr FORCEINLINE _ScalarType GetSum() const noexcept
		{
			_ScalarType ret = (_ScalarType)0;
			const _ScalarType *pSrc = GetPointer();

			for (int i = 0; i < DIMENSION; ++i)
				ret += pSrc[i];

			return ret;
		}

		template<typename _Other> constexpr _ScalarType FORCEINLINE Dot(const _VectorType<_Other>& rhs)
		{
			const _ScalarType* left = GetPointer();
			const _ScalarType* right = rhs.GetPointer();
			_ScalarType	dot = (_ScalarType)0;
			for (int i = 0; i < DIMENSION; ++i)
			{
				dot += left[i] * _ScalarType(right[i]);
			}
			return dot;
		}

		constexpr FORCEINLINE RealType GetNormalized() const noexcept
		{
			RealType ret;
			const _ScalarType* pSrc = GetPointer();
			_ScalarType* pDst = ret.GetPointer();
			for (int i = 0; i < DIMENSION; ++i)
			{
				pDst[i] = pSrc[i];
			}
			ret.Normalize();
			return ret;
		}

		constexpr FORCEINLINE void Normalize() noexcept
		{
			_ScalarType length = GetLength();
			if(length > EPISILON)
			{
				_ScalarType* pSrc = GetPointer();
				for (int i = 0; i < DIMENSION; ++i)
				{
					pSrc[i] /= length;
				}
			}
		}

		constexpr FORCEINLINE bool operator ==(const RealType& rhs) const noexcept
		{
			const _ScalarType* left = GetPointer();
			const _ScalarType* right = rhs.GetPointer();
			bool				ret = true;
			for (int i = 0; i < DIMENSION; ++i)
			{
				ret &= left[i] == right[i];
			}
			return ret;
		}

		constexpr FORCEINLINE bool operator !=(const RealType& rhs) const noexcept
		{
			const _ScalarType* left = GetPointer();
			const _ScalarType* right = rhs.GetPointer();
			bool				ret = false;
			for (int i = 0; i < DIMENSION; ++i)
			{
				ret |= left[i] != right[i];
			}
			return ret;
		}

		constexpr FORCEINLINE RealType operator +(const RealType& rhs) const noexcept
		{
			const _ScalarType* left = GetPointer();
			const _ScalarType* right = rhs.GetPointer();
			RealType	ret;
			_ScalarType* dst = ret.GetPointer();
			for (int i = 0; i < DIMENSION; ++i)
			{
				dst[i] = left[i] + right[i];
			}
			return ret;
		}

		constexpr FORCEINLINE RealType operator +(const _ScalarType& rhs) const noexcept
		{
			const _ScalarType* left = GetPointer();
			RealType	ret;
			_ScalarType* dst = ret.GetPointer();
			for (int i = 0; i < DIMENSION; ++i)
			{
				dst[i] = left[i] + rhs;
			}
			return ret;
		}

		constexpr FORCEINLINE RealType operator -(const RealType& rhs) const noexcept
		{
			const _ScalarType* left = GetPointer();
			const _ScalarType* right = rhs.GetPointer();
			RealType	ret;
			_ScalarType* dst = ret.GetPointer();
			for (int i = 0; i < DIMENSION; ++i)
			{
				dst[i] = left[i] - right[i];
			}
			return ret;
		}

		constexpr FORCEINLINE RealType operator -(const _ScalarType& rhs) const noexcept
		{
			const _ScalarType* left = GetPointer();
			RealType	ret;
			_ScalarType* dst = ret.GetPointer();
			for (int i = 0; i < DIMENSION; ++i)
			{
				dst[i] = left[i] - rhs;
			}
			return ret;
		}

		constexpr FORCEINLINE RealType operator -() const noexcept
		{
			const _ScalarType* pSrc = GetPointer();
			RealType	ret;
			_ScalarType* dst = ret.GetPointer();
			for (int i = 0; i < DIMENSION; ++i)
			{
				dst[i] = _ScalarType(-1) * pSrc[i];
			}
			return ret;
		}

		constexpr FORCEINLINE RealType operator *(const RealType& rhs) const noexcept
		{
			const _ScalarType* left = GetPointer();
			const _ScalarType* right = rhs.GetPointer();
			RealType	ret;
			_ScalarType* dst = ret.GetPointer();
			for (int i = 0; i < DIMENSION; ++i)
			{
				dst[i] = left[i] * right[i];
			}
			return ret;
		}

		constexpr FORCEINLINE RealType operator *(const _ScalarType& rhs) const noexcept
		{
			const _ScalarType* left = GetPointer();
			RealType	ret;
			_ScalarType* dst = ret.GetPointer();
			for (int i = 0; i < DIMENSION; ++i)
			{
				dst[i] = left[i] * rhs;
			}
			return ret;
		}

		constexpr FORCEINLINE RealType& operator *=(const _ScalarType& rhs) noexcept
		{
			_ScalarType* pSrc = GetPointer();
			for (int i = 0; i < DIMENSION; ++i)
			{
				pSrc[i] = pSrc[i] * rhs;
			}
			return *static_cast<RealType*>(this);
		}


		constexpr FORCEINLINE RealType operator /(const RealType& rhs) const noexcept
		{
			const _ScalarType* left = GetPointer();
			const _ScalarType* right = rhs.GetPointer();
			RealType	ret;
			_ScalarType* dst = ret.GetPointer();
			for (int i = 0; i < DIMENSION; ++i)
			{
				dst[i] = left[i] / Max(right[i], std::numeric_limits<_ScalarType>::epsilon());
			}
			return ret;
		}

		constexpr FORCEINLINE RealType operator /(const _ScalarType& rhs) const noexcept
		{
			const _ScalarType* left = GetPointer();
			RealType	ret;
			_ScalarType* dst = ret.GetPointer();
			for (int i = 0; i < DIMENSION; ++i)
			{
				dst[i] = left[i] / Max(rhs, std::numeric_limits<_ScalarType>::epsilon());
			}
			return ret;
		}

		constexpr FORCEINLINE RealType& operator /=(const _ScalarType& rhs) noexcept
		{
			_ScalarType* pSrc = GetPointer();
			for (int i = 0; i < DIMENSION; ++i)
			{
				pSrc[i] = pSrc[i] / Max(rhs, std::numeric_limits<_ScalarType>::epsilon());
			}
			return *static_cast<RealType*>(this);
		}

		constexpr FORCEINLINE _ScalarType operator [](int index) const noexcept
		{
			HYBRID_CHECK(index < DIMENSION);
			return GetPointer()[index];
		}

		constexpr FORCEINLINE std::size_t Hash() const noexcept
		{
			const _ScalarType* pSrc = GetPointer();
			std::size_t hash = boost::hash_value(pSrc[0]);

			for (int i = 1; i < DIMENSION; ++i)
				boost::hash_combine(hash, pSrc[i]);

			return hash;
		}
	};
}