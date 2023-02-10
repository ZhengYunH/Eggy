#pragma once
#include "IRenderPipeline.h"


namespace Eggy
{
	struct DrawCall;
	class RenderPipeline;

	class RenderPass
	{
	public:
		RenderPass() = default;

		RenderPass(RenderPipeline* pipeline)
			: Pipeline(pipeline)
		{

		}

		virtual void Compile(RenderGraphBuilder* builder) {}
		virtual void Consolidate();

		void AddInput(RenderPass* pass) { mInputPasses.push_back(pass); }
		List<RenderPass*> GetInput() { return mInputPasses; }
		
		size_t GetOutput();
		void SetOutput(RenderGraphBuilder* builder, RenderTargetDesc desc, uint8 rtIndex=0);

		void AddDrawCall(DrawCall* drawCall);
		void Clear();

		DrawCall* GetDrawCallHead() { return DrawCallHead; }
		void SetPipeline(RenderPipeline* pipeline) { Pipeline = pipeline; }

		virtual void EncodeRenderPassEnd() {}

	protected:
		RenderPipeline* Pipeline{ nullptr };
		DrawCall* DrawCallHead{ nullptr };
		DrawCall* DrawCallLast{ nullptr };
		bool bIsConsolidated{ false };
		List<RenderPass*> mInputPasses;
		List<size_t> mOutputRTs;
	};
}

