#pragma once
#include "D3D11Common.h"
#include "Core/Config.h"
#include "Graphics/RHI/IRenderHeader.h"
#include "Graphics/RHI/IRenderResource.h"
#include "Graphics/RHI/IRenderDevice.h"


namespace Eggy
{
	class D3D11Device;
	struct D3D11Shader;

	struct D3D11Resource : public IDevcieResource
	{
		D3D11Resource() = default;
		virtual ~D3D11Resource() {}

		virtual void CreateResource(class D3D11Device& Device) {}
	};

	struct D3D11InputLayout : public D3D11Resource
	{
		D3D11InputLayout(IInputLayout* inputLayout): InputLayout(inputLayout)
		{}

		IInputLayout* InputLayout;
		TComPtr<ID3D11InputLayout> ppAddress;
	};

	struct D3D11Buffer : public D3D11Resource
	{
		D3D11Buffer(IBuffer* buffer): Buffer(buffer)
		{}

		IBuffer* Buffer;
		TComPtr<ID3D11Buffer> ppBuffer;
	};

	struct D3D11Texture : public D3D11Resource
	{
		D3D11Texture(ITextureBuffer* texture) : Texture(texture)
		{}

		ITextureBuffer* Texture;
		ETextureType TextureType;

		TComPtr<ID3D11ShaderResourceView> ppSRV;
	};

	struct D3D11Texture2D : public D3D11Texture
	{
		D3D11Texture2D(ITextureBuffer* texture) : D3D11Texture(texture)
		{
			TextureType = ETextureType::Texture2D;
		}

		TComPtr<ID3D11Texture2D> ppTex;
	};

	struct D3D11SamplerState : public D3D11Resource
	{
		D3D11SamplerState(SamplerState* state) : State(state)
		{}

		SamplerState* State;
		TComPtr<ID3D11SamplerState> ppSamplerState;
	};

	struct D3D11PipelineState : public D3D11Resource
	{
		D3D11PipelineState(PipelineState* state) : State(state)
		{}

		PipelineState* State;
		TComPtr<ID3D11RasterizerState> ppRasterizerState;
	};
	
	struct D3D11RenderTarget : public D3D11Resource
	{
		D3D11RenderTarget(IRenderTarget* rt) : RenderTarget(rt)
		{}
		IRenderTarget* RenderTarget;
		TComPtr<ID3D11Texture2D> ppTex;
		TComPtr<ID3D11RenderTargetView> ppRTV;
		TComPtr<ID3D11ShaderResourceView> ppSRV;
	};

	struct D3D11DepthStencil : public D3D11Resource
	{
		D3D11DepthStencil(IRenderTarget* rt) : RenderTarget(rt)
		{}
		IRenderTarget* RenderTarget;
		TComPtr<ID3D11Texture2D> ppTex;
		TComPtr<ID3D11DepthStencilView> ppDSV;
	};

	class D3D11ResourceFactory : public IRenderResourceFactory
	{
	public:
		D3D11ResourceFactory(class D3D11Device* device);

		void CreateInputLayout(struct IInputLayout* inputLayout, struct ShaderRenderResource* vsShader) override;
		void CreateShader(struct ShaderRenderResource* shader) override;
		void CreateBuffer(struct IBuffer* buffer) override;
		void CreateTexture(struct ITextureBuffer* texture) override;
		void CreateSamplerState(struct SamplerState* samplerState) override;
		void CreatePipelineState(struct PipelineState* pipelineState) override;
		void CreateRenderTarget(struct IRenderTarget* renderTarget) override;
		void CreateDepthStencil(struct IRenderTarget* renderTarget) override;
		void CreateBackBuffer(struct IRenderTarget* renderTarget, void* backBufferImage) override;

	protected:
		void CreateShaderFromFile(D3D11Shader* deviceShader);
		void CreateVertexShader(D3D11Shader* deviceShader);
		void CreatePixelShader(D3D11Shader* deviceShader);
		void CreateTexture2D(ITextureBuffer* texture);
		void CreateTextureCube(ITextureBuffer* texture);

	protected:
		class D3D11Device* mD3D11Device_;
	};
}