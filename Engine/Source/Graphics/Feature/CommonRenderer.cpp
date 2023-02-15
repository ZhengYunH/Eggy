#include "CommonRenderer.h"

namespace Eggy
{
	RenderPass* ForwardScenePass::Connect(RenderPass* input)
	{
		AddInput(input);
		return this;
	}

	void ForwardScenePass::Compile(RenderGraphBuilder* builder)
	{
		Pipeline->AddDrawCallChannel(ERenderSet::Main, this);
		
		RenderPass* inputPass = GetInput(0);
		SetOutput(builder, inputPass->GetOutput(0));
		SetDepthStencil(builder, inputPass->GetDepthStencil());
	}

	void Denotator::Compile(RenderGraphBuilder* builder)
	{
		size_t backBuffer = RenderGraphBuilder::INVALID_RT;
		size_t dsBuffer = RenderGraphBuilder::INVALID_RT;
		builder->GetBackBuffer(backBuffer, dsBuffer);
		SetOutput(builder, builder->GetOutputTarget(backBuffer));
		SetDepthStencil(builder, builder->GetOutputTarget(dsBuffer));
	}

}

