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
}

