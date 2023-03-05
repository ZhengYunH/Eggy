#include "DeferredPipeline.h"


namespace Eggy
{
	RenderPass* DeferredPipeline::Setup(RenderGraphBuilder* builder)
	{
		RenderPass* output = scenePass.Connect(&denotator);
		output = lightingPass.Connect(output);
		output = blurPostprocess.Connect(output);
		output = textureToScreen.Connect(output);
		output = imguiPass.Connect(output);
		return output;
	}

	const RenderTargetDesc& DeferredPipeline::GetBackBuffer(RenderGraphBuilder* builder)
	{
		return denotator.GetOutputTarget(builder);
	}
}