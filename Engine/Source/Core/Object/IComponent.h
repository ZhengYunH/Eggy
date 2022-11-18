#pragma once
#include "IObject.h"
#include "IEntity.h"


namespace Eggy
{
	class IComponent : public IObject
	{
		friend class IEntity;
	public:
		IComponent() = default;
		
	public:
		void AddToEntity(IEntity* parent);
		IEntity* GetParent();

	protected:
		virtual void PreInitialize() {}
		virtual void PostInitialize() {}

	protected:
		IEntity* mParent_{ nullptr };
	};
}