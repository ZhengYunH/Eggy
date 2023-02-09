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


		virtual void Compile() {}
		void AddInput(RenderPass* pass) { mInputPasses.push_back(pass); }
		List<RenderPass*> GetInput() { return mInputPasses; }

		void AddDrawCall(DrawCall* drawCall);
		void Clear();
		virtual void Consolidate();

		DrawCall* GetDrawCallHead() { return DrawCallHead; }
		void SetPipeline(RenderPipeline* pipeline) { Pipeline = pipeline; }

	protected:
		RenderPipeline* Pipeline{ nullptr };
		DrawCall* DrawCallHead{ nullptr };
		DrawCall* DrawCallLast{ nullptr };
		bool bIsConsolidated{ false };
		List<RenderPass*> mInputPasses;
	};
}

