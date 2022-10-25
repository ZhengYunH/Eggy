#pragma once
#include "Core/Config.h"
#include "Graphics/RHI/IRenderDevice.h"


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

	struct IRenderResource
	{
		virtual void CreateDeviceResource(IRenderResourceFactory* factory)
		{
			if (IsResourceCreated())
				return;
			CreateDeviceResourceImp(factory);
		}
		virtual void CreateDeviceResourceImp(IRenderResourceFactory* factory) = 0;
		virtual bool IsValid() { return true; }
		virtual bool IsResourceCreated() { return DeviceResource != nullptr; }
		void* DeviceResource{ nullptr };
	};

	struct IBuffer : public IRenderResource
	{
		EBufferUsage Usage{ EBufferUsage::Immutable };
		EBufferType BindType{ EBufferType::None };
		size_t Size{ 0 };
		size_t Stride{ 0 };
		void* Data{ nullptr };

		virtual void CreateDeviceResourceImp(IRenderResourceFactory* factory) override
		{
			if (IsResourceCreated())
				return;
			factory->CreateBuffer(this);
		}

		virtual bool IsValid() override 
		{
			return BindType != EBufferType::None && Size && Data;
		}
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

		virtual void CreateDeviceResourceImp(IRenderResourceFactory* factory) override
		{
			if (IsResourceCreated())
				return;
			HYBRID_CHECK(ShaderCollection);
			factory->CreateInputLayout(this, ShaderCollection);
		}

		List<InputElementDesc> Descs;
		struct IShaderCollection* ShaderCollection;

		virtual bool IsValid() override { return !Descs.empty(); }
	};

	template<typename _TVertexType>
	struct InputLayout : public IInputLayout
	{
		InputLayout() 
		{
			_TVertexType::GetDesc(Descs);
		}
	};

	struct IGeometry : IRenderResource
	{
		IInputLayout Layout;
		IBuffer VertexBuffer;

		IGeometry()
		{
			VertexBuffer.BindType = EBufferType::VertexBuffer;
		}

		virtual void CreateDeviceResourceImp(IRenderResourceFactory* factory)
		{
			if (IsResourceCreated())
				return;
			Layout.CreateDeviceResourceImp(factory);
			VertexBuffer.CreateDeviceResourceImp(factory);
		}

		virtual bool IsValid() override
		{
			return Layout.IsValid() && VertexBuffer.IsValid();
		}
		
		virtual bool IsResourceCreated() override
		{
			return Layout.IsResourceCreated() && VertexBuffer.IsResourceCreated();
		}
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