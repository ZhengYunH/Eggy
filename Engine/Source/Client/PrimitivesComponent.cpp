#include "PrimitivesComponent.h"
#include "Graphics/Elements/RenderElement.h"


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
			matRes->mShader_ = "Basic_VS";
		}
	}

	void PrimitiveComponent::PostInitialize()
	{
	}

	void PrimitiveComponent::CollectPrimitives(IRenderScene* renderScene)
	{
		if (!mMesh_ || mMesh_)
			return;
		IRenderElement* element = renderScene->AllocateRenderElement();
		element->mMesh = mMesh_;
		element->mMaterial = mMaterial_;

		IEntity* entity = GetParent();
		if (entity)
		{
			auto& objectInfo = element->mObjectInfo;

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

			{
				Matrix4x3 viewMat;
				viewMat.LookAt(Vector3(0.f, 0.f, -5.f), Vector3(0.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f));
				objectInfo.ViewTransform = viewMat;

				float fov = 45;
				float rad = DegreeToRadian(fov);
				float h = Cos(0.5f * rad) / Sin(0.5f * rad);
				float mScreenHeight_ = 600;
				float mScreenWidth_ = 800;
				float mNear_ = 1;
				float mFar_ = 5000;
				float w = h * mScreenHeight_ / mScreenWidth_;
				
				bool perspective = false;
				
				if (perspective)
				{
					objectInfo.ProjectTransform = Matrix4x4(
						w, 0, 0, 0,
						0, h, 0, 0,
						0, 0, mFar_ / (mNear_ - mFar_), -1,
						0, 0, -(mFar_ * mNear_) / (mFar_ - mNear_), 0
					);
				}
				else
				{
					objectInfo.ProjectTransform = Matrix4x4(
						2.f / w, 0, 0, 0,
						0, 2.f / h, 0, 0,
						0, 0, -1 / (mFar_ - mNear_), 0,
						0, 0, mNear_ / (mFar_ - mNear_), 1
					);
				}
			}
		}
		
		renderScene->SubmitRenderElement(ERenderSet::MAIN, element);
	}

	void PrimitiveComponent::EnterWorld(IWorld* world)
	{
		
	}

}
