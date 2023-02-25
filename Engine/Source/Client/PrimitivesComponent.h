#pragma once
#include "Interface/IRenderScene.h"
#include "Object/IComponent.h"
#include "Object/Model.h"
#include "Object/IPrimitive.h"


namespace Eggy
{
	class IPrimitivesComponent : public IComponent, public IPrimitive
	{
	public:
		String GetName() const override { return "IPrimitivesComponent"; }

	};

	class PrimitiveComponent : public IPrimitivesComponent
	{
	public:
		virtual void CollectPrimitives(RenderContext* context) override;
		void LoadResource(String resource);

		void EnterWorld() override;
		void LeaveWorld() override;

	protected:
		virtual void PreInitialize() override;
		virtual void PostInitialize() override;

		virtual void Destroy() override;

	protected:
		RenderObject mRenderObject_;
		Model mModel_;
	};
}

