#pragma once
#include "IRenderPipeline.h"


namespace Eggy
{
	struct DrawCall;
	class RenderPipeline;

	// TODO Support all intrinsic type
	struct RenderPassBatch
	{
		void SetRenderTarget(const String& Name, const size_t& rtIndex)
		{
			RenderTargetMaps[Name] = rtIndex;
		}

		void SetFloat(const String& Name, const float& v)
		{
			ConstantMaps[Name] = v;
		}

		void SetInt(const String& Name, const int& v);
		void SetBoolean(const String& Name, const bool& v);

		void Clear()
		{
			RenderTargetMaps.clear();
			ConstantMaps.clear();
		}

		Map<String, float> ConstantMaps;
		Map<String, size_t> RenderTargetMaps;
	};

	class RenderPass
	{
	public:
		RenderPass() = default;

		RenderPass(RenderPipeline* pipeline)
			: Pipeline(pipeline)
		{

		}

		virtual void Compile(RenderGraphBuilder* builder) {}
		virtual void Render(RenderContext* context) {}
		virtual void Consolidate();

		void AddInput(RenderPass* pass) { mInputPasses.push_back(pass); }
		List<RenderPass*> GetInput() { return mInputPasses; }
		RenderPass* GetInput(size_t index) 
		{ 
			return index < mInputPasses.size() ? mInputPasses[index] : nullptr;
		}
		
		size_t GetOutput();
		size_t GetOutput(uint8 index);
		const size_t GetOutputCount();
		size_t GetDepthStencil() { return mDepthStencil; }

		const RenderTargetDesc& GetOutputTarget(RenderGraphBuilder* builder);
		void SetOutput(RenderGraphBuilder* builder, const RenderTargetDesc& desc, uint8 rtIndex=0);
		void SetOutput(RenderGraphBuilder* builder, size_t targetOuput, uint8 rtIndex = 0);

		void SetDepthStencil(RenderGraphBuilder* builder, const RenderTargetDesc& desc);
		void SetDepthStencil(RenderGraphBuilder* builder, size_t targetOuput);

		void Sample(RenderGraphBuilder* builder, size_t rtIndex);
		void Sample(RenderGraphBuilder* builder, ITextureBuffer* texture);

		void AddDrawCall(DrawCall* drawCall);
		void Clear(RenderContext* context);

		DrawCall* GetDrawCallHead() { return DrawCallHead; }
		void SetPipeline(RenderPipeline* pipeline) { Pipeline = pipeline; }
		const RenderPipeline* const GetPipeline() { return Pipeline; }

		virtual void EncodeRenderPassEnd() {}

		List<IRenderTarget*>& GetRenderTargetResources() { return mRenderTargets; }
		IRenderTarget* GetDepthStencilResource() { return mDepthStencilTarget; }

	protected:
		RenderPipeline* Pipeline{ nullptr };
		DrawCall* DrawCallHead{ nullptr };
		DrawCall* DrawCallLast{ nullptr };
		bool bIsConsolidated{ false };
		
		List<RenderPass*> mInputPasses;
		List<size_t> mOutputRTs;
		size_t mDepthStencil{ RenderGraphBuilder::INVALID_RT };

		List<size_t> mSamples;
		RenderPassBatch mBatch;

		List<IRenderTarget*> mRenderTargets;
		IRenderTarget* mDepthStencilTarget{ nullptr };

	};
}

