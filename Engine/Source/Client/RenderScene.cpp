#include "RenderScene.h"
#include "Graphics/RHI/IRenderHeader.h"
#include "Graphics/Elements/RenderElement.h"
#include "Graphics/Pipeline/ForwardPipeline.h"
#include "Camera.h"


namespace Eggy
{
	RenderScene::RenderScene()
	{
		mPipeline_ = new ForwardPipeline();
		mContext_ = new RenderContext(mPipeline_);
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
		auto& globalConstant = mPipeline_->GetGlobalConstant();
		globalConstant.ProjectTransform = GetCamera()->getProjMatrix();
		globalConstant.ViewTransform = GetCamera()->getViewMatrix();
		mPipeline_->Consolidate();
	}

	void RenderScene::StartDeviceFrame()
	{
	}

	void RenderScene::EndDeviceFrame()
	{
		mContext_->Clear();
 	}
}
