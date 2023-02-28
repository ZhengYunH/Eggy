#include "LoggerSystem.h"
#include "SystemManager.h"
#include "FileSystem.h"
#include "Core/Misc/StringHelper.h"
#include <chrono>
#include <iomanip>
#include <time.h>


namespace Eggy
{
	DefineSystem(LoggerSystem);
	DefineDependency(LoggerSystem, FileSystem);

	ILogger* GLogger = nullptr;
	static String getCurrentTimeStr()
	{
		auto now = std::chrono::system_clock::now();
		auto time = std::chrono::system_clock::to_time_t(now);

		std::stringstream ss;
		tm t;
		localtime_s(&t, &time);
		ss << std::put_time(&t, "%Y-%m-%d_%X");
		String str = ss.str();
		StringHelper::Replace(str, ':', '_');
		return str;
	}

	void LoggerSystem::Initialize()
	{
		GLogger = this;
		FPath path = FileSystem::Get()->GetLogRoot() + getCurrentTimeStr();
		path.replaceFilePostfix(".txt");
		mFilePath_ = path.ToString();
	}

	void LoggerSystem::Finalize()
	{
		Log(std::source_location::current(), ELogLevel::Info);
		FileHandle handle = FileSystem::Get()->CreateFile(mFilePath_);
		File* file = dynamic_cast<File*>(handle.get());
		if (file)
		{
			for (auto& s : mHistories_)
			{
				size_t len = s.length();
				char* str = new char[len + 1];
				memcpy(str, s.data(), len);
				str[len] = '\n';
				file->Save(str, len + 1);
				delete[] str;
			}
		}
	}

	ILogger& LoggerSystem::Log(std::source_location srcLoc, ELogLevel level)
	{
		if (!mLastInfo_.empty())
			mHistories_.push_back(std::format("[{0}]{1}", getCurrentTimeStr(), mLastInfo_));
		mLastInfo_.clear();
		return *this;
	}

	ILogger& LoggerSystem::operator<<(const char* info)
	{
		mLastInfo_ += info;
		return *this;
	}

	ILogger& LoggerSystem::operator<<(const std::string& info)
	{
		return operator<<(info.c_str());
	}
}

