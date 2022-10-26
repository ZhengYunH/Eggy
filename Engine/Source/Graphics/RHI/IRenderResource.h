#pragma once
#include "Core/Config.h"
#include "Graphics/RHI/IRenderDevice.h"
#include "IRenderHeader.h"


namespace Eggy
{
	struct IRenderResource
	{
		virtual void CreateDeviceResource(IRenderResourceFactory* factory)
		{
			if (IsResourceCreated())
				return;
			CreateDeviceResource_Impl(factory);
		}
		virtual void CreateDeviceResource_Impl(IRenderResourceFactory* factory) = 0;
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

		virtual void CreateDeviceResource_Impl(IRenderResourceFactory* factory) override
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

		virtual void CreateDeviceResource_Impl(IRenderResourceFactory* factory) override
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

		virtual void CreateDeviceResource_Impl(IRenderResourceFactory* factory)
		{
			if (IsResourceCreated())
				return;
			Layout.CreateDeviceResource_Impl(factory);
			VertexBuffer.CreateDeviceResource_Impl(factory);
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