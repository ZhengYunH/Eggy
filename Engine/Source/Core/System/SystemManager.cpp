#include "SystemManager.h"

#include "InputSystem.h"
#include "ConfigSystem.h"
#include "FileSystem.h"
#include "RenderSystem.h"
#include "TextureSystem.h"
#include "LoggerSystem.h"


namespace Eggy
{
	DefineSystem(SystemManager);

	void SystemManager::Initialize()
	{
		ConstructSystems();
		InitializeSystems();
	}

	void SystemManager::Finalize()
	{
		FinalizeSystems();
	}

	void SystemManager::ConstructSystems()
	{
#define AddSystem(System) { ISystem* sys = new System(); AddSystemDependency<System>(); mSystems[StaticName(System)] = sys; }

		AddSystem(InputSystem);
		AddSystem(ConfigSystem);
		AddSystem(FileSystem);
		AddSystem(RenderSystem);
		AddSystem(TextureSystem);
		AddSystem(LoggerSystem);

#undef AddSystem
	}

	void SystemManager::InitializeSystems()
	{
		// build dependency graph
		Map<String, TNode<ISystem*>*> nodes;
		for (auto& pair : mSystems)
		{
			auto node = mDependencyGraph.AddNode(pair.second);
			nodes[pair.first] = node;
		}

		for (auto pair : mSystemDependency)
		{
			auto node = nodes.at(pair.first);
			for (auto dep : pair.second)
			{
				nodes.at(dep)->Link(node);
			}
		}

		List<TNode<ISystem*>*> depList;
		mDependencyGraph.GetDependencyList(depList);

		for (auto node : depList)
		{
			node->mObject_->Initialize();
		}
	}

	void SystemManager::FinalizeSystems()
	{
		List<TNode<ISystem*>*> depList;
		mDependencyGraph.GetDependencyList(depList);
		for (auto itr = depList.rbegin(); itr != depList.rend(); ++itr)
		{
			(*itr)->mObject_->Finalize();
		}

		for (auto pair : mSystems)
		{
			SafeDestroy(pair.second);
		}
	}

	void SystemManager::StartDependency(const String& system)
	{
		mSettingDependSystem = system;
		mSystemDependency[system] = List<String>();
	}

	void SystemManager::AddDependency(List<String> system)
	{
		mSystemDependency[mSettingDependSystem].swap(system);
	}

	void SystemManager::EndDependency()
	{
		mSettingDependSystem.clear();
	}

}

