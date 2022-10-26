#pragma once
#include "Core/Config.h"

// Define Common Enumerate
namespace Eggy
{
	enum class EFormat
	{
		UNDEFINED = 0,
		R32G32B32,
		A32R32G32B32F,
		A16B16G16R16F,
		R8G8B8A8,
		D32_SFLOAT_S8_UINT,
		R8_UINT,
		R16_UINT,
		R32_UINT
	};

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
		VertexBuffer = 0x1,
		IndexBuffer = 0x2,
		ConstantBuffer = 0x3
	};
}