#pragma once
#include "Graphics/RHI/IRenderPass.h"

namespace Eggy
{
	class Denotator : public RenderPass
	{
	public:
		Denotator() = default;
		void Compile(RenderGraphBuilder* builder) override;
	};

	class ForwardScenePass : public RenderPass
	{
	public:
		ForwardScenePass() = default;
		RenderPass* Connect(RenderPass* input);
		void Compile(RenderGraphBuilder* builder) override;
	};

	class DeferredScenePass : public RenderPass
	{
	public:
		DeferredScenePass() = default;
		RenderPass* Connect(RenderPass* input);
		void Compile(RenderGraphBuilder* builder) override;
	};
}