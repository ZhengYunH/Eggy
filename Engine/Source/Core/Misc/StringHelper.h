#pragma once
#include "Core/Config.h"
#if defined(_WIN32)
#include <Windows.h>
#endif

namespace Eggy
{
	class StringHelper
	{
	public:
		static WString String2WString(const String& str)
		{
			if (str.empty()) return WString();
#if defined(_WIN32)
			int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
			WString wstrTo(size_needed, 0);
			MultiByteToWideChar(CP_UTF8, 0, &str[0], -1, &wstrTo[0], size_needed);
			return wstrTo;
#endif
		}

		static String WString2String(const WString& wstr)
		{
			if (wstr.empty()) return String();
#if defined(_WIN32)
			int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
			String strTo(size_needed, 0);
			WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
			return strTo;
#endif
		}

		static void Replace(String& s, char src, char dst)
		{
			std::replace(s.begin(), s.end(), src, dst);
		}
	};
}
