#pragma once
#include "Core/Config.h"

// Define Common Enumerate
namespace Eggy
{
	enum class EFormat : uint32
	{
		UNDEFINED = 0,
		R32G32B32,
		A32R32G32B32F,
		A16B16G16R16F,
		R8G8B8A8,
		D32_SFLOAT_S8X24_UINT,
		R8_UINT,
		R16_UINT,
		R32_UINT,
		END,
	};

	struct EFormatInfo
	{
		uint32 DataSize;
	};

	static EFormatInfo GFormatInfo[uint32(EFormat::END)] =
	{
		{ 0 }, // UNDEFINED
		{ 4 * 3 }, // R32G32B32,
		{ 4 * 4 }, // A32R32G32B32F,
		{ 2 * 4 }, // A16B16G16R16F,
		{ 1 * 4 }, // R8G8B8A8,
		{ 4 + 4 }, // D32_SFLOAT_S8X24_UINT,
		{ 1 }, // R8_UINT,
		{ 2 }, // R16_UINT,
		{ 4 } // R32_UINT,
	};

	const EFormatInfo& GetFormatInfo(EFormat format);

	enum class ETextureType
	{
		None = 0,
		Texture2D,
		Texture3D,
		TextureCube,
	};

	enum class ESamplerQuality
	{
		None = 0,
		Quality2X,
		Quality4X,
		Quality8X,
	};

	enum class EInputClassification
	{
		None = 0,
		PerVertex,
		PerInstance
	};

	enum class EShaderType
	{
		None = 0,
		VS = 1,
		PS = 2,
	};

	enum class EBufferUsage
	{
		Default = 0, // GPU(r&w)
		Immutable = 1, // GPU(r))
		Dynamic = 2, // CPU(w), GPU(r)
		Staging = 3, // CPU(r&w), GPU(r&w)
	};

	enum class EBufferType
	{
		None = 0x0,
		VertexBuffer = 0x1L,
		IndexBuffer = 0x2L,
		ConstantBuffer = 0x4L,
		ShaderResource = 0x8L
	};

	enum class ECPUAccessFlag
	{
		None = 0x0,
		Write = 0x1,
		Read = 0x2
	};
	using ECPUAccessFlags = uint32;

	enum class EFilterType : uint8
	{
		UNDEFINE = 0,
		MIN_MAG_MIP_POINT,
		MIN_MAG_MIP_LINEAR,
		ANISOTROPIC,
		END,
	};

	enum class EAddressMode : uint8
	{
		UNDEFINE = 0,
		WRAP,
		MIRROR,
		CLAMP,
		BORDER,
		MIRROR_ONCE,
		END,
	};
}