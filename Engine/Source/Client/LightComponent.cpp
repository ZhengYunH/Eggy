#include "LightComponent.h"


namespace Eggy
{
	void DirectionLightComponent::PostInitialize()
	{
		IEntity* parent = GetParent();
		auto transform = parent->GetTransform();
		Vector3 ambient(0.1f, 0.1f, 0.1f);
		Vector3 diffuse(0.4f, 0.4f, 0.4f);
		Vector3 specular(0.5f, 0.5f, 0.5f);
		mLight_ = new DirectionLight(transform.GetZAxis(), ambient, diffuse, specular);
	}

	void DirectionLightComponent::EnterWorld()
	{
		GetParent()->GetWorld()->GetRenderScene()->AddLight(mLight_);
	}

	void DirectionLightComponent::LeaveWorld()
	{
		GetParent()->GetWorld()->GetRenderScene()->DelLight(mLight_);
	}

	String DirectionLightComponent::GetName() const
	{
		return "DirectionLightComponent";
	}

}
