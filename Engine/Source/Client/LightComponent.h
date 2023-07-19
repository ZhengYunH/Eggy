#pragma once
#include "Interface/IRenderScene.h"
#include "Object/IComponent.h"
#include "Graphics/Light/Light.h"
#include "Graphics/Renderer/VertexFactory.h"

namespace Eggy
{
	class LightComponent : public IComponent
	{
	public:
		void EnterWorld() override;
		void LeaveWorld() override;
		String GetName() const override;

	protected:
		void PostInitialize() override;
		virtual void InitLightData() = 0;
		
		Color4B mColor_{ Color4B_WRITE };
		float mIntensity_{ 1.0f };
		ILight* mLight_{ nullptr };
	};

	class DirectionLightComponent : public LightComponent
	{
	protected:
		void InitLightData() override;
	};

	class PointLightComponent : public LightComponent
	{
	protected:
		void InitLightData() override;
	};

	class SpotLightComponent : public LightComponent
	{
	protected:
		void InitLightData() override;
	};
}


