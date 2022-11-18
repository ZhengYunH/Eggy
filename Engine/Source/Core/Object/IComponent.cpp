#include "IComponent.h"
#include "IEntity.h"


namespace Eggy
{
	void IComponent::AddToEntity(IEntity* parent)
	{
		PreInitialize();
		mParent_ = parent;
		PostInitialize();
	}

	IEntity* IComponent::GetParent()
	{
		return mParent_;
	}

}