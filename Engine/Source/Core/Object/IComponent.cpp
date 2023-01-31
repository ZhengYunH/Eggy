#include "IComponent.h"
#include "IEntity.h"


namespace Eggy
{

	IComponent::~IComponent()
	{
		Destroy();
	}

	String IComponent::Serialize() const
	{
		return "";
	}

	void IComponent::Deserialize(const String& s)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

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