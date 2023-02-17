#pragma once
#include "Core/Config.h"
#include "Core/Interface/ISystem.h"
#include "Core/DataType/Graph.h"

namespace Eggy
{
	class SystemManager : public TSystem<SystemManager>
	{
		DeclareSystem(SystemManager);
	public:
		void Initialize() override;
		void Finalize() override;

	public:
		void StartDependency(const String& system);
		void AddDependency(List<String> system);
		void EndDependency();

	private:
		void ConstructSystems();
		void InitializeSystems();
		void FinalizeSystems();

	private:
		String mSettingDependSystem;
		Map<String, List<String>> mSystemDependency;
		Map<String, ISystem*> mSystems;
		TGraph<ISystem*> mDependencyGraph;
	};
}
