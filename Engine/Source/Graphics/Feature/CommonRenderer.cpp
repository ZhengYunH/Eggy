#include "CommonRenderer.h"

namespace Eggy
{
	void Denotator::Compile(RenderGraphBuilder* builder)
	{
		size_t backBuffer = RenderGraphBuilder::INVALID_RT;
		size_t dsBuffer = RenderGraphBuilder::INVALID_RT;
		builder->GetBackBuffer(backBuffer, dsBuffer);
		SetOutput(builder, builder->GetOutputTarget(backBuffer));
		SetDepthStencil(builder, builder->GetOutputTarget(dsBuffer));
	}

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

	RenderPass* DeferredScenePass::Connect(RenderPass* input)
	{
		AddInput(input);
		return this;
	}

	void DeferredScenePass::Compile(RenderGraphBuilder* builder)
	{
		Pipeline->AddDrawCallChannel(ERenderSet::Main, this);
		auto backBufferDesc = Pipeline->GetBackBuffer(builder);
		RenderTargetDesc GBufferDescHigh{
			EFormat::A32R32G32B32F,
			backBufferDesc.Width,
			backBufferDesc.Height,
		};

		RenderTargetDesc GBufferDescLow{
			EFormat::R8G8B8A8,
			backBufferDesc.Width,
			backBufferDesc.Height,
		};

		RenderTargetDesc GBufferDepth{
			EFormat::D32_SFLOAT_S8X24_UINT,
			backBufferDesc.Width,
			backBufferDesc.Height,
		};
		
		SetOutput(builder, GBufferDescHigh, 0); // GBufferA: BaseColor, Shading Model
		SetOutput(builder, GBufferDescHigh, 1); // GBufferB: Emissive
		SetOutput(builder, GBufferDescLow, 2); // GBufferC: normal.xyz, sheen
		SetOutput(builder, GBufferDescLow, 3); // GBufferD: metallic, roughness, anisotropic, clearCoat 
		SetOutput(builder, GBufferDepth, 4); // GBufferE: depth
	}

}

