#pragma once
#include <concepts>

// TODO: impl all concepts in https://www.enseignement.polytechnique.fr/informatique/INF478/docs/Cpp/en/cpp/concept.html
namespace Eggy
{
	template<typename _T>
	concept cDefaultConstructible = std::is_default_constructible<_T>::value;

	template<typename _Base, typename _Derived>
	concept cDerivedFrom = std::derived_from<_Derived, _Base>;

	template<typename _U, typename _V>
	concept cConvertable = std::is_convertible_v<_U, _V>;

	// Customize
	template<typename _T>
	concept cEnableShared = requires(_T * a)
	{
		{a->Acquire()} -> std::same_as<void>;
		{a->Release()} -> std::same_as<void>;
		{a->GetRefCount()} -> std::same_as<int>;
	};

	template<typename _T>
	concept cHashable = requires(const _T& t)
	{
		{ std::hash<_T>()(t) } -> std::same_as<size_t>;
	};
}