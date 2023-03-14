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

#include <functional>
#include <memory>
#include <stdexcept>
#include <sstream>
#include <chrono>

#include "Core/DataType/Container/String.h"
#include "Core/DataType/Container/List.h"
#include "Core/DataType/Container/Map.h"


namespace Eggy
{
	using uint8 = uint8_t;
	using uint16 = uint16_t;
	using uint32 = uint32_t;
}


namespace Eggy
{
	namespace Tool
	{
		LPCWSTR stringToLPCWSTR(const std::string& orig);
	}
}

// CopyData Signal
namespace Eggy::CopyDataSignal
{
	const uint32 ShaderChange = 1;
}