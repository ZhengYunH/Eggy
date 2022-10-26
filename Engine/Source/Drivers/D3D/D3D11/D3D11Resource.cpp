#include "D3D11Resource.h"
#include "D3D11Device.h"
#include "D3D11Shader.h"
#include "Graphics/RHI/IShader.h"
#include <d3dcompiler.h>


namespace Eggy
{
	void D3D11ResourceFactory::CreateInputLayout(IInputLayout* inputLayout, IShaderCollection* shaderCollection)
	{
		D3D11InputLayout* deviceInputLayout = new D3D11InputLayout(inputLayout);
		inputLayout->DeviceResource = (void*)deviceInputLayout;

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
		D3D11VertexShader* vertexShader = (D3D11VertexShader*)shaderCollection->GetShader(EShaderType::VS)->DeviceResource;
		void* pShaderBytecodeWithInputSignature = vertexShader->ppBlob->GetBufferPointer();
		SIZE_T bytecodeLength = vertexShader->ppBlob->GetBufferSize();
		HR(mD3D11Device_->mDevice_->CreateInputLayout(
			layoutDescs, numElements,
			pShaderBytecodeWithInputSignature, bytecodeLength,
			deviceInputLayout->ppAddress.GetAddressOf()
		));
	}


	void D3D11ResourceFactory::CreateShader(IShader* shader)
	{
		D3D11Shader* deviceShader = nullptr;
		switch (shader->Type)
		{
		case EShaderType::None:
			Unimplement();
			break;
		case EShaderType::VS:
			deviceShader = new D3D11VertexShader(shader);
			break;
		case EShaderType::PS:
			deviceShader = new D3D11PixelShader(shader);
			break;
		default:
			Unimplement();
			break;
		}

		HYBRID_CHECK(deviceShader);
		shader->DeviceResource = (void*)deviceShader;
		CreateShaderFromFile(deviceShader);

		switch (shader->Type)
		{
		case EShaderType::None:
			Unimplement();
			break;
		case EShaderType::VS:
			CreateVertexShader(deviceShader);
			break;
		case EShaderType::PS:
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
		
		HR(mD3D11Device_->mDevice_->CreateBuffer(
				&bufferDesc, 
			buffer->Data ? &initData : nullptr,
				deviceBuffer->ppBuffer.GetAddressOf()
			)
		);
	}
}

