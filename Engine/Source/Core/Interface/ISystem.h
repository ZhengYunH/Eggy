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

#define DeclareDependency(Type, ...) \
	template<Type> void AddSystemDependency() \
	{\
		SystemManager::Get()->StartDenpendency(#Type);\
		SystemManager::Get()->AddDependency({ ##__VA_ARGS__ });\
		SystemManager::Get()->EndDenpendency();\
	}
}