#pragma once
#include "FloatVector.h"

namespace Eggy
{
	template<typename _Type, template<typename> class _VectorType, int _Dimension, typename _Traits> 
	struct SelectVectorBase;

	template<template<typename> class _VectorType, int _Dimension, typename _Traits> 
	struct SelectVectorBase<float, _VectorType, _Dimension, _Traits>
	{
		using type = TFloatVector<float, _VectorType, _Dimension, _Traits>;
	};

	template<template<typename> class _VectorType, int _Dimension, typename _Traits>
	struct SelectVectorBase<double, _VectorType, _Dimension, _Traits>
	{
		using type = TFloatVector<double, _VectorType, _Dimension, _Traits>;
	};
}