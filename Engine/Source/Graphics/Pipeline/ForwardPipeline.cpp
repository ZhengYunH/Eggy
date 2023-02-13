#include "ForwardPipeline.h"


namespace Eggy
{
	RenderPass* ForwardPipeline::Setup(RenderGraphBuilder* builder)
	{
		 RenderPass* output = scenePass.Connect(&denotator);
		 output = blurPostprocess.Connect(output);
		 output = textureToScreen.Connect(output);
		 output = imguiPass.Connect(output);
		 return output;
	}

	const RenderTargetDesc& ForwardPipeline::GetBackBuffer(RenderGraphBuilder* builder)
	{
		return denotator.GetOutputTarget(builder);
	}
}


