#include "LightComponent.h"


namespace Eggy
{
	void LightComponent::PostInitialize()
	{
		InitLightData();
	}

	void LightComponent::EnterWorld()
	{
		GetParent()->GetWorld()->GetRenderScene()->AddLight(mLight_);
	}

	void LightComponent::LeaveWorld()
	{
		GetParent()->GetWorld()->GetRenderScene()->DelLight(mLight_);
	}

	String LightComponent::GetName() const
	{
		return "LightComponent";
	}

	void DirectionLightComponent::InitLightData()
	{
		IEntity* parent = GetParent();
		auto transform = parent->GetTransform();
		mLight_ = new DirectionLight(transform.GetZAxis(), mColor_.ToVector3() / 255.f, mIntensity_);
	}

	void PointLightComponent::InitLightData()
	{
		IEntity* parent = GetParent();
		auto transform = parent->GetTransform();

		PointLight* pointLight = new PointLight(transform.GetTranslation(), mColor_.ToVector3() / 255.f, mIntensity_ * 4 * MATH_PI);
		mLight_ = pointLight;

	}

	void SpotLightComponent::InitLightData()
	{
		IEntity* parent = GetParent();
		auto transform = parent->GetTransform();
		SpotLight* spotLight = new SpotLight(transform.GetTranslation(), transform.GetZAxis(), mColor_.ToVector3() / 255.f, mIntensity_ * 4 * MATH_PI);
		mLight_ = spotLight;
	}
}
