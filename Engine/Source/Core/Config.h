#pragma once
#include "Foundation.h"

#if D3D11_DEVICE
	#define HR(X, ...)	HYBRID_RUN(SUCCEEDED(X),  ##__VA_ARGS__)
#else

#endif


// platform
#if defined(_WIN32)
#include "Platform/Win32.h"
#endif

#include <string>
#include <map>
#include <vector>
#include <map>
#include <functional>
#include <memory>
#include <stdexcept>
#include <set>
#include <sstream>


namespace Eggy
{
	using String = std::string;
	using Name = std::string;


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

	template<typename _Type>
	using List = std::vector<_Type>;

	using uint8 = uint8_t;
	using uint16 = uint16_t;
	using uint32 = uint32_t;
}

namespace Eggy
{
	extern const uint32 SCREEN_INIT_WIDTH;
	extern const uint32 SCREEN_INIT_HEIGHT;
	extern const uint32 SCREEN_REFRESH_RATE;
}

namespace Eggy
{
	namespace Tool
	{
		LPCWSTR stringToLPCWSTR(const std::string& orig);
	}
}