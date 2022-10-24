#pragma once
#include "Core/Config.h"


namespace Eggy
{
	enum class EPixelFormat
	{
		UNDEFINED = 0,
		R32G32B32,
		A32R32G32B32F,
		A16B16G16R16F,
		R8G8B8A8,
		D32_SFLOAT_S8_UINT,
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
		PER_VERTEX,
		PER_INSTANCE
	};

	struct IRenderResource
	{
		virtual void CreateDeviceResource() {}
		void* DeviceResource{ nullptr };
	};

	struct IInputLayout : public IRenderResource
	{
		struct InputElementDesc
		{
			String SemanticName;
			uint16 SemanticIndex{ 0 };
			EPixelFormat Format{ EPixelFormat::UNDEFINED };
			uint16 InputSlot{ 0 };
			uint16 AlignedByteOffset{ 0 };
			EInputClassification SlotClass{ EInputClassification::None };
		};

		List<InputElementDesc> Descs;
	};

	template<typename _TVertexType>
	struct InputLayout : public IRenderResource 
	{
		InputLayout() 
		{
			_TVertexType::GetDesc(Descs);
		}
	};


	struct IGeometry : IRenderResource
	{
		IInputLayout* Layout;
	};

	struct ResourceBinding
	{
		uint16 Textures{ 0 };
		uint16 Buffers{ 0 };
		uint16 Views{ 0 };

		IRenderResource* Data;
	};

	struct ShadingState : IRenderResource 
	{
		// ShaderInstance Shader{nullptr};
		byte* Constant_;
	};

	struct RenderState : IRenderResource
	{

	};

	struct PipelineState : IRenderResource
	{
		RenderState* RenderState_;
	};

	struct RenderItemInfo
	{
		IGeometry* GeometryBinding_;
		ResourceBinding* ResourceBinding_;
	};

	struct RenderItem
	{
		RenderItemInfo* Info{ nullptr };
	};

	struct DrawCall
	{
		RenderItem* Item{ nullptr };
	};
}