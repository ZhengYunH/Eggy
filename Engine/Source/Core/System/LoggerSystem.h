#pragma once
#include "Core/Interface/ILogger.h"
#include "Core/Interface/ISystem.h"
#include "Core/DataType/Container/List.h"


namespace Eggy
{
	class LoggerSystem : public TSystem<LoggerSystem>, public ILogger
	{
		DeclareSystem(LoggerSystem);
	public:
		void Initialize() override;
		void Finalize() override;

		ILogger& Log(std::source_location srcLoc, ELogLevel level) override;
		ILogger& operator <<(const char* info) override;
		ILogger& operator <<(const std::string& info) override;

	protected:
		String mFilePath_;
		List<String> mHistories_;
		String mLastInfo_;
	};
	DeclareDependency(LoggerSystem);
}

