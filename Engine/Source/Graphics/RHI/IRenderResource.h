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
		EBufferType BindType{ EBufferType::UNDEFINE };
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
			return BindType != EBufferType::UNDEFINE && Count && Data;
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

	struct ITexture : public IBuffer
	{
		ITexture() : IBuffer()
		{
			Usage = EBufferUsage::Immutable;
			BindType = EBufferType::ShaderResource;
			Count = 1;
			CPUAcesssFlags = ECPUAccessFlags(ECPUAccessFlag::Read);
		}

		virtual void CreateDeviceResource_Impl(IRenderResourceFactory* factory) override
		{
			if (IsResourceCreated())
				return;
			factory->CreateTexture(this);
		}

		uint32 Width{ 0 };
		uint32 Height{ 0 };
		uint8 Mips{ 0 };
		ETextureType TextureType{ ETextureType::UNDEFINE };
		EFormat Format{ EFormat::UNDEFINED };
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
			EInputClassification SlotClass{ EInputClassification::UNDEFINE };
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

	struct SamplerState : IRenderResource
	{
		EFilterType Filter{ EFilterType::MIN_MAG_MIP_LINEAR };
		EAddressMode AddressU{ EAddressMode::Wrap };
		EAddressMode AddressV{ EAddressMode::Wrap };
		EAddressMode AddressW{ EAddressMode::Wrap };
		float MipLodBias{ 0 };
		uint32 MaxAnisotropy{ 0 };
		float MinLod{ 0 };
		float MaxLod{ -1 };

		void CreateDeviceResource_Impl(IRenderResourceFactory* factory) override
		{
			factory->CreateSamplerState(this);
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

	struct PipelineState : IRenderResource
	{
		RenderState State;

		void CreateDeviceResource_Impl(IRenderResourceFactory* factory) override
		{
			factory->CreatePipelineState(this);
		}

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