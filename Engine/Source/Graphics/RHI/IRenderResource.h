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
		size_t Count{ 0 };
		size_t ByteWidth{ 0 };
		ECPUAccessFlags CPUAcesssFlags{ 0 };
		
		void* Data{ nullptr };

		virtual void CreateDeviceResource_Impl(IRenderResourceFactory* factory) override
		{
			if (IsResourceCreated())
				return;
			factory->CreateBuffer(this);
		}

		virtual bool IsValid() override 
		{
			return BindType != EBufferType::None && Count && Data;
		}
	};

	struct IConstantBuffer : public IBuffer
	{
		IConstantBuffer()
		{
			Usage = EBufferUsage::Dynamic;
			BindType = EBufferType::ConstantBuffer;
			CPUAcesssFlags = ECPUAccessFlags(ECPUAccessFlag::Write);
		}

		virtual bool IsValid() override
		{
			return IBuffer::IsValid() && ByteWidth % 16 == 0;
		}
	};

	struct IInputLayout : public IRenderResource
	{
		struct InputElementDesc
		{
			String SemanticName;
			uint16 SemanticIndex{ 0 };
			EFormat Format{ EFormat::UNDEFINED };
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
		struct IShaderCollection* ShaderCollection{ nullptr };

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

	struct GeometryBinding : IRenderResource
	{
		IInputLayout Layout;
		IBuffer VertexBuffer;
		IBuffer IndexBuffer;

		GeometryBinding()
		{
			VertexBuffer.BindType = EBufferType::VertexBuffer;
			IndexBuffer.BindType = EBufferType::IndexBuffer;
		}

		virtual void CreateDeviceResource_Impl(IRenderResourceFactory* factory)
		{
			if (IsResourceCreated())
				return;
			Layout.CreateDeviceResource(factory);
			VertexBuffer.CreateDeviceResource(factory);
			IndexBuffer.CreateDeviceResource(factory);
		}

		virtual bool IsValid() override
		{
			return Layout.IsValid() && VertexBuffer.IsValid();
		}
		
		virtual bool IsResourceCreated() override
		{
			return Layout.IsResourceCreated() && VertexBuffer.IsResourceCreated() && IndexBuffer.IsResourceCreated();
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
		IConstantBuffer Constant;
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
		GeometryBinding* GeometryBinding_;
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