#pragma once
#include "Core/Object/IComponent.h"
#include "Core/Interface/IRenderScene.h"
#include "Core/Object/Mesh.h"
#include "Core/Object/Material.h"
#include "Core/Object/IPrimitive.h"


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
		void LoadResource(String resource) { mResource_ = resource; }

		void EnterWorld() override;
		void LeaveWorld() override;

	protected:
		virtual void PreInitialize() override;
		virtual void PostInitialize() override;

		virtual void Destroy() override;

	protected:
		Mesh* mMesh_;
		Material* mMaterial_;
		RenderObject* mRenderObject_;
		String mResource_;
	};
}

