#pragma once
#include "Core/Config.h"

#define CheckSingleton(Instance) HYBRID_CHECK(!Instance)
namespace Eggy
{
	class ISystem
	{
	public:
		virtual void Initialize() = 0;
		virtual void Finalize() = 0;
	};

	template<typename _TType>
	class TSystem : public ISystem
	{
	public:
		TSystem()
		{
		}

		virtual ~TSystem()
		{
			_TType::GInstance = nullptr;
		}

		static _TType* Get()
		{
			return _TType::GInstance;
		}

	public:
		String GVersion{ "Unknown" };
	};

	template<typename System>
	void AddSystemDependency() {}

#define DeclareSystem(Type) private: static Type* GInstance; friend class TSystem<Type>; public: Type() { HYBRID_CHECK(!GInstance); GInstance = this; }
#define DefineSystem(Type) Type* Type::GInstance = nullptr;

#define DeclareDependency(Type) template<> void AddSystemDependency<typename Type>();
#define DefineDependency(Type, ...) \
	template<> void AddSystemDependency<Type>() \
	{\
		SystemManager::Get()->StartDependency(#Type);\
		SystemManager::Get()->AddDependency({STRINGIFY_MULTI(##__VA_ARGS__ )});\
		SystemManager::Get()->EndDependency();\
	}
}