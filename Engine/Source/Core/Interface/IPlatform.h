#pragma once
#include "Core/Config.h"
#include "Core/Platform/Win32.h"

namespace Eggy
{
	class IPlatform 
	{
	public:
		virtual WINDOW_HANDLE GetWindow() = 0;
		virtual bool PumpMessage() = 0;
	};
}