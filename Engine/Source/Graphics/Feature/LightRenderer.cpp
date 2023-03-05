#include "LightRenderer.h"

namespace Eggy
{
	RenderPass* DeferredLightingPass::Connect(RenderPass* input)
	{
		AddInput(input);
		return this;
	}

	void DeferredLightingPass::Compile(RenderGraphBuilder* builder)
	{
		RenderPass* scenePass = GetInput(0);

		Sample(builder, scenePass->GetOutput(0));
		Sample(builder, scenePass->GetOutput(1));
		Sample(builder, scenePass->GetOutput(2));
		Sample(builder, scenePass->GetOutput(3));
		Sample(builder, scenePass->GetOutput(4));

		mBatch.SetRenderTarget("BaseColorMap", scenePass->GetOutput(0));
		mBatch.SetRenderTarget("EmissiveMap", scenePass->GetOutput(1));
		mBatch.SetRenderTarget("NormalMap", scenePass->GetOutput(2));
		mBatch.SetRenderTarget("MiscMap", scenePass->GetOutput(3));
		mBatch.SetRenderTarget("DepthMap", scenePass->GetOutput(4));

		auto backBufferDesc = Pipeline->GetBackBuffer(builder);
		RenderTargetDesc rtDesc{
			EFormat::A32R32G32B32F,
			backBufferDesc.Width,
			backBufferDesc.Height,
		};
		SetOutput(builder, rtDesc);

	}

}

