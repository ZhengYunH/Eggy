#pragma once
#include <concepts>

// TODO: impl all concepts in https://www.enseignement.polytechnique.fr/informatique/INF478/docs/Cpp/en/cpp/concept.html
namespace Eggy
{
	template<typename T>
	concept cDefaultConstructible = std::is_default_constructible<T>::value;

	template<typename _Base, typename _Derived>
	concept cDerivedFrom = std::derived_from<_Derived, _Base>;
}