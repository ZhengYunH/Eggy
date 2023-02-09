#include "ForwardPipeline.h"

namespace Eggy
{
	RenderPass* ScenePass::Connect(RenderPass* input)
	{
		AddInput(input);
		return this;
	}

	void ScenePass::Compile()
	{
		Pipeline->AddDrawCallChannel(ERenderSet::MAIN, this);
	}

	void Postprocess::Consolidate()
	{
		/*DrawCall* dp = new DrawCall();
		AddDrawCall(dp);*/
		RenderPass::Consolidate();
	}

	RenderPass* BlurPostprocess::Connect(RenderPass* input)
	{
		AddInput(input);
		return this;
	}

	RenderPass* ForwardPipeline::Setup()
	{
		 RenderPass* output = scenePass.Connect(&denotator);
		 output = blurPostprocess.Connect(output);
		 return output;
	}

	void ForwardPipeline::ResolveConnection(RenderPass* output)
	{
		// DFS
		for (RenderPass* inputPass : output->GetInput())
		{
			ResolveConnection(inputPass);
		}
		AddRenderPass(output);
	}
}

