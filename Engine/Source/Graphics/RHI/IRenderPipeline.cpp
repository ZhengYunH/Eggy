#include "IRenderPipeline.h"
#include "IRenderPass.h"
#include "IShadingState.h"
#include "RenderItem.h"


namespace Eggy
{
	void RenderContext::Prepare()
	{
		CollectionLights();

		RenderPass* output = mPipeline_->Setup(&mBuilder_);
		mBuilder_.Prepare();
		mBuilder_.ResolveConnection(output, mPipeline_->GetRenderPasses());
		mPipeline_->Compile(&mBuilder_);
		mBuilder_.Resolve();
		mPipeline_->Render(this);
		mPipeline_->CopyRenderTargetList(std::move(mBuilder_.GetRenderTargetResource()));
	}
 
	void RenderContext::Clear()
	{
		mBuilder_.Clear();
		mPipeline_->Clear(this); 
		for (auto info : mRenderInfoItems_)
		{
			delete info;
		}
		mRenderInfoItems_.clear();
 	}

	void RenderContext::SubmitRenderItem(ERenderSet set, RenderItem* item)
	{
		DrawCall* dp = GenerateDrawCall(item);
		mPipeline_->AddDrawCall(set, dp);
	}

	RenderContext::RenderContext(IRenderScene* scene, RenderPipeline* pipeline) 
		: mScene_(scene)
		, mPipeline_(pipeline)
	{
		mPipeline_->mContext_ = this;
		auto collection = ShaderCollectionFactory::Instance().GetCollection("Basic");
		auto globalTable = collection->GetShaderTechnique(ETechnique::Shading)->GetConstantTable(EShaderConstant::Global);
		mParams_ = new ShadingParameterCollection(globalTable);

		auto lightTable = collection->GetShaderTechnique(ETechnique::Shading)->GetConstantTable(EShaderConstant::Light);
		mLightParams_ = new ShadingParameterCollection(lightTable);
	}

	RenderContext::~RenderContext()
	{

	}

	RenderItemInfo* RenderContext::AddRenderSceneInfo(RenderObject* object)
	{
		RenderItemInfo* info = new RenderItemInfo();
		mRenderInfoItems_.push_back(info);
		info->Object = object;
		return info;
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
		IShadingState* shadingState = info->ShadingState_;
		dp->ShadingState_ = shadingState;
		IShadingBatch* batch = shadingState->GetBatch();
		shadingState->SetDataFromMaterial(info->Material_);
		object->PrepareBatchData(batch);

		// ShaderCollection
		{
			dp->ShaderTechnique_ = const_cast<ShaderTechnique*>(shadingState->GetShaderCollection()->GetShaderTechnique(ETechnique::Shading));
		}
		
		// Geometry Binding
		{
			dp->GeometryBinding_ = info->GeometryBinding_; 
			dp->GeometryBinding_->Layout.VSShader = dp->ShaderTechnique_->GetStageInstance(EShaderStage::VS)->_ShaderRenderResource;
		}
		// TODO: setup Pipeline State
		return dp;
	}

	// ! must compatible with GenerateDrawCall
	void RenderContext::RecycleDrawCall(DrawCall* dp)
	{
		SafeDestroy(dp->Item_);
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
			if (!rt->IsBackBuffer)
			{
				SafeDestroy(rt);
			}
		}
		mRenderTargetResource_.clear();
	}

	DrawCall::~DrawCall()
	{
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
		if (rtIndex == RenderGraphBuilder::INVALID_RT)
			return nullptr;
		HYBRID_CHECK(rtIndex < mRenderTargetResource_.size());
		return mRenderTargetResource_[rtIndex];
	}

	void RenderContext::PrepareBatchData()
	{
		mParams_->SetMatrix4x4("cView", mConstant_.ViewTransform);
		mParams_->SetMatrix4x4("cProj", mConstant_.ProjectTransform);
		Vector3 viewPos = mConstant_.ViewPos;
		mParams_->SetFloat("ViewPos", 0, 3, mConstant_.ViewPos.GetPointer());
		mParams_->SetFloat("DebugColor", 0, 4, mConstant_.Color.GetPointer());

		int lightSize = (int)mLights_.size();
		mLightParams_->SetStruct("Lights", 0, (uint16)mLights_.size(), (const void**)mLights_.data());
		mLightParams_->SetInteger("LightCount", 0, 1, &lightSize);
	}

	void RenderContext::CollectionLights()
	{
		mLights_.clear();
		uint32 i = 0;
		for (auto light : mScene_->GetLights())
		{
			if (!light)
				continue;
			mLights_.push_back(light);
			++i;
			if (i >= MAX_LIGHT_COUNT)
				break;
		}
	}

}


