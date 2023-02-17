#pragma once
#include "Core/Config.h"
#include "Core/Interface/ISystem.h"

namespace Eggy
{
	enum EModuleType : uint8
	{
		Default = 0,
		Plugins = 1,
		External = 2
	};

	class ModuleSystem : public TSystem<ModuleSystem>
	{
		DeclareSystem(ModuleSystem);

		
	public:
		bool LoadModule(const String& dllName, EModuleType type=EModuleType::Default);
		void UnloadModule(const String& dllName);

#if defined(_WIN32)
		HMODULE GetModule(const String& dllName);
#endif
		void Initialize() override;

		void Finalize() override
		{
		}

	protected:
#if defined(_WIN32)
		Map<String, HMODULE> mModules_;
#endif
		Map<EModuleType, String> mModuleParentPaths_;
	};
}