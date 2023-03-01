#pragma once
#include "Core/Config.h"

// Define Common Enumerate
namespace Eggy
{
	enum class ERenderSet : uint32
	{
		NONE = 0,
		Main = 0x1,
		PostProcess = 0x2,

		START = Main,
		END = Main + 1,
	};
	using ERenderSets = uint32;


	enum class EFormat : uint16
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
		R32G32F,
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

	enum class ETextureType : uint8
	{
		UNDEFINE = 0,
		Texture2D,
		Texture3D,
		TextureCube,
		END
	};

	enum class ESamplerQuality
	{
		DEFAULT = 0,
		Quality2X,
		Quality4X,
		Quality8X,
		END
	};

	enum class EInputClassification
	{
		UNDEFINE = 0,
		PerVertex,
		PerInstance,
		END
	};

	enum class EShaderStage : uint8
	{
		UNDEFINE = 0,
		START = 0,
		VS = 1,
		PS = 2,
		END = 3
	};
	static const char* ESHADER_TYPE_STR[uint8(EShaderStage::END)] =
	{
		"undefine",
		"vs",
		"ps"
	};


	enum class ETechnique
	{
		Shading = 0,
	};

	enum class EShaderConstant
	{
		Global = 0,
		Shader = 1,
		Batch = 2,
		Skeleton = 3,
		Light = 4,
		END
	};

	enum class EBufferUsage
	{
		Default = 0, // GPU(r&w)
		Immutable = 1, // GPU(r))
		Dynamic = 2, // CPU(w), GPU(r)
		Staging = 3, // CPU(r&w), GPU(r&w)
		END,
	};

	enum class EBufferType
	{
		UNDEFINE = 0x0,
		VertexBuffer = 0x1L,
		IndexBuffer = 0x2L,
		ConstantBuffer = 0x4L,
		ShaderResource = 0x8L,
		StreamOutput = 0x10L,
		RenderTarget = 0x20L,
		DepthStencil = 0x40L,
		END
	};
	using EBufferTypes = uint32;
	inline EBufferTypes operator | (EBufferType a, EBufferType b) 
	{
		return EBufferTypes(a) | EBufferTypes(b);
	}


	enum class ECPUAccessFlag
	{
		UNDEFINE = 0x0,
		Write = 0x1,
		Read = 0x2,
		END
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
		Wrap,
		Mirror,
		Clamp,
		Border,
		Mirror_Once,
		END,
	};

	enum class ECompFunc : uint8
	{
		UNDEFINE = 0,
		Never = 1,
		Less = 2,
		Equal = 3,
		LessEqual = 4,
		Greater = 5,
		NotEqual = 6,
		GreaterEqual = 7,
		Always = 8,
		END,
	};

	enum class EFillMode : uint8
	{
		UNDEFINE = 0,
		Wireframe,
		Solid,
		END
	};

	enum class ECullMode : uint8
	{
		UNDEFINE = 0,
		Front = 1,
		Back = 2,
		CW = Front,
		CCW	= Back,
		END
	};

	enum class EStencilOp : uint8
	{
		UNDEFINE = 0,
		Keep = 1,
		Zero = 2,
		Replace = 3,
		IncrSat = 4,
		DecrSat = 5,
		Invert = 6,
		Incr = 7,
		Decr = 8,
		END
	};

	struct StencilOp
	{
		union
		{
			uint32			HashValue{ 0 };
			struct
			{
				EStencilOp	StencilFailOp : 8;
				EStencilOp	StencilDepthFailOp : 8;
				EStencilOp	StencilPassOp : 8;
				ECompFunc	StencilFunc : 8;
			};
		};

		StencilOp()
		{
			StencilFailOp = EStencilOp::Keep;
			StencilDepthFailOp = EStencilOp::Keep;
			StencilPassOp = EStencilOp::Keep;
			StencilFunc = ECompFunc::Never;
		}
	};

	enum class EBlendOp : uint8
	{
		UNDEFINE = 0,
		Add,
		Substract,
		RevSubstract,
		Min,
		Max,
		END
	};

	enum class EBlendMode : uint8
	{
		UNDEFINE = 0,
		Zero,
		One,
		SrcColor,
		InvSrcColor,
		SrcAlpha,
		InvSrcAlpha,
		DestAlpha,
		InvDestAlpha,
		DestColor,
		InvDestColor,
		SrcAlphaSaturate,
		BlendFactor,
		InvBlendFactor,
		END
	};

	enum class EColorWrite : uint8
	{
		UNDEFINE = 0,
		Red = 0x1,
		Green = 0x2,
		Blue = 0x4,
		Alpha = 0x8,
		Color = Red | Blue | Green,
		All = Color | Alpha
	};

	struct BlendState
	{
		union
		{
			uint32			HashValue{ 0 };
			struct
			{
				EBlendOp	BlendOp : 4;
				EBlendOp	BlendOpAlpha : 4;
				/*1*/
				EBlendMode	SrcBlend : 4;
				EBlendMode	DestBlend : 4;
				/*2*/
				EBlendMode	SrcBlendAlpha : 4;
				EBlendMode	DestBlendAlpha : 4;
				/*3*/
				bool		EnableBlend : 1;
				bool		EnableAlphaToCoverage : 1;
				bool		EnableIndependentBlend : 1; // No use so far, for future.
				uint8		_padding_ : 1;

				EColorWrite	WriteMask : 4;
				/*4*/
			};
		};
		BlendState()
		{
			SrcBlend = EBlendMode::SrcAlpha;
			DestBlend = EBlendMode::InvSrcAlpha;
			BlendOp = EBlendOp::Add;
			SrcBlendAlpha = EBlendMode::One;
			DestBlendAlpha = EBlendMode::InvSrcAlpha;
			BlendOpAlpha = EBlendOp::Add;
			EnableBlend = false;
			EnableAlphaToCoverage = false;
			EnableIndependentBlend = false;
			WriteMask = EColorWrite::All;
		}
	};

	struct RenderState
	{
		union
		{
			uint32 Hash[4]{ 0, 0, 0 ,0 };
			struct
			{
				bool			DepthEnable : 1;
				bool			DepthWriteEnable : 1;
				bool			StencilEnable : 1;
				bool			TwoSidedStencil : 1;
				ECompFunc		DepthFunc : 4;
				/*1*/
				uint8			StencilReadMask;
				/*2*/
				uint8			StencilWriteMask;
				/*3*/
				EFillMode		FillMode : 2;
				ECullMode		CullMode : 2;
				bool			EnableDepthClip : 1;
				bool			EnableScissor : 1;
				bool			EnableMultisample : 1;
				bool			EnableAntialiasedLine : 1;
				/*4*/
				StencilOp		FrontFace;
				/*8*/
				StencilOp		BackFace;
				/*12*/
				BlendState		Blending;
				/*16*/
			};
		};
		RenderState()
		{
			DepthEnable = true;
			DepthWriteEnable = true;
			StencilEnable = false;
			TwoSidedStencil = false;
			DepthFunc = ECompFunc::Less;
			StencilReadMask = 0x0;
			StencilWriteMask = 0x0;
			FillMode = EFillMode::Solid;
			CullMode = ECullMode::Back;
			EnableDepthClip = true;
			EnableScissor = false;
			EnableMultisample = false;
			EnableAntialiasedLine = false;
		}
	};
}