#include "RenderScene.h"
#include "Graphics/RHI/IRenderHeader.h"
#include "Graphics/Elements/RenderElement.h"
#include "Camera.h"

namespace Eggy
{
	RenderScene::RenderScene()
	{
		for (uint32 i = uint32(ERenderSet::START); i < uint32(ERenderSet::END); ++i)
		{
			mRenderObjects_[ERenderSet(i)] = List<IRenderObject*>();
		}

		mPipeline_ = new RenderPipeline();
		mContext_ = new RenderContext(mPipeline_);
	}

	RenderScene::~RenderScene()
	{
		ClearResourcePool();
	}

	void RenderScene::StartFrame()
	{
		RenderPass* pass = mPipeline_->AddRenderPass();
		mPipeline_->AddDrawCallChannel(ERenderSet::MAIN, pass);
	}

	void RenderScene::EndFrame()
	{
		mContext_->Prepare();
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

	void RenderScene::ClearRenderObjects()
	{
		if (mRenderObjects_.empty())
			return;

		// clear RenderObject
		for (auto& pair : mRenderObjects_)
		{
			auto& objects = pair.second;
			for (auto* obj : objects)
			{
				delete obj;
			}
		}
		mRenderObjects_.clear();
	}

	void RenderScene::ClearRenderElements()
	{
		if (mRenderElements_.empty())
			return;

		// clear RenderElement
		for (auto ele : mRenderElements_)
		{
			delete ele;
		}
		mRenderElements_.clear();
	}

	void RenderScene::ClearResourcePool()
	{
	}
}
