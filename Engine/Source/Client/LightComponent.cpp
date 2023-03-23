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
		Vector3 ambient(1.0f, 1.0f, 1.0f);
		Vector3 diffuse(0.4f, 0.4f, 0.4f);
		Vector3 specular(0.5f, 0.5f, 0.5f);
		mLight_ = new DirectionLight(transform.GetZAxis(), ambient, diffuse, specular);
	}

	void PointLightComponent::InitLightData()
	{
		IEntity* parent = GetParent();
		auto transform = parent->GetTransform();

		Vector3 ambient(1.0f, 1.0f, 1.0f);
		Vector3 diffuse(0.4f, 0.4f, 0.4f);
		Vector3 specular(0.5f, 0.5f, 0.5f);
		PointLight* pointLight = new PointLight(transform.GetTranslation(), ambient, diffuse, specular);
		mLight_ = pointLight;

	}

	void SpotLightComponent::InitLightData()
	{
		IEntity* parent = GetParent();
		auto transform = parent->GetTransform();

		Vector3 ambient(1.0f, 1.0f, 1.0f);
		Vector3 diffuse(0.4f, 0.4f, 0.4f);
		Vector3 specular(0.5f, 0.5f, 0.5f);
		SpotLight* spotLight = new SpotLight(transform.GetTranslation(), transform.GetZAxis(), ambient, diffuse, specular);
		mLight_ = spotLight;
	}
}
