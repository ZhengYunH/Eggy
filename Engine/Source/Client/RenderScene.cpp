#include "RenderScene.h"
#include "Graphics/RHI/IRenderHeader.h"
#include "Graphics/RHI/ILight.h"
#include "Graphics/Elements/RenderElement.h"
#include "Graphics/Pipeline/ForwardPipeline.h"
#include "Graphics/Pipeline/DeferredPipeline.h"
#include "Camera.h"


namespace Eggy
{
	RenderScene::RenderScene()
	{
		if (RenderSystem::Get()->IsDeferredShading())
			mPipeline_ = new DeferredPipeline();
		else
			mPipeline_ = new ForwardPipeline();
		mContext_ = new RenderContext(this, mPipeline_);
	}

	RenderScene::~RenderScene()
	{
	}

	void RenderScene::StartFrame()
	{
		mContext_->Prepare();
	}

	void RenderScene::EndFrame()
	{
		auto& globalConstant = mContext_->GetGlobalConstant();
		Camera* camera = GetCamera();
		HYBRID_CHECK(camera);
		globalConstant.ProjectTransform = camera->getProjMatrix();
		globalConstant.ViewTransform = camera->getViewMatrix();
		globalConstant.CamData.ViewPos = camera->GetTransform().GetTranslation();
		globalConstant.CamData.BasisX = camera->GetWBasisX();
		globalConstant.CamData.BasisY = camera->GetWBasisY();
		globalConstant.CamData.BasisZ = camera->GetWBasisZ();
		globalConstant.CamData.Info = Vector4(camera->GetNearPlane(), camera->GetFarPlane(), 1.0 / camera->GetNearPlane(), 1.0 / camera->GetFarPlane());
		mContext_->PrepareBatchData();
		mPipeline_->Consolidate();
	}

	void RenderScene::StartDeviceFrame()
	{
	}

	void RenderScene::EndDeviceFrame()
	{
		mContext_->Clear();
 	}

	void RenderScene::AddLight(ILight* light)
	{
		HYBRID_CHECK(std::find(mLights_.begin(), mLights_.end(), light) == mLights_.end());
		mLights_.push_back(light);
	}

	void RenderScene::DelLight(ILight* light)
	{
		if (auto itr = std::find(mLights_.begin(), mLights_.end(), light); itr != mLights_.end())
		{
			mLights_.erase(itr);
		}
	}

	List<ILight*>& RenderScene::GetLights()
	{
		return mLights_;
	}

}
