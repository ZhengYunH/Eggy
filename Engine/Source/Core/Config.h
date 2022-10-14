#define D3D11_DEVICE 1
#define VULKAN_DEVICE 0
#define DEBUG_MODE 1

#if DEBUG_MODE
	#include <assert.h>
	#define HYBRID_CHECK(X, ...) assert((X))
	#define DEBUG_RUN(X) X
#else
	#define HYBRID_CHECK(X, ...) 
	#define DEBUG_RUN(X)
#endif


#define Unimplement(...) HYBRID_CHECK(0, ##__VA_ARGS__)
#define UNEXPECTED(X) (X)
#define SafeDestroy(X) if(X){ delete X; X=nullptr;}
#define SafeDestroyArray(X) if(X){delete[] X; X=nullptr;}


// platform
#if defined(_WIN32)
#include "System/Win32.h"
#endif

#include <string>
#include <map>
#include <vector>
#include <map>
#include <functional>

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
}