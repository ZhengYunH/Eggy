#pragma once
#include "IObject.h"
#include "IEntity.h"
#include "IWorld.h"


namespace Eggy
{
	class IComponent : public IObject
	{
		friend class IEntity;
	public:
		IComponent() = default;
		String Serialize() const override;
		void Deserialize(const String& s) override;

	public:
		void AddToEntity(IEntity* parent);
		virtual void EnterWorld() {}
		virtual void LeaveWorld() {}
		virtual String GetName() const = 0;
		IEntity* GetParent();

	protected:
		virtual void PreInitialize() {}
		virtual void PostInitialize() {}

	protected:
		IEntity* mParent_{ nullptr };
	};
}