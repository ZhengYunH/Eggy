#include "ModelResource.h"
#include "ResourceItem.h"
#include "MaterialResource.h"
#include "MeshResource.h"


namespace Eggy
{
	DefineResource(ModelResource, EResourceType::Model);

	bool ModelResource::LoadObject() noexcept
	{
		HYBRID_CHECK(mMaterial_ && mMaterial_->IsLoaded());
		HYBRID_CHECK(mMesh_ && mMesh_->IsLoaded());
		SetLoaded();
		return true;
	}

	bool ModelResource::SaveObject() noexcept
	{
		Unimplement();
		return false;
	}

	void ModelResource::AddDependency(TSharedPtr<ResourceObject>& dep)
	{
		ResourceObject::AddDependency(dep);
		EResourceType type = dep->GetItem()->GetType();
		if (type == EResourceType::Material)
			mMaterial_ = dep;
		else if (type == EResourceType::Mesh)
			mMesh_ = dep;
	}

	void ModelResource::ResetDependency()
	{
		ResourceObject::ResetDependency();
		mMaterial_ = nullptr;
		mMesh_ = nullptr;
	}

}