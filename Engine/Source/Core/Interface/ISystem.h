#pragma once
#include "Core/Config.h"


namespace Eggy
{
	template<typename _TType>
	class ISystem
	{
	public:
		static _TType* Get()
		{
			return _TType::GInstance;
		}

		virtual void Initialize() = 0;
		virtual void Finalize() = 0;

	public:
		String GVersion{ "Unknown" };
	};

#define DeclareSystem(Type) private: static Type* GInstance; friend class ISystem; public:
#define DefineSystem(Type) Type* Type::GInstance = nullptr;
}