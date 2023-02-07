#include "IRenderPipeline.h"

namespace Eggy
{
	void RenderContext::Prepare()
	{

	}
 
	void RenderContext::Clear()
	{
		mPipeline_->Clear(); 
 	}


	void RenderContext::SubmitRenderItem(ERenderSet set, RenderItem* item)
	{
		DrawCall* dp = GenerateDrawCall(item);
		mPipeline_->AddDrawCall(set, dp);
		while (dp)
 		{
 			uint8 globalConstantSlot = dp->ShaderCollection_->GetConstantSlot(EShaderConstant::Global);
			if (globalConstantSlot == IShaderCollection::INVALID_SLOT || globalConstantSlot >= dp->ResourceBinding_->Buffers)
 			{
 				dp = dp->Next_;
 				continue;
 
 			}
 			dp->ResourceBinding_->Data[globalConstantSlot] = new IConstantBuffer();
			auto& globalConstant = mPipeline_->GetGlobalConstant();
			static_cast<IConstantBuffer*>(dp->ResourceBinding_->Data[globalConstantSlot])->Data = &globalConstant;
			static_cast<IConstantBuffer*>(dp->ResourceBinding_->Data[globalConstantSlot])->ByteWidth = sizeof(globalConstant);
			static_cast<IConstantBuffer*>(dp->ResourceBinding_->Data[globalConstantSlot])->Count = 1;
 			dp = dp->Next_;
 		}
	}

	void RenderPass::AddDrawCall(DrawCall* drawCall)
	{
		if (!DrawCallHead)
		{
			DrawCallHead = DrawCallLast = drawCall;
			return;
		}
		DrawCallLast->Next_ = drawCall;
		DrawCallLast = drawCall;
		bIsConsolidated = false;
	}

	void RenderPass::Clear()
	{
		while (DrawCallHead)
		{
			DrawCall* dp = DrawCallHead;
			DrawCallHead = DrawCallHead->Next_;
			delete dp;
		}
		DrawCallHead = nullptr;
		DrawCallLast = nullptr;
		bIsConsolidated = false;
	}

	void RenderPass::Consolidate()
	{
		if (bIsConsolidated)
			return;
		DrawCall* dp = DrawCallHead;
		while (dp)
		{
			uint8 globalConstantSlot = dp->ShaderCollection_->GetConstantSlot(EShaderConstant::Global);
			if (globalConstantSlot == IShaderCollection::INVALID_SLOT || globalConstantSlot >= dp->ResourceBinding_->Buffers)
			{
				dp = dp->Next_;
				continue;

			}
			dp->ResourceBinding_->Data[globalConstantSlot] = new IConstantBuffer();
			auto& globalConstant = Pipeline->GetGlobalConstant();
			IConstantBuffer* globalConstantBuffer = static_cast<IConstantBuffer*>(dp->ResourceBinding_->Data[globalConstantSlot]);
			globalConstantBuffer->Data = &globalConstant;
			globalConstantBuffer->ByteWidth = sizeof(globalConstant);
			globalConstantBuffer->Count = 1;
			dp = dp->Next_;
		}
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
			dp->ShaderCollection_ = info->Material_->GetShaderCollection();
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
			auto viewSize = 0;
			resourceBinding->Buffers = constantSize;
			resourceBinding->Textures = textureSize;
			resourceBinding->Data = new IRenderResource * [constantSize + textureSize + viewSize];
			for (uint16 i = 0; i < constantSize + textureSize + viewSize; ++i)
				resourceBinding->Data[i] = nullptr;

			auto shaderCollection = dp->ShaderCollection_;
			resourceBinding->Data[shaderCollection->GetConstantSlot(EShaderConstant::Batch)] = new IConstantBuffer();
			auto batchConstant = static_cast<IConstantBuffer*>(resourceBinding->Data[dp->ShaderCollection_->GetConstantSlot(EShaderConstant::Batch)]);
			batchConstant->Data = &object->ObjectConstantData_;
			batchConstant->ByteWidth = sizeof(object->ObjectConstantData_);
			batchConstant->Count = 1;

			for (auto& pair : info->Material_->GetTextures())
			{
				resourceBinding->Data[constantSize + shaderCollection->GetTextureSlot(pair.first)] = new ITexture();
				auto bindingTex = static_cast<ITexture*>(resourceBinding->Data[constantSize + shaderCollection->GetTextureSlot(pair.first)]);
				auto& resource = pair.second->GetResource();
				bindingTex->Data = resource.GetData();
				bindingTex->Width = resource.Size.x;
				bindingTex->Height = resource.Size.y;
				bindingTex->Mips = resource.Mips;
				bindingTex->Format = resource.Format;
				bindingTex->ByteWidth = resource.ByteWidth;
				bindingTex->TextureType = resource.TextureType;
			}
		}
		
		// TODO: setup Pipeline State
		return dp;
	}

	RenderPipeline::~RenderPipeline()
	{
		Clear();
	}

	RenderPass* RenderPipeline::AddRenderPass()
	{
		RenderPass* pass = new RenderPass(this);
		return pass;
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
		if (std::find(mRenderPasses_.begin(), mRenderPasses_.end(), pass) == mRenderPasses_.end())
			mRenderPasses_.push_back(pass);

		if (mRenderPassSet_.find(set) == mRenderPassSet_.end())
		{
			mRenderPassSet_[set] = List<RenderPass*>();
		}

		mRenderPassSet_[set].push_back(pass);
	}

	void RenderPipeline::Clear()
	{
		mRenderPassSet_.clear();

		for (RenderPass* pass : mRenderPasses_)
		{
			pass->Clear();
			delete pass;
		}
		mRenderPasses_.clear();
	}

	DrawCall::~DrawCall()
	{
	}

	void RenderPipeline::Consolidate()
	{
		for (auto& pair : mRenderPassSet_)
		{
			for (auto renderPass : pair.second)
			{
				renderPass->Consolidate();
			}
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

}


