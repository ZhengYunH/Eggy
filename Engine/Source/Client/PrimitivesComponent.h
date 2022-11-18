#pragma once
#include "Core/Object/IComponent.h"
#include "Core/Interface/IRenderScene.h"
#include "Core/Object/IMesh.h"
#include "Core/Object/IMaterial.h"


namespace Eggy
{
	class IPrimitivesComponent : public IComponent
	{
	public:
		virtual void CollectPrimitives(IRenderScene* renderScene) = 0;
	};

	class PrimitiveComponent : public IPrimitivesComponent
	{
	public:
		virtual void PreInitialize() override;
		virtual void CollectPrimitives(IRenderScene* renderScene);

	protected:
		Mesh* mMesh_;
		Material* mMaterial_;
	};
}

