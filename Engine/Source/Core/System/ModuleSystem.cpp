#include "ModuleSystem.h"


namespace Eggy
{
	DefineSystem(ModuleSystem);

	void ModuleSystem::Initialize()
	{
		mModuleParentPaths_[EModuleType::Default] = "./Engine/";
		mModuleParentPaths_[EModuleType::Plugins] = "./Engine/Plugins";
		mModuleParentPaths_[EModuleType::External] = "./Engine/External";
	}

	bool ModuleSystem::LoadModule(const String& dllName, EModuleType type)
	{
		if (mModules_.find(dllName) != mModules_.end())
			return true;

		String realName = mModuleParentPaths_[type] + dllName + ".dll";
#if defined(_WIN32)
		ModuleHandle module = ::LoadLibraryA(realName.c_str());
		if (!module)
		{
			LOG(Core, Fatal) << "Load Module <" << dllName << "> Fail";
			return false;
		}
		mModules_[dllName] = module;
#endif
		return true;
	}

	void ModuleSystem::UnloadModule(const String& dllName)
	{
		if (ModuleHandle module = GetModule(dllName))
		{
#if defined(_WIN32)
			::FreeLibrary(module);
			mModules_.erase(dllName);
#endif
		}
	}

	ModuleHandle ModuleSystem::GetModule(const String& dllName)
	{
		auto itr = mModules_.find(dllName);
		if (itr == mModules_.end())
			return NULL;
		return itr->second;
	}
}

