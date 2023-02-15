#include "PrimitivesComponent.h"
#include "Graphics/Elements/RenderElement.h"
#include "Client/Camera.h"


namespace Eggy
{

	void PrimitiveComponent::PreInitialize()
	{
		{
			mMesh_ = new Mesh();
			MeshResource* meshResource = new MeshResource();
			if (mResource_.empty())
				meshResource->SetGeometry(new CubeMesh<EVF_P3F_C4B_T2F>());
			else
				meshResource->Deserialize(FileSystem::Get()->LoadFile(mResource_).get());
			mMesh_->SetResource(meshResource);
		}
		
		{
			mMaterial_ = new Material();
			MaterialResource* matRes = new MaterialResource();
			matRes->Deserialize(nullptr);
			mMaterial_->SetResource(matRes);
		}

		IRenderMesh* renderMesh = mMesh_->GetRenderMesh();
		for (size_t i = 0; i < renderMesh->GetElementsSize(); ++i)
		{
			IRenderElement* element = renderMesh->GetRenderElement(i);
			element->Initialize(mMaterial_);
		}

		mRenderObject_ = new RenderObject();
	}

	void PrimitiveComponent::PostInitialize()
	{
	}

	void PrimitiveComponent::Destroy()
	{
		SafeDestroy(mMaterial_);
		SafeDestroy(mMesh_);
	}

	void PrimitiveComponent::CollectPrimitives(RenderContext* context)
	{
		if (!mMesh_ || !mMaterial_)
			return;

		IEntity* entity = GetParent();

		mRenderObject_->ObjectConstantData_.ModelTransform = entity->GetTransform();

		IRenderMesh* renderMesh = mMesh_->GetRenderMesh();
		for (size_t i = 0; i < renderMesh->GetElementsSize(); ++i)
		{
			IRenderElement* element = renderMesh->GetRenderElement(i);
			RenderItemInfo* info = context->AddRenderSceneInfo(mRenderObject_);
			element->PrepareRenderItemInfo(context, info);
			RenderItem* item = context->GenerateRenderItem(info);
			context->SubmitRenderItem(ERenderSet::Main, item);
		}
	}

	void PrimitiveComponent::EnterWorld()
	{
		IWorld* world = GetParent()->GetWorld();
		world->AddPrimitives(this);
	}

	void PrimitiveComponent::LeaveWorld()
	{
		IWorld* world = GetParent()->GetWorld();
		world->DelPrimitives(this);
	}

}
