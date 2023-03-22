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
				return DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
			case EFormat::R8G8B8A8_SNORM:
				return DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_SNORM;
			case EFormat::D32_SFLOAT_S8X24_UINT:
				return DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
			case EFormat::R8_UINT:
				return DXGI_FORMAT::DXGI_FORMAT_R8_UINT;
			case EFormat::R16_UINT:
				return DXGI_FORMAT::DXGI_FORMAT_R16_UINT;
			case EFormat::R16_UNORM:
				return DXGI_FORMAT::DXGI_FORMAT_R16_UNORM;
			case EFormat::R32_UINT:
				return DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
			case EFormat::R32G32F:
				return DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT;
			case EFormat::R32F:
				return DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
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
			case EInputClassification::UNDEFINE:
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

		static UINT BufferType(EBufferTypes types)
		{
			UINT d3dType = 0;
			auto MapType = [&d3dType, &types](EBufferType type, D3D11_BIND_FLAG d3dFlag)
			{
				if (types & EBufferTypes(type))
					d3dType |= d3dFlag;
			};
			MapType(EBufferType::VertexBuffer, D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER);
			MapType(EBufferType::IndexBuffer, D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER);
			MapType(EBufferType::ConstantBuffer, D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER);
			MapType(EBufferType::ShaderResource, D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE);
			MapType(EBufferType::StreamOutput, D3D11_BIND_FLAG::D3D11_BIND_STREAM_OUTPUT);
			MapType(EBufferType::RenderTarget, D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET);
			MapType(EBufferType::DepthStencil, D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL);
			return d3dType;
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

		static D3D11_FILTER FilterType(EFilterType filterType)
		{
			static D3D11_FILTER filterTypes[uint32(EFilterType::END)] = {
				D3D11_FILTER_MIN_MAG_MIP_POINT, // UNDEFINE
				D3D11_FILTER_MIN_MAG_MIP_POINT, // MIN_MAG_MIP_POINT
				D3D11_FILTER_MIN_MAG_MIP_LINEAR, // MIN_MAG_MIP_LINEAR
				D3D11_FILTER_ANISOTROPIC, // ANISOTROPIC
			};

			HYBRID_CHECK(uint32(filterType) <= uint32(EFilterType::END));
			return filterTypes[uint32(filterType)];
		}

		static D3D11_TEXTURE_ADDRESS_MODE AddressMode(EAddressMode mode)
		{
			static D3D11_TEXTURE_ADDRESS_MODE addressModes[uint32(EAddressMode::END)] = {
				D3D11_TEXTURE_ADDRESS_WRAP, // UNDEFINE
				D3D11_TEXTURE_ADDRESS_WRAP, // WRAP
				D3D11_TEXTURE_ADDRESS_MIRROR, // MIRROR
				D3D11_TEXTURE_ADDRESS_CLAMP, // CLAMP
				D3D11_TEXTURE_ADDRESS_BORDER, // BORDER
				D3D11_TEXTURE_ADDRESS_MIRROR_ONCE, // MIRROR_ONCE
			};

			HYBRID_CHECK(uint32(mode) <= uint32(EAddressMode::END));
			return addressModes[uint32(mode)];
		}

		static D3D11_FILL_MODE FillMode(EFillMode mode)
		{
			static D3D11_FILL_MODE fillModes[uint32(EFillMode::END)] = {
				D3D11_FILL_SOLID, // UNDEFINE
				D3D11_FILL_WIREFRAME, // Wireframe
				D3D11_FILL_SOLID // Solid
			};

			HYBRID_CHECK(uint32(mode) <= uint32(EFillMode::END));
			return fillModes[uint32(mode)];
		}

		static D3D11_CULL_MODE CullMode(ECullMode mode)
		{
			static D3D11_CULL_MODE cullModes[uint32(ECullMode::END)] = {
				D3D11_CULL_NONE, // UNDEFINE
				D3D11_CULL_FRONT, // Front, CW
				D3D11_CULL_BACK // Back, CCW
			};

			HYBRID_CHECK(uint32(mode) <= uint32(ECullMode::END));
			return cullModes[uint32(mode)];
		}

		static D3D11_BLEND Blend(EBlendMode mode)
		{
			static D3D11_BLEND blendModes[uint32(EBlendMode::END)] = {
				D3D11_BLEND_ZERO, // UNDEFINE
				D3D11_BLEND_ZERO, // Zero
				D3D11_BLEND_ONE , // One
				D3D11_BLEND_SRC_COLOR, // SrcColor
				D3D11_BLEND_INV_SRC_COLOR, // InvSrcColor
				D3D11_BLEND_SRC_ALPHA, // SrcAlpha
				D3D11_BLEND_INV_SRC_ALPHA, // InvSrcAlpha
				D3D11_BLEND_DEST_ALPHA, // DestAlpha
				D3D11_BLEND_INV_DEST_ALPHA, // InvDestAlpha
				D3D11_BLEND_DEST_COLOR, // DestColor
				D3D11_BLEND_INV_DEST_COLOR, // InvDestColor
				D3D11_BLEND_SRC_ALPHA_SAT, // SrcAlphaSaturate
				D3D11_BLEND_BLEND_FACTOR, // BlendFactor
				D3D11_BLEND_INV_BLEND_FACTOR, // InvBlendFactor
			};

			HYBRID_CHECK(uint32(mode) <= uint32(EBlendMode::END));
			return blendModes[uint32(mode)];
		}

		static D3D11_BLEND_OP BlendOp(EBlendOp op)
		{
			static D3D11_BLEND_OP blendOps[uint32(EBlendOp::END)] = {
				D3D11_BLEND_OP_ADD, // UNDEFINE
				D3D11_BLEND_OP_ADD, // Add,
				D3D11_BLEND_OP_SUBTRACT, // Substract,
				D3D11_BLEND_OP_REV_SUBTRACT, // RevSubstract,
				D3D11_BLEND_OP_MIN, // Min,
				D3D11_BLEND_OP_MAX, // Max
			};

			HYBRID_CHECK(uint32(op) <= uint32(EBlendOp::END));
			return blendOps[uint32(op)];
		}

		static UINT8 ColorWriteMask(EColorWrite colorWrite)
		{
			return UINT8(colorWrite);
		}
		
		static UINT SamplerCount(ESamplerQuality quality)
		{
			static UINT smplerCounts[uint32(ESamplerQuality::END)] =
			{
				1, //DEFAULT
				2,
				4,
				8
			};

			HYBRID_CHECK(uint32(quality) <= uint32(ESamplerQuality::END));
			return smplerCounts[uint32(quality)];
		}

		static float SamplerQualityFactor(ESamplerQuality quality)
		{
			static float smplerQualities[uint32(ESamplerQuality::END)] =
			{
				0, //DEFAULT
				0.25,
				0.5,
				1
			};

			HYBRID_CHECK(uint32(quality) <= uint32(ESamplerQuality::END));
			return smplerQualities[uint32(quality)];
		}
	};
}