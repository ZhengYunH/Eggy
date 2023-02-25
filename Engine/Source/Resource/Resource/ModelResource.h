#pragma once
#include "Core/Config.h"
#include "Resource/ResourceObject.h"
#include "MaterialResource.h"
#include "MeshResource.h"


namespace Eggy
{
	class ModelResource : public ResourceObject
	{
		DeclareResource(ModelResource, EResourceType::Model);
	public:
		bool LoadObject() noexcept override;
		bool SaveObject() noexcept override;
		void AddDependency(TSharedPtr<ResourceObject>& dep) override;
		void ResetDependency() override;

		TSharedPtr<MeshResource> GetMesh() const { return mMesh_; }
		TSharedPtr<MaterialResource> GetMaterial() const { return mMaterial_; }

	protected:
		TSharedPtr<MeshResource> mMesh_;
		TSharedPtr<MaterialResource> mMaterial_;
	};
}