#include "PrimitivesComponent.h"
#include "Graphics/Elements/RenderElement.h"
#include "Client/Camera.h"
#include "ResourceModule.h"
#include "ResourceItem.h"
#include "Object/Mesh.h"


namespace Eggy
{

	void PrimitiveComponent::LoadResource(String resource)
	{
		ResourceItem* item = ResourceModule::Instance().GetItem(resource);
		if (item)
			mModel_.SetResourceID(item->GetResourceID());
	}

	void PrimitiveComponent::PreInitialize()
	{
	}

	void PrimitiveComponent::PostInitialize()
	{
	}

	void PrimitiveComponent::Destroy()
	{
	}

	void PrimitiveComponent::CollectPrimitives(RenderContext* context)
	{
		if (!mModel_.IsReady())
			return;

		IEntity* entity = GetParent();
		mRenderObject_.ObjectConstantData_.ModelTransform = entity->GetTransform();

		IRenderMesh* renderMesh = mModel_.GetMesh()->GetRenderMesh();
		Material* material = mModel_.GetMaterial();
		for (size_t i = 0; i < renderMesh->GetElementsSize(); ++i)
		{
			IRenderElement* element = renderMesh->GetRenderElement(i);
			RenderItemInfo* info = context->AddRenderSceneInfo(&mRenderObject_);
			info->Material_ = material;
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
