#pragma once
#include "Interface/IRenderScene.h"
#include "Object/IComponent.h"
#include "Graphics/Light/Light.h"


namespace Eggy
{
	class DirectionLightComponent : public IComponent
	{
	public:
		void EnterWorld() override;

		void LeaveWorld() override;

		String GetName() const override;

	protected:
		void PostInitialize() override;

	protected:
		DirectionLight* mLight_;
	};
}


