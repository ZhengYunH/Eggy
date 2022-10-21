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


namespace Eggy
{
	using String = std::string;
	using Name = std::string;

	template<typename _TKey, typename _TValue>
	using Map = std::unordered_map<_TKey, _TValue>;
	
	template<typename _Type>
	using List = std::vector<_Type>;

	using uint16 = uint16_t;
	using uint32 = uint32_t;

	template<typename _TType>
	using SharedPtr = std::shared_ptr<_TType>;

	/*template<typename _TType>
	using SharedFrom = std::make_shared<_TType>;*/
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