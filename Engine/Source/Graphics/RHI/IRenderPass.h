#pragma once
#include "IRenderPipeline.h"


namespace Eggy
{
	struct DrawCall;
	class RenderPipeline;

	class RenderPass
	{
	public:
		RenderPass(RenderPipeline* pipeline)
			: Pipeline(pipeline)
		{

		}

		void AddDrawCall(DrawCall* drawCall);
		void Clear();
		void Consolidate();

		DrawCall* GetDrawCallHead() { return DrawCallHead; }

	protected:
		RenderPipeline* Pipeline{ nullptr };
		DrawCall* DrawCallHead{ nullptr };
		DrawCall* DrawCallLast{ nullptr };
		bool bIsConsolidated{ false };
	};
}

