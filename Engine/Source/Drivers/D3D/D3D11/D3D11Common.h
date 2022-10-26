#pragma once
#include "Core/Config.h"
#include "Graphics/RHI/IRenderHeader.h"

#include <d3d11_1.h>
#include <d3dcommon.h>
#include <dxgiformat.h>
#include <wrl/client.h>

namespace Eggy
{
	template <class T>
	using TComPtr = Microsoft::WRL::ComPtr<T>;


	struct Converter
	{
		static DXGI_FORMAT Format(EFormat format)
		{
			switch (format)
			{
			case EFormat::UNDEFINED:
				Unimplement();
			case EFormat::R32G32B32:
				return DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
			case EFormat::A32R32G32B32F:
				return DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
			case EFormat::A16B16G16R16F:
				return DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT;
			case EFormat::R8G8B8A8:
				return DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM;
				break;
			case EFormat::D32_SFLOAT_S8_UINT:
				return DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
			case EFormat::R8_UINT:
				return DXGI_FORMAT::DXGI_FORMAT_R8_UINT;
			case EFormat::R16_UINT:
				return DXGI_FORMAT::DXGI_FORMAT_R16_UINT;
			case EFormat::R32_UINT:
				return DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
			default:
				Unimplement();
				break;
			}
			return DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
		}

		static DXGI_FORMAT IndexFormat(size_t indexSize)
		{
			if (indexSize == sizeof(uint8))
			{
				return DXGI_FORMAT::DXGI_FORMAT_R8_UINT;
			}
			else if (indexSize == sizeof(uint16))
			{
				return DXGI_FORMAT::DXGI_FORMAT_R16_UINT;
			}
			else if (indexSize == sizeof(uint32))
			{
				return DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
			}

			Unimplement();
			return DXGI_FORMAT::DXGI_FORMAT_R8_UINT;
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

		static uint32 CPUAccessFlag(uint32 flag)
		{
			uint32 d3dFlag = 0;
			if (flag & uint32(ECPUAccessFlag::Read))
				d3dFlag |= D3D11_CPU_ACCESS_READ;
			if (flag & uint32(ECPUAccessFlag::Write))
				d3dFlag |= D3D11_CPU_ACCESS_WRITE;
			return d3dFlag;
		}
	};
}