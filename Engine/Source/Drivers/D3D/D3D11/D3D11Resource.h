#pragma once
#include "D3D11Common.h"
#include "Core/Config.h"
#include "Graphics/RHI/IRenderResource.h"
#include "Graphics/RHI/IRenderDevice.h"


namespace Eggy
{
	struct Converter
	{
		static DXGI_FORMAT Format(EPixelFormat format)
		{
			switch (format)
			{
			case EPixelFormat::UNDEFINED:
				Unimplement();
			case EPixelFormat::R32G32B32:
				return DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
			case EPixelFormat::A32R32G32B32F:
				return DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
			case EPixelFormat::A16B16G16R16F:
				return DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT;
			case EPixelFormat::R8G8B8A8:
				return DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM;
				break;
			case EPixelFormat::D32_SFLOAT_S8_UINT:
				return DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
			default:
				Unimplement();
				break;
			}
			return DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
		}

		static D3D11_INPUT_CLASSIFICATION InputClassification(EInputClassification classification)
		{
			switch (classification)
			{
			case EInputClassification::None:
				Unimplement();
			case EInputClassification::PerVertex:
				return D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
			case EInputClassification::PerInstance:
				return D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_INSTANCE_DATA;
			default:
				Unimplement();
				break;
			}
			return D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
		}
	
		static D3D11_USAGE Usage(EBufferUsage usage)
		{
			switch (usage)
			{
			case EBufferUsage::Default:
				return D3D11_USAGE::D3D11_USAGE_DEFAULT;
			case EBufferUsage::Immutable:
				return D3D11_USAGE::D3D11_USAGE_IMMUTABLE;
			case EBufferUsage::Dynamic:
				return D3D11_USAGE::D3D11_USAGE_DYNAMIC;
			case EBufferUsage::Staging:
				return D3D11_USAGE::D3D11_USAGE_STAGING;
			default:
				Unimplement();
				break;
			}
			return D3D11_USAGE::D3D11_USAGE_DEFAULT;
		}

		static D3D11_BIND_FLAG BufferType(EBufferType type)
		{
			switch (type)
			{
			case EBufferType::VertexBuffer:
				return D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
			case EBufferType::IndexBuffer:
				return D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
			case EBufferType::ConstantBuffer:
				return D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
			default:
				Unimplement(0);
				break;
			}
			return D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
		}
	};

	class D3D11Device;
	struct D3D11Shader;
	struct IShaderCollection;


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

	class D3D11ResourceFactory : public IRenderResourceFactory
	{
	public:
		D3D11ResourceFactory(class D3D11Device* device) : mD3D11Device_(device)
		{
		}

		void CreateInputLayout(IInputLayout* inputLayout, IShaderCollection* shaderCollection);
		void CreateShader(IShader* shader);
		void CreateBuffer(IBuffer* buffer);

	protected:
		void CreateShaderFromFile(D3D11Shader* deviceShader);
		void CreateVertexShader(D3D11Shader* deviceShader);
		void CreatePixelShader(D3D11Shader* deviceShader);


	protected:
		class D3D11Device* mD3D11Device_;
	};
}