#pragma once
#include "Core/Config.h"


namespace Eggy
{
	class IRenderResourceFactory
	{
	public:
		virtual void CreateInputLayout(struct IInputLayout* inputLayout, struct ShaderRenderResource* vsShader) = 0;
		virtual void CreateShader(struct ShaderRenderResource* shader) = 0;
		virtual void CreateBuffer(struct IBuffer* buffer) = 0;
		virtual void CreateTexture(struct ITextureBuffer* texture) = 0;
		virtual void CreateSamplerState(struct SamplerState* samplerState) = 0;
		virtual void CreatePipelineState(struct PipelineState* pipelineState) = 0;
		virtual void CreateRenderTarget(struct IRenderTarget* renderTarget) = 0;
		virtual void CreateBackBuffer(struct IRenderTarget* renderTarget, void* backBufferImage) = 0;
		virtual void CreateDepthStencil(struct IRenderTarget* renderTarget) = 0;
	};

	class IRenderDevice
	{
	public:
		IRenderDevice() = default;

	public:
		virtual void PrepareResource() = 0;
		virtual void DrawFrame() = 0;
		virtual IRenderResourceFactory* GetResourceFactory() = 0;
		virtual void* GetDevice() = 0;
		virtual void* GetContext() = 0;
		virtual IRenderTarget* GetBackBuffer() = 0;

	protected:
		virtual void CreateSwapChain() {};
		virtual void OnResize() {};
	};
	extern void CreateRenderDevice();
	extern IRenderDevice* GRenderDevice;

	class IDevcieResource
	{
	public:
		IDevcieResource() = default;
		virtual ~IDevcieResource() {}
	};
}
