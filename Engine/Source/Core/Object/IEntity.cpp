#include "IEntity.h"
#include "IComponent.h"


namespace Eggy
{
	bool IEntity::AddComponent(IComponent* comp)
	{
		comp->AddToEntity(this);
		mComponents_.push_back(comp);
		return true;
	}

	String IEntity::Serialize() const
	{
		String s;
		for (auto& comp : mComponents_)
		{
			s += comp->GetName() + ":" + comp->Serialize();
		}
		return s;
	}

	void IEntity::Deserialize(const String& s)
	{
		
	}

	void IEntity::EnterWorld(IWorld* world)
	{
		mWorld_ = world;
		for (auto& comp : mComponents_)
		{
			comp->EnterWorld();
		}
	}

	void IEntity::LeaveWorld()
	{
		for (auto& comp : mComponents_)
		{
			comp->LeaveWorld();
		}
		mWorld_ = nullptr;
	}
}

