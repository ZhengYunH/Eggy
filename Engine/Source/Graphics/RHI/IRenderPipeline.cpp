#include "IRenderPipeline.h"
#include "IRenderPass.h"
#include "Core/Engine/Resource/Texture.h"


namespace Eggy
{
	void RenderContext::Prepare()
	{
		RenderPass* output = mPipeline_->Setup(&mBuilder_);
		mBuilder_.Prepare();
		mBuilder_.ResolveConnection(output, mPipeline_->GetRenderPasses());
		mPipeline_->Compile(&mBuilder_);
		mBuilder_.Resolve();
		mPipeline_->Render(this);
		mPipeline_->CopyRenderTargetList(mBuilder_.GetRenderTargetResource());
	}
 
	void RenderContext::Clear()
	{
		mBuilder_.Clear();
		mPipeline_->Clear(this); 
 	}

	void RenderContext::SubmitRenderItem(ERenderSet set, RenderItem* item)
	{
		DrawCall* dp = GenerateDrawCall(item);
		mPipeline_->AddDrawCall(set, dp);
	}

	RenderContext::~RenderContext()
	{

	}

	RenderItem* RenderContext::GenerateRenderItem(RenderItemInfo* info)
	{
		RenderItem* item = new RenderItem();
		item->Info = info;
		return item;
	}

	DrawCall* RenderContext::GenerateDrawCall(RenderItem* item)
	{
		DrawCall* dp = new DrawCall();
		dp->Item_ = item;
		RenderItemInfo* info = item->Info;
		RenderObject* object = info->Object;

		// ShaderCollection
		{
			dp->ShaderCollection_ = info->ShadingState_->ShaderCollection;
		}
		
		// Geometry Binding
		{
			dp->GeometryBinding_ = info->GeometryBinding_; 
			dp->GeometryBinding_->Layout.ShaderCollection = dp->ShaderCollection_;
		}

		// Resource Binding
		{
			dp->ResourceBinding_ = new ResourceBinding();
			auto resourceBinding = dp->ResourceBinding_;
			auto constantSize = dp->ShaderCollection_->GetConstantSize();
			auto textureSize = dp->ShaderCollection_->GetTextureSize();
			auto viewSize = dp->ShaderCollection_->GetViewSize();
			resourceBinding->Buffers = constantSize;
			resourceBinding->Textures = textureSize;
			resourceBinding->Views = viewSize;
			resourceBinding->Data = new IRenderResource * [constantSize + textureSize + viewSize];
			for (uint16 i = 0; i < constantSize + textureSize + viewSize; ++i)
				resourceBinding->Data[i] = nullptr;

			// Bind Batch Constant
			auto shaderCollection = dp->ShaderCollection_;
			uint8 batchSlot = shaderCollection->GetConstantSlot(EShaderConstant::Batch);
			auto batchConstant = info->ShadingState_->BindingConstants[batchSlot];
			batchConstant->Data = &object->ObjectConstantData_;
			batchConstant->ByteWidth = sizeof(object->ObjectConstantData_);
			batchConstant->Count = 1;
			resourceBinding->SetConstant(batchSlot, batchConstant);

			for (size_t i = 0; i < info->ShadingState_->BindingTextures.size(); ++i)
			{
				resourceBinding->SetTexture(static_cast<uint16>(i), info->ShadingState_->BindingTextures[i]);
			}
		}
		
		// TODO: setup Pipeline State
		return dp;
	}

	// ! must compatible with GenerateDrawCall
	void RenderContext::RecycleDrawCall(DrawCall* dp)
	{
		SafeDestroy(dp->Item_);
		SafeDestroyArray(dp->ResourceBinding_->Data); 
		SafeDestroy(dp->ResourceBinding_);
		SafeDestroy(dp);
	}

	RenderPipeline::~RenderPipeline()
	{
	}

	RenderPass* RenderPipeline::GenerateRenderPass()
	{
		RenderPass* pass = new RenderPass(this);
		return pass;
	}

	void RenderPipeline::AddRenderPass(RenderPass* pass)
	{
		pass->SetPipeline(this);
		mRenderPasses_.push_back(pass);
	}

	void RenderPipeline::AddDrawCallChannel(ERenderSets sets, RenderPass* pass)
	{
		uint8 bitCount = 0;
		while (bitCount < 32 && sets & 1 << bitCount)
		{
			AddDrawCallChannel(ERenderSet(1 << bitCount), pass);
		}
	}

	void RenderPipeline::AddDrawCallChannel(ERenderSet set, RenderPass* pass)
	{
		HYBRID_CHECK(std::find(mRenderPasses_.begin(), mRenderPasses_.end(), pass) != mRenderPasses_.end());
		if (mRenderPassSet_.find(set) == mRenderPassSet_.end())
		{
			mRenderPassSet_[set] = List<RenderPass*>();
		}

		mRenderPassSet_[set].push_back(pass);
	}

	void RenderPipeline::Compile(RenderGraphBuilder* builder)
	{
		for (RenderPass* renderPass : mRenderPasses_)
		{
			renderPass->SetPipeline(this);
			renderPass->Compile(builder);
		}
	}

	void RenderPipeline::Render(RenderContext* context)
	{
		for (RenderPass* renderPass : mRenderPasses_)
		{
			renderPass->Render(context);
		}
	}

	void RenderPipeline::Clear(RenderContext* context)
{
		mRenderPassSet_.clear();
		for (RenderPass* pass : mRenderPasses_)
		{
			pass->Clear(context);
		}
		mRenderPasses_.clear();

		for (auto rt : mRenderTargetResource_)
		{
			if(!rt->IsBackBuffer)
				delete rt;
		}
		mRenderTargetResource_.clear();
	}

	DrawCall::~DrawCall()
	{
		SafeDestroy(Item_);

	}

	void RenderPipeline::Consolidate()
	{
		for (RenderPass* renderPass : mRenderPasses_)
		{
			renderPass->Consolidate();
		}
	}

	void RenderPipeline::AddDrawCall(ERenderSet set, DrawCall* dp)
	{
		HYBRID_CHECK(mRenderPassSet_.find(set) != mRenderPassSet_.end());
		for (auto renderPass : mRenderPassSet_[set])
		{
			renderPass->AddDrawCall(dp);
		}
	}

	void RenderPipeline::CopyRenderTargetList(List<IRenderTarget*>&& RenderTargets)
	{
		mRenderTargetResource_.swap(RenderTargets);
	}

	IRenderTarget* RenderPipeline::GetRenderTargetResource(size_t rtIndex) const 
	{
		HYBRID_CHECK(rtIndex < mRenderTargetResource_.size());
		return mRenderTargetResource_[rtIndex];
	}

}


