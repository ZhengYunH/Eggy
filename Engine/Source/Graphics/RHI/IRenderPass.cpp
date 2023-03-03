#include "IRenderPass.h"
#include "RenderGraph.h"


namespace Eggy
{
	size_t RenderPass::GetOutput()
	{
		HYBRID_CHECK(!mOutputRTs.empty());
		return mOutputRTs[0];
	}

	size_t RenderPass::GetOutput(uint8 index)
	{
		HYBRID_CHECK(index < mOutputRTs.size());
		return mOutputRTs[index];
	}

	const size_t RenderPass::GetOutputCount()
	{
		return mOutputRTs.size();
	}

	const RenderTargetDesc& RenderPass::GetOutputTarget(RenderGraphBuilder* builder)
	{
		return builder->GetOutputTarget(GetOutput());
	}

	void RenderPass::SetOutput(RenderGraphBuilder* builder, const RenderTargetDesc& desc, uint8 rtIndex /*= 0*/)
	{
		if (rtIndex >= mOutputRTs.size())
		{
			mOutputRTs.resize(rtIndex + 1, 0);
		}
		mOutputRTs[rtIndex] = builder->NewRenderTarget(this, desc);
	}

	void RenderPass::SetOutput(RenderGraphBuilder* builder, size_t targetOuput, uint8 rtIndex /*= 0*/)
	{
		if (builder->CheckRenderTargetValidation(targetOuput))
		{
			if (rtIndex >= mOutputRTs.size())
			{
				mOutputRTs.resize(rtIndex + 1, 0);
			}
			mOutputRTs[rtIndex] = targetOuput;
		}
		else
		{
			Unimplement(0);
		}
	}

	void RenderPass::SetDepthStencil(RenderGraphBuilder* builder, const RenderTargetDesc& desc)
	{
		HYBRID_CHECK(desc.IsDepthStencil);
		size_t outputIndex = 0;
		mDepthStencil = builder->NewRenderTarget(this, desc);
	}

	void RenderPass::SetDepthStencil(RenderGraphBuilder* builder, size_t targetOuput)
	{
		if (builder->CheckRenderTargetValidation(targetOuput))
		{
			HYBRID_CHECK(builder->GetOutputTarget(targetOuput).IsDepthStencil);
			mDepthStencil = targetOuput;
		}
		else
		{
			Unimplement(0);
		}
	}

	void RenderPass::Sample(RenderGraphBuilder* builder, size_t rtIndex)
	{
		mSamples.push_back(rtIndex);
	}

	void RenderPass::AddDrawCall(DrawCall* drawCall)
	{
		drawCall->Pass_ = this;
		if (!DrawCallHead)
		{
			DrawCallHead = DrawCallLast = drawCall;
			return;
		}
		DrawCallLast->Next_ = drawCall;
		DrawCallLast = drawCall;
		bIsConsolidated = false;
	}

	void RenderPass::Clear(RenderContext* context)
	{
		mSamples.clear();
		mInputPasses.clear();
		
		mOutputRTs.clear();
		mRenderTargets.clear();

		mDepthStencilTarget = nullptr;
		mDepthStencil = RenderGraphBuilder::INVALID_RT;

		while (DrawCallHead)
		{
			DrawCall* dp = DrawCallHead;
			DrawCallHead = DrawCallHead->Next_;
			context->RecycleDrawCall(dp);
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
			auto batch = dp->ShadingState_->GetBatch();
			// Bind Global Constant
			dp->ShadingState_->GetBatch()->SetConstantBuffer(EShaderConstant::Global, Pipeline->GetContext()->GetParameters());

			// Bind Texture(RT Type)
			for (auto& pair : mBatch.RenderTargetMaps)
			{
				uint8 textureSlot;
				if (batch->GetTextureSlot(ETechnique::Shading, pair.first, textureSlot))
				{
					batch->SetTextureBinding(textureSlot, Pipeline->GetRenderTargetResource(pair.second));
				}
				
			}

			dp = dp->Next_;
		}

		// Consolidate RenderTarget
		mRenderTargets.resize(GetOutputCount(), nullptr);
		for (size_t i = 0; i < GetOutputCount(); ++i)
		{
			IRenderTarget* target = Pipeline->GetRenderTargetResource(GetOutput(static_cast<uint8>(i)));
			mRenderTargets[i] = target;
		}
		if (mDepthStencil != RenderGraphBuilder::INVALID_RT)
		{
			mDepthStencilTarget = Pipeline->GetRenderTargetResource(mDepthStencil);
		}
		bIsConsolidated = true;
	}

}