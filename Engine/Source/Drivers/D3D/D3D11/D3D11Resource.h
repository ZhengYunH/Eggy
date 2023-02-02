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
	struct IShaderCollection;

	struct D3D11Resource
	{
		virtual void CreateResource(class D3D11Device& Device) {};
	};

	struct D3D11InputLayout 
	{
		D3D11InputLayout(IInputLayout* inputLayout): InputLayout(inputLayout)
		{}

		IInputLayout* InputLayout;
		TComPtr<ID3D11InputLayout> ppAddress;
	};

	struct D3D11Buffer
	{
		D3D11Buffer(IBuffer* buffer): Buffer(buffer)
		{}

		IBuffer* Buffer;
		TComPtr<ID3D11Buffer> ppBuffer;
	};

	struct D3D11Texture
	{
		D3D11Texture(ITexture* texture) : Texture(texture)
		{}

		ITexture* Texture;
		ETextureType TextureType;

		TComPtr<ID3D11ShaderResourceView> ppSRV;
	};

	struct D3D11Texture2D : public D3D11Texture
	{
		D3D11Texture2D(ITexture* texture) : D3D11Texture(texture)
		{
			TextureType = ETextureType::Texture2D;
		}

		TComPtr<ID3D11Texture2D> ppTex;
	};

	struct D3D11SamplerState
	{
		D3D11SamplerState(SamplerState* state) : State(state)
		{}

		SamplerState* State;
		TComPtr<ID3D11SamplerState> ppSamplerState;
	};

	struct D3D11PipelineState
	{

	};

	class D3D11ResourceFactory : public IRenderResourceFactory
	{
	public:
		D3D11ResourceFactory(class D3D11Device* device) : mD3D11Device_(device)
		{
		}

		void CreateInputLayout(IInputLayout* inputLayout, IShaderCollection* shaderCollection) override;
		void CreateShader(IShader* shader) override;
		void CreateBuffer(IBuffer* buffer) override;
		void CreateTexture(ITexture* texture) override;
		void CreateSamplerState(SamplerState* state) override;

	protected:
		void CreateShaderFromFile(D3D11Shader* deviceShader);
		void CreateVertexShader(D3D11Shader* deviceShader);
		void CreatePixelShader(D3D11Shader* deviceShader);
		
		void CreateTexture2D(ITexture* texture);
		void CreateTextureCube(ITexture* texture) { Unimplement(); }


	protected:
		class D3D11Device* mD3D11Device_;
	};
}