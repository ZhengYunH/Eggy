#pragma once
#include <vector>
#include <list>
#include <queue>


namespace Eggy
{
	template<typename _Type>
	using List = std::vector<_Type>;

	template<typename _Type>
	using LinkList = std::list<_Type>;

	template<typename _Type>
	using Queue = std::queue<_Type>;
}