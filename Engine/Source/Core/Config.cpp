#include "Config.h"

namespace Eggy
{
	const uint32 SCREEN_INIT_WIDTH = 800;
	const uint32 SCREEN_INIT_HEIGHT = 600;
	const uint32 SCREEN_REFRESH_RATE = 60;
};

namespace Eggy::Tool
{
	LPCWSTR stringToLPCWSTR(const std::string& orig)
	{
		size_t origsize = orig.length() + 1;
		const size_t newsize = 100;
		size_t convertedChars = 0;
		wchar_t* wcstring = (wchar_t*)malloc(sizeof(wchar_t) * (orig.length() - 1));
		mbstowcs_s(&convertedChars, wcstring, origsize, orig.c_str(), _TRUNCATE);

		return wcstring;
	}
}
