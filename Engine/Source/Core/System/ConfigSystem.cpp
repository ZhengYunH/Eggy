#include "ConfigSystem.h"

namespace Eggy
{
	DefineSystem(ConfigSystem);

	Configuration* ConfigSystem::InsertConfiguration(const Name& config, bool bReplaceMode)
	{
		auto pair = mConfigs_.find(config);
		if (pair != mConfigs_.end())
		{
			if (!bReplaceMode)
				return pair->second;
			else
				pair->second->Clear();
		}
		else
		{
			mConfigs_[config] = new Configuration();
		}
		return mConfigs_[config];
	}

	void ConfigSystem::Initialize()
	{
		InsertConfiguration(GLOBAL_CONFIG.data());
		SetGlobalConfig("AppWidth", 800);
		SetGlobalConfig("AppHeight", 600);
		SetGlobalConfig("IsFullScreen", false);
	}

	void ConfigSystem::Finalize()
	{
	}

}