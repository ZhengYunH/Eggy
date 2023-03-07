#pragma once
#include "Graphics/Feature/PostProcess/PostProcessRenderer.h"


namespace Eggy
{
	class DeferredLightingPass : public Postprocess
	{
	public:
		DeferredLightingPass() : Postprocess(StaticName(DeferredLighting)) {}

		RenderPass* Connect(RenderPass* input);
		void Compile(RenderGraphBuilder* builder) override;
	};
}