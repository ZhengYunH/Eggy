#pragma once
#include <source_location>
#include <string>


namespace Eggy
{
	enum class ELogLevel : char
	{
		Info,
		Warning,
		Error,
		Fatal
	};

	class ILogger
	{
	public:
		virtual ILogger& Log(std::source_location srcLoc, ELogLevel level) = 0;
		virtual ILogger& operator << (const char* info) = 0;
		virtual ILogger& operator << (const std::string& info) = 0;
	};
	extern ILogger* GLogger;
}

