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
			meshResource->mGeometry_ = new CubeMesh<EVF_P3F_C4B_T2F>();
			mMesh_->SetResource(meshResource);
		}
		
		{
			mMaterial_ = new Material();
			MaterialResource* matRes = new MaterialResource();
			matRes->Deserialize(nullptr);
			mMaterial_->SetResource(matRes);
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
			RenderItemInfo info;
			info.Object = mRenderObject_;
			info.Material_ = mMaterial_;
			element->PrepareRenderItemInfo(context, &info);
			RenderItem* item = context->GenerateRenderItem(&info);
			context->SubmitRenderItem(ERenderSet::MAIN, item);
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
