#pragma once
#include "IRenderResource.h"


namespace Eggy
{
	class RenderPass;
	
	// Render Graph Builder
	struct RenderTargetDesc
	{
		EFormat Format;
		uint16	Width;
		uint16  Height;
		bool IsBackBuffer{ false };
		bool IsDepthStencil{ false };
	};

	struct DepthStencilDesc
	{

	};


	class RenderGraphBuilder
	{
	public:
		static const size_t INVALID_RT = 0x7fff;

	public:
		RenderGraphBuilder();
		
		void Prepare();

		void ResolveConnection(RenderPass* outputPass, List<RenderPass*>& validPasses);
		size_t NewRenderTarget(RenderPass* pass, const RenderTargetDesc& desc);
		void Append(RenderPass* src) {}
		const RenderTargetDesc& GetOutputTarget(size_t index);
		List<IRenderTarget*>& GetRenderTargetResource() { return mRenderTargetResource; }
		void Resolve();
		void Clear();
		void GetBackBuffer(size_t& backBuffer, size_t& backDS);
		bool CheckRenderTargetValidation(size_t index);

	protected:
		List<RenderTargetDesc> mRenderTargetDescs;
		List<IRenderTarget*> mRenderTargetResource;

	private:
		uint32 mCurrentFrame_{ 0 };
		uint32 mBackBufferCount_{ 0 };
	};
}

