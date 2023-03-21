#include "Model.h"
#include "Object/Mesh.h"
#include "Object/Material.h"
#include "ResourceModule.h"

namespace Eggy
{

	Model::~Model()
	{
		SafeDestroy(mMaterial_);
		SafeDestroy(mMesh_);
	}

	void Model::SetResourceID(const Guid& resourceID)
	{
		mResource_ = ResourceModule::Instance().Load(resourceID, [this, token=++mToken_]()
		{
			_OnResourceLoaded(token);
		});
	}

	bool Model::IsReady()
	{
		return mMesh_ && mMaterial_;
	}

	void Model::_OnResourceLoaded(uint8 token)
	{
		if (token != mToken_)
			return;
		SetMesh(new Mesh(mResource_->GetMesh()));
		SetMaterial(new Material(mResource_->GetMaterial()));
	}

}