#pragma once
#include "Graphics/RHI/IRenderPipeline.h"
#include "Graphics/RHI/IRenderPass.h"

#include "Graphics/Feature/CommonRenderer.h"
#include "Graphics/Feature/ImGuiRenderer.h"
#include "Graphics/Feature/LightRenderer.h"
#include "Graphics/Feature/PostProcess/PostProcessRenderer.h"


namespace Eggy
{
	class DeferredPipeline : public RenderPipeline
	{
	public:
		RenderPass* Setup(RenderGraphBuilder* builder) override;
		const RenderTargetDesc& GetBackBuffer(RenderGraphBuilder* builder) override;

	protected:
		Denotator denotator;
		DeferredScenePass scenePass;
		DeferredLightingPass lightingPass;

		BlurPostprocess blurPostprocess;

		ImguiPass imguiPass;
		TextureToScreen textureToScreen;
	};
}