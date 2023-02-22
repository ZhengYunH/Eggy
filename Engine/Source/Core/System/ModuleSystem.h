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
#if defined(_WIN32)
		typedef HMODULE ModuleHandle;
#endif	
	public:
		bool LoadModule(const String& dllName, EModuleType type=EModuleType::Default);
		void UnloadModule(const String& dllName);

		ModuleHandle GetModule(const String& dllName);

		void Initialize() override;

		void Finalize() override
		{
		}

	protected:
		Map<String, ModuleHandle> mModules_;
		Map<EModuleType, String> mModuleParentPaths_;
	};
	using ModuleHandle = ModuleSystem::ModuleHandle;
}