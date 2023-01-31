#pragma once
#include "Core/Object/IComponent.h"
#include "Core/Interface/IRenderScene.h"
#include "Core/Object/IMesh.h"
#include "Core/Object/IMaterial.h"
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
		virtual void CollectPrimitives(IRenderScene* renderScene) override;

		void EnterWorld() override;
		
		void LeaveWorld() override;

	protected:
		virtual void PreInitialize() override;
		virtual void PostInitialize() override;

		virtual void Destroy() override;

	protected:
		Mesh* mMesh_;
		Material* mMaterial_;


	};
}

