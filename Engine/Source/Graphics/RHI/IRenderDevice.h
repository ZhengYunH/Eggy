#pragma once
#include "Core/Config.h"


namespace Eggy
{
	struct IInputLayout;
	struct IShaderCollection;
	struct IShader;
	struct IBuffer;

	class IRenderResourceFactory
	{
	public:
		virtual void CreateInputLayout(IInputLayout* inputLayout, IShaderCollection* shaderCollection) = 0;
		virtual void CreateShader(IShader* shader) = 0;
		virtual void CreateBuffer(IBuffer* buffer) = 0;
	};

	class IRenderDevice
	{
	public:
		IRenderDevice() = default;

	public:
		virtual void PrepareResource() = 0;
		virtual void DrawFrame() = 0;
		virtual IRenderResourceFactory* GetResourceFactory() = 0;

	protected:
		virtual void CreateSwapChain() {};
		virtual void OnResize() {};
	};
	extern void CreateRenderDevice();
	extern IRenderDevice* GRenderDevice;
}
