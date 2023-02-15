#include "RenderGraph.h"
#include "IRenderPass.h"
#include "Core/System/RenderSystem.h"


namespace Eggy
{

	RenderGraphBuilder::RenderGraphBuilder()
	{
		mBackBufferCount_ = RenderSystem::Get()->GetBackBufferCount();
	}

	void RenderGraphBuilder::Prepare()
	{
		RenderTargetDesc backBufferDesc;
		backBufferDesc.Format = RenderSystem::Get()->ScreenFormat;
		backBufferDesc.Height = RenderSystem::Get()->ScreenHeight; 
		backBufferDesc.Width = RenderSystem::Get()->ScreenWidth;
		backBufferDesc.IsBackBuffer = true;
		mRenderTargetDescs.push_back(std::move(backBufferDesc));

		RenderTargetDesc dsBufferDesc;
		dsBufferDesc.Format = RenderSystem::Get()->ScreenDSFormat;
		dsBufferDesc.Height = RenderSystem::Get()->ScreenHeight;
		dsBufferDesc.Width = RenderSystem::Get()->ScreenWidth; 
		dsBufferDesc.IsBackBuffer = true;
		dsBufferDesc.IsDepthStencil = true;
		mRenderTargetDescs.push_back(std::move(dsBufferDesc));
	}

	void RenderGraphBuilder::ResolveConnection(RenderPass* outputPass, List<RenderPass*>& validPasses)
	{
		// DFS
		for (RenderPass* inputPass : outputPass->GetInput())
		{
			ResolveConnection(inputPass, validPasses);
		}
		validPasses.push_back(outputPass);
	}

	size_t RenderGraphBuilder::NewRenderTarget(RenderPass* pass, const RenderTargetDesc& desc)
	{
		size_t rtIndex = mRenderTargetDescs.size();
		mRenderTargetDescs.push_back(desc);
		return rtIndex;
	}

	const RenderTargetDesc& RenderGraphBuilder::GetOutputTarget(size_t index)
	{
		HYBRID_CHECK(index < mRenderTargetDescs.size());
		return mRenderTargetDescs[index];
	}

	void RenderGraphBuilder::Resolve()
	{
		mRenderTargetResource.resize(mRenderTargetDescs.size());
		// always remain index0 for back-buffer, index1 for depthStencil-buffer
		mRenderTargetResource[0] = RenderSystem::Get()->GetBackBuffer(mCurrentFrame_);
		mRenderTargetResource[1] = RenderSystem::Get()->GetDepthStencilBuffer(mCurrentFrame_);
		for (size_t i = 2; i < mRenderTargetDescs.size(); ++i)
		{
			mRenderTargetResource[i] = new IRenderTarget();
			auto& desc = mRenderTargetDescs[i];
			IRenderTarget* rt = mRenderTargetResource[i];
			rt->Format = desc.Format;
			rt->Width = desc.Width;
			rt->Height = desc.Height;
			rt->Quality = ESamplerQuality::DEFAULT;
			rt->BindType = desc.IsDepthStencil ? EBufferTypes(EBufferType::DepthStencil) : EBufferTypes(EBufferType::RenderTarget);
			// TODO
			bool beSampled = true;
			if (beSampled && !desc.IsDepthStencil)
			{
				rt->BindType |= EBufferTypes(EBufferType::ShaderResource);
			}
		}
	}

	void RenderGraphBuilder::Clear()
	{
		mRenderTargetDescs.clear();
		for (size_t i = 2; i < mRenderTargetResource.size(); ++i)
		{
			SafeDestroy(mRenderTargetResource[i]);
		}
		mRenderTargetResource.clear();

		mCurrentFrame_ = (mCurrentFrame_ + 1) % mBackBufferCount_;
	}

	void RenderGraphBuilder::GetBackBuffer(size_t& backBuffer, size_t& backDS)
	{
		backBuffer = 0;
		backDS = 1;
	}

	bool RenderGraphBuilder::CheckRenderTargetValidation(size_t index)
	{
		return index < mRenderTargetDescs.size();
	}

}

