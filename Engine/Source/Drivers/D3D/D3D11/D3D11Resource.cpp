#include "D3D11Resource.h"
#include "D3D11Device.h"
#include "D3D11Shader.h"
#include "Graphics/RHI/IShaderRenderResource.h"
#include <d3dcompiler.h>


namespace Eggy
{
	D3D11ResourceFactory::D3D11ResourceFactory(class D3D11Device* device) : mD3D11Device_(device)
	{

	}

	void D3D11ResourceFactory::CreateInputLayout(struct IInputLayout* inputLayout, struct ShaderRenderResource* vsShader)
	{
		SafeDestroy(inputLayout->DeviceResource);
		D3D11InputLayout* deviceInputLayout = new D3D11InputLayout(inputLayout);
		inputLayout->DeviceResource = deviceInputLayout;

		UINT numElements = static_cast<UINT>(inputLayout->Descs.size());
		D3D11_INPUT_ELEMENT_DESC* layoutDescs = new D3D11_INPUT_ELEMENT_DESC[numElements];
		for (UINT i = 0; i < numElements; ++i)
		{
			auto& desc = inputLayout->Descs[i];
			layoutDescs[i] = {
				desc.SemanticName.c_str(),
				desc.SemanticIndex,
				Converter::Format(desc.Format),
				desc.InputSlot,
				desc.AlignedByteOffset,
				Converter::InputClassification(desc.SlotClass)
			};
		}
		D3D11VertexShader* vertexShader = (D3D11VertexShader*)vsShader->DeviceResource;
		void* pShaderBytecodeWithInputSignature = vertexShader->ppBlob->GetBufferPointer();
		SIZE_T bytecodeLength = vertexShader->ppBlob->GetBufferSize();
		HR(mD3D11Device_->mDevice_->CreateInputLayout(
			layoutDescs, numElements,
			pShaderBytecodeWithInputSignature, bytecodeLength,
			deviceInputLayout->ppAddress.GetAddressOf()
		));
		SafeDestroyArray(layoutDescs);
	}


	void D3D11ResourceFactory::CreateShader(ShaderRenderResource* shader)
	{
		using enum EShaderStage;
		SafeDestroy(shader->DeviceResource);

		D3D11Shader* deviceShader = nullptr;
		switch (shader->Stage)
		{
		case UNDEFINE:
			Unimplement();
			break;
		case VS:
			deviceShader = new D3D11VertexShader(shader);
			break;
		case PS:
			deviceShader = new D3D11PixelShader(shader);
			break;
		default:
			Unimplement();
			break;
		}

		HYBRID_CHECK(deviceShader);
		shader->DeviceResource = deviceShader;
		CreateShaderFromFile(deviceShader);

		switch (shader->Stage)
		{
		case UNDEFINE:
			Unimplement();
			break;
		case VS:
			CreateVertexShader(deviceShader);
			break;
		case PS:
			CreatePixelShader(deviceShader);
			break;
		default:
			Unimplement();
			break;
		}
	}

	void D3D11ResourceFactory::CreateShaderFromFile(D3D11Shader* deviceShader)
	{
		HRESULT hr = S_OK;
		if (!deviceShader->CSOPath.empty() && D3DReadFileToBlob(Tool::stringToLPCWSTR(deviceShader->CSOPath), deviceShader->ppBlob.GetAddressOf()) == S_OK)
		{
		}
		else
		{
			DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if DEBUG_MODE
			dwShaderFlags |= D3DCOMPILE_DEBUG;
			dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
			ID3DBlob* errorBlob = nullptr;
			const D3D_SHADER_MACRO* macros = nullptr;
			hr = D3DCompileFromFile(
				Tool::stringToLPCWSTR(deviceShader->HLSLPath), macros, D3D_COMPILE_STANDARD_FILE_INCLUDE,
				deviceShader->EntryPoint.c_str(), deviceShader->ShaderModel.c_str(),
				dwShaderFlags, 0, deviceShader->ppBlob.GetAddressOf(), &errorBlob
			);

			if (FAILED(hr))
			{
				if (errorBlob != nullptr)
				{
					OutputDebugStringA(reinterpret_cast<const char*>(errorBlob->GetBufferPointer()));
					errorBlob->Release();
					errorBlob = nullptr;
				}
				HYBRID_CHECK(0);
			}

			if (!deviceShader->CSOPath.empty())
			{
				HR(D3DWriteBlobToFile(deviceShader->ppBlob.Get(), Tool::stringToLPCWSTR(deviceShader->CSOPath), FALSE));
			}
		}
	}

	void D3D11ResourceFactory::CreateVertexShader(D3D11Shader* deviceShader)
	{
		D3D11VertexShader* vertexShader = dynamic_cast<D3D11VertexShader*>(deviceShader);
		HR(mD3D11Device_->mDevice_->CreateVertexShader(deviceShader->ppBlob->GetBufferPointer(), deviceShader->ppBlob->GetBufferSize(), nullptr, vertexShader->ppShader.GetAddressOf()));
	}

	void D3D11ResourceFactory::CreatePixelShader(D3D11Shader* deviceShader)
	{
		D3D11PixelShader* pixelShader = dynamic_cast<D3D11PixelShader*>(deviceShader);
		HR(mD3D11Device_->mDevice_->CreatePixelShader(deviceShader->ppBlob->GetBufferPointer(), deviceShader->ppBlob->GetBufferSize(), nullptr, pixelShader->ppShader.GetAddressOf()));
	}

	void D3D11ResourceFactory::CreateBuffer(IBuffer* buffer)
	{
		SafeDestroy(buffer->DeviceResource);
		D3D11Buffer* deviceBuffer = new D3D11Buffer(buffer);
		buffer->DeviceResource = deviceBuffer;
		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		bufferDesc.Usage = Converter::Usage(buffer->Usage);
		bufferDesc.ByteWidth = static_cast<UINT>(buffer->ByteWidth);
		bufferDesc.BindFlags = Converter::BufferType(buffer->BindType);
		bufferDesc.CPUAccessFlags = Converter::CPUAccessFlag(buffer->CPUAcesssFlags);

		D3D11_SUBRESOURCE_DATA initData;
		ZeroMemory(&initData, sizeof(initData));
		initData.pSysMem = buffer->Data;
		
		HRESULT hr = mD3D11Device_->mDevice_->CreateBuffer(
			&bufferDesc,
			buffer->Data ? &initData : nullptr,
			deviceBuffer->ppBuffer.GetAddressOf()
		);

		HR(hr
		);
	}

	void D3D11ResourceFactory::CreateTexture(ITextureBuffer* texture)
	{
		switch (texture->TextureType)
		{
		case ETextureType::Texture2D:
			CreateTexture2D(texture);
			break;
		case ETextureType::TextureCube:
			CreateTextureCube(texture);
			break;
		default:
			Unimplement();
		}
	}

	void D3D11ResourceFactory::CreateSamplerState(struct SamplerState* samplerState)
	{
		SafeDestroy(samplerState->DeviceResource);
		D3D11SamplerState* deviceSamplerState = new D3D11SamplerState(samplerState);
		samplerState->DeviceResource = deviceSamplerState;

		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory(&sampDesc, sizeof(sampDesc));
		sampDesc.Filter = Converter::FilterType(samplerState->Filter);
		sampDesc.AddressU = Converter::AddressMode(samplerState->AddressU);
		sampDesc.AddressV = Converter::AddressMode(samplerState->AddressV);
		sampDesc.AddressW = Converter::AddressMode(samplerState->AddressW);
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = samplerState->MinLod >= 0 ? samplerState->MinLod : 0;
		sampDesc.MaxLOD = samplerState->MaxLod >= 0 ? samplerState->MaxLod : D3D11_FLOAT32_MAX;
		HR(mD3D11Device_->mDevice_->CreateSamplerState(&sampDesc, deviceSamplerState->ppSamplerState.GetAddressOf()));
	}

	void D3D11ResourceFactory::CreatePipelineState(PipelineState* pipelineState)
	{
		SafeDestroy(pipelineState->DeviceResource);
		D3D11PipelineState* devicePipelineState = new D3D11PipelineState(pipelineState);
		pipelineState->DeviceResource = devicePipelineState;

		{
			D3D11_RASTERIZER_DESC desc = {};
			auto& renderState = pipelineState->State;
			desc.FillMode = Converter::FillMode(renderState.FillMode);
			desc.CullMode = Converter::CullMode(renderState.CullMode);
			desc.FrontCounterClockwise = false;
			desc.DepthBias = 0;
			desc.SlopeScaledDepthBias = 0.0f;
			desc.DepthBiasClamp = 0.0f;
			desc.DepthClipEnable = renderState.EnableDepthClip;
			desc.ScissorEnable = renderState.EnableScissor;
			desc.MultisampleEnable = renderState.EnableMultisample;
			desc.AntialiasedLineEnable = renderState.EnableAntialiasedLine;
			mD3D11Device_->mDevice_->CreateRasterizerState(&desc, devicePipelineState->ppRasterizerState.GetAddressOf());
		}
	}

	void D3D11ResourceFactory::CreateRenderTarget(struct IRenderTarget* renderTarget)
	{
		SafeDestroy(renderTarget->DeviceResource);
		D3D11RenderTarget* deviceRT = new D3D11RenderTarget(renderTarget);
		renderTarget->DeviceResource = deviceRT;

		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(desc));

		desc.ArraySize = 1;
		desc.BindFlags = Converter::BufferType(renderTarget->BindType);
		desc.CPUAccessFlags = Converter::CPUAccessFlag(renderTarget->CPUAcesssFlags);
		desc.Format = Converter::Format(renderTarget->Format);
		desc.Height = renderTarget->Height;
		desc.Width = renderTarget->Width;
		desc.MipLevels = 1;
		desc.MiscFlags = 0;
		desc.SampleDesc.Count = Converter::SamplerCount(renderTarget->Quality);
		UINT quality = UINT(Converter::SamplerQualityFactor(renderTarget->Quality) * mD3D11Device_->m4xMsaaQaulity_);
		desc.SampleDesc.Quality = quality > 0 ? quality - 1 : 0;
		desc.Usage = Converter::Usage(renderTarget->Usage);

		HRESULT hr = mD3D11Device_->mDevice_->CreateTexture2D(&desc, nullptr, deviceRT->ppTex.GetAddressOf());
		HR(hr);

		D3D11_RENDER_TARGET_VIEW_DESC RTVDesc;
		ZeroMemory(&RTVDesc, sizeof(RTVDesc));
		RTVDesc.Format = desc.Format;
		RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		HR(mD3D11Device_->mDevice_->CreateRenderTargetView(deviceRT->ppTex.Get(), &RTVDesc, deviceRT->ppRTV.GetAddressOf()));
		
		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
		SRVDesc.Format = desc.Format;
		SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		SRVDesc.Texture2D.MipLevels = desc.MipLevels;
		HR(mD3D11Device_->mDevice_->CreateShaderResourceView(deviceRT->ppTex.Get(), &SRVDesc, deviceRT->ppSRV.GetAddressOf()));
	}

	void D3D11ResourceFactory::CreateDepthStencil(struct IRenderTarget* renderTarget)
	{
		SafeDestroy(renderTarget->DeviceResource);
		D3D11DepthStencil* deviceRT = new D3D11DepthStencil(renderTarget);
		renderTarget->DeviceResource = deviceRT;

		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(desc));

		desc.ArraySize = 1;
		desc.BindFlags = Converter::BufferType(renderTarget->BindType);
		desc.CPUAccessFlags = Converter::CPUAccessFlag(renderTarget->CPUAcesssFlags);
		desc.Format = Converter::Format(renderTarget->Format);
		desc.Height = renderTarget->Height;
		desc.Width = renderTarget->Width;
		desc.MipLevels = 1;
		desc.MiscFlags = 0;
		desc.SampleDesc.Count = Converter::SamplerCount(renderTarget->Quality);
		UINT quality = UINT(Converter::SamplerQualityFactor(renderTarget->Quality) * mD3D11Device_->m4xMsaaQaulity_);
		desc.SampleDesc.Quality = quality > 0 ? quality - 1 : 0;
		desc.Usage = Converter::Usage(renderTarget->Usage);

		D3D11_SUBRESOURCE_DATA initData = {
			renderTarget->Data,
			renderTarget->Width * GetFormatInfo(renderTarget->Format).DataSize,
			renderTarget->Width * renderTarget->Height * GetFormatInfo(renderTarget->Format).DataSize
		};

		HR(mD3D11Device_->mDevice_->CreateTexture2D(&desc, nullptr, deviceRT->ppTex.GetAddressOf()));
		D3D11_DEPTH_STENCIL_VIEW_DESC DSVDesc;
		ZeroMemory(&DSVDesc, sizeof(DSVDesc));
		DSVDesc.Format = desc.Format;
		DSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		HR(mD3D11Device_->mDevice_->CreateDepthStencilView(deviceRT->ppTex.Get(), &DSVDesc, deviceRT->ppDSV.GetAddressOf()));
	}

	void D3D11ResourceFactory::CreateBackBuffer(struct IRenderTarget* renderTarget, void* backBufferImage)
	{
		SafeDestroy(renderTarget->DeviceResource);
		D3D11RenderTarget* deviceRT = new D3D11RenderTarget(renderTarget);
		renderTarget->DeviceResource = deviceRT;
		HR(mD3D11Device_->mDevice_->CreateRenderTargetView((ID3D11Texture2D*)backBufferImage, nullptr, deviceRT->ppRTV.GetAddressOf()));
	}

	void D3D11ResourceFactory::CreateTexture2D(ITextureBuffer* texture)
	{
		SafeDestroy(texture->DeviceResource);
		D3D11Texture2D* deviceTexture = new D3D11Texture2D(texture);
		texture->DeviceResource = deviceTexture;

		D3D11_TEXTURE2D_DESC desc = {};
		desc.Width = texture->Width;
		desc.Height = texture->Height;
		desc.MipLevels = texture->Mips;
		desc.ArraySize = 1;
		desc.Format = Converter::Format(texture->Format);
		desc.SampleDesc.Count = 1;
		desc.Usage = Converter::Usage(texture->Usage);;
		desc.BindFlags = Converter::BufferType(texture->BindType);

		D3D11_SUBRESOURCE_DATA initData = { texture->Data, texture->Width * GetFormatInfo(texture->Format).DataSize, texture->Width * texture->Height * GetFormatInfo(texture->Format).DataSize};
		HRESULT hr = mD3D11Device_->mDevice_->CreateTexture2D(&desc, &initData, deviceTexture->ppTex.GetAddressOf());
		HR(hr);

		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
		SRVDesc.Format = Converter::Format(texture->Format);
		SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		SRVDesc.Texture2D.MipLevels = texture->Mips;
		HR(mD3D11Device_->mDevice_->CreateShaderResourceView(deviceTexture->ppTex.Get(), &SRVDesc, deviceTexture->ppSRV.GetAddressOf()));
	}

	void D3D11ResourceFactory::CreateTextureCube(ITextureBuffer* deviceShader)
	{
		Unimplement(0);
	}

}

