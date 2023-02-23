#pragma once
#include <map>
#include <unordered_map>
#include <set>

namespace Eggy
{
	struct EnumClassHash
	{
		template<typename T>
		std::size_t operator()(T t)const
		{
			return static_cast<std::size_t>(t);
		}
	};

	template<typename Key>
	using MapKeyHashType = typename std::conditional<std::is_enum_v<Key>, EnumClassHash, std::hash<Key>>::type;

	template<typename _TKey, typename _TValue>
	using Map = std::unordered_map<_TKey, _TValue, MapKeyHashType<_TKey>>;
}