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
			meshResource->mGeometry_ = new QuatMesh();
			mMesh_->SetResource(meshResource);
		}
		
		{
			mMaterial_ = new Material();
			MaterialResource* matRes = new MaterialResource();
			matRes->mShader_ = "Basic";
			mMaterial_->SetResource(matRes);
		}
	}

	void PrimitiveComponent::PostInitialize()
	{
	}

	void PrimitiveComponent::Destroy()
	{
		SafeDestroy(mMaterial_);
		SafeDestroy(mMesh_);
	}

	void PrimitiveComponent::CollectPrimitives(IRenderScene* renderScene)
	{
		if (!mMesh_ || !mMaterial_)
			return;
		IRenderElement* element = renderScene->AllocateRenderElement();
		element->mMesh = mMesh_;
		element->mMaterial = mMaterial_;

		IEntity* entity = GetParent();
		if (entity)
		{
			auto& objectInfo = element->mObjectInfo;

			if(false)
			{
				static float phi = 0.0f, theta = 0.0f;
				Vector3 Translation;
				phi += 0.00001f;
				theta += 0.00015f;
				Matrix4x3 modelTransform = entity->GetTransform();
				modelTransform.SetRotationX(phi, Translation);
				modelTransform.SetRotationY(theta, Translation);
				entity->SetTransform(modelTransform);
			}

			objectInfo.ModelTransform = entity->GetTransform();
			objectInfo.ViewTransform = renderScene->GetCamera()->getViewMatrix();;
			objectInfo.ProjectTransform = renderScene->GetCamera()->getProjMatrix();
		}
		
		renderScene->SubmitRenderElement(ERenderSet::MAIN, element);
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
