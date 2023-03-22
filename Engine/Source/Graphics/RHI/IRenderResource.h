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
		IDevcieResource* DeviceResource{ nullptr };

		virtual ~IRenderResource()
		{
			SafeDestroy(DeviceResource);
		}
	};

	struct IBuffer : public IRenderResource
	{
		EBufferUsage Usage{ EBufferUsage::Immutable };
		EBufferTypes BindType{ EBufferTypes(EBufferType::UNDEFINE) };
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
			return BindType != EBufferTypes(EBufferType::UNDEFINE) && Count && Data;
		}
	};

	struct IConstantBuffer : public IBuffer
	{
		IConstantBuffer()
		{
			Usage = EBufferUsage::Dynamic;
			BindType = EBufferTypes(EBufferType::ConstantBuffer);
			CPUAcesssFlags = ECPUAccessFlags(ECPUAccessFlag::Write);
		}

		virtual bool IsValid() override
		{
			return IBuffer::IsValid() && ByteWidth % 16 == 0;
		}
	};

	struct ITextureBuffer : public IBuffer
	{
		ITextureBuffer() : IBuffer()
		{
			Usage = EBufferUsage::Immutable;
			BindType = EBufferTypes(EBufferType::ShaderResource);
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

	struct IRenderTarget : public ITextureBuffer
	{
		IRenderTarget() : ITextureBuffer()
		{
			Usage = EBufferUsage::Default;
			CPUAcesssFlags = 0;
			Count = 1;
			BindType = EBufferTypes(EBufferType::ShaderResource | EBufferType::RenderTarget);
			TextureType = ETextureType::Texture2D;
		}

		virtual void CreateDeviceResource_Impl(IRenderResourceFactory* factory) override
		{
			if (IsResourceCreated())
				return;
			if (IsDepthStencil())
			{
				factory->CreateDepthStencil(this);
			}
			else
			{
				factory->CreateRenderTarget(this);
			}
		}

		bool IsDepthStencil()
		{
			return BindType & EBufferTypes(EBufferType::DepthStencil);
		}

		bool IsBackBuffer{ false };
		ESamplerQuality Quality{ ESamplerQuality::DEFAULT };
	};

	struct IDepthStencil : public ITextureBuffer
	{
		IDepthStencil() : ITextureBuffer()
		{
			Usage = EBufferUsage::Default;
			CPUAcesssFlags = 0;
			Count = 1;
			BindType = EBufferTypes(EBufferType::DepthStencil);
			TextureType = ETextureType::Texture2D;
			Format = EFormat::D32_SFLOAT_S8X24_UINT;
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
			EInputClassification SlotClass{ EInputClassification::UNDEFINE };
		};

		virtual void CreateDeviceResource_Impl(IRenderResourceFactory* factory) override
		{
			if (IsResourceCreated())
				return;
			HYBRID_CHECK(VSShader);
			factory->CreateInputLayout(this, VSShader);
		}

		List<InputElementDesc> Descs;
		struct ShaderRenderResource* VSShader{ nullptr };

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
			VertexBuffer.BindType = EBufferTypes(EBufferType::VertexBuffer);
			IndexBuffer.BindType = EBufferTypes(EBufferType::IndexBuffer);
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

	struct PipelineState : IRenderResource
	{
		RenderState State;

		void CreateDeviceResource_Impl(IRenderResourceFactory* factory) override
		{
			factory->CreatePipelineState(this);
		}

		size_t Hash() const
		{
			size_t v = ((size_t)State.Hash[0]) + ((size_t)State.Hash[1] << 16) + ((size_t)State.Hash[2] << 24) + ((size_t)State.Hash[3] << 32);
			return v;
		}
	};
}

namespace std
{
	using namespace Eggy;
	template<> struct hash<PipelineState>
	{
		size_t operator()(PipelineState const& ps) const noexcept
		{
			return ps.Hash();
		}
	};
}