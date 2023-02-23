#pragma once
#include <concepts>

namespace Eggy
{
	template<typename T>
	concept cDefaultConstructible = std::is_default_constructible<T>::value;

	template<typename _Base, typename _Derived>
	concept cDerivedFrom = std::derived_from<_Derived, _Base>;
}