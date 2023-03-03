#pragma once
#include "IRenderHeader.h"
#include "Shader/ShaderParamter.h"
#include "Shader/ShaderCollection.h"


namespace Eggy
{

	struct ResourceBinding
	{
		uint16 Textures{ 0 };
		uint16 Buffers{ 0 };
		uint16 Views{ 0 };

		// Texture -> Buffer -> Views
		IRenderResource** Data{ nullptr };

		ResourceBinding(uint16 textures, uint16 buffers, uint16 views)
		{
			Textures = textures;
			Buffers = buffers;
			Views = views;

			Data = new IRenderResource * [textures + buffers + views];
			for (uint16 i = 0; i < Textures + Buffers + Views; ++i)
				Data[i] = nullptr;
		}

		~ResourceBinding()
		{
			SafeDestroyArray(Data);
		}

		void CreateDeviceResource(IRenderResourceFactory* factory)
		{
			for (uint16 i = 0; i < Textures; ++i)
			{
				if (!Data[i])
					continue;
				ITextureBuffer* texture = (ITextureBuffer*)Data[i];
				HYBRID_CHECK(texture);
				texture->CreateDeviceResource(factory);
			}
		}

		void SetTexture(uint16 i, ITextureBuffer* tex)
		{
			Data[i] = tex;
		}
		ITextureBuffer* GetTexture(uint16 i)
		{
			return (ITextureBuffer*)Data[i];
		}

		void SetBuffer(uint16 i, IBuffer* tex)
		{
			Data[Textures + i] = tex;
		}

		IBuffer* GetBuffer(uint16 i)
		{
			return (IBuffer*)Data[Textures + i];
		}

		void SetView(uint16 i, IRenderTarget* view)
		{
			Data[Buffers + Textures + i] = view;
		}
		IRenderTarget* GetView(uint16 i)
		{
			return (IRenderTarget*)Data[Buffers + Textures + i];
		}
	};


	class Shader;

	// Container for All Draw CallData
	class IShadingBatch
	{
		friend class IShadingState;
	public:
		IShadingBatch(class Shader* shader);
		~IShadingBatch();
		
	public:
		void CreateDeviceResource(IRenderResourceFactory* factory);
		ShadingParameterCollection* GetConstantBuffer(EShaderConstant esc);
		
		void SetConstantBuffer(EShaderConstant esc, ShadingParameterCollection* parameters);
		void SetTextureBinding(uint16 i, ITextureBuffer* tex);

		bool GetTextureSlot(ETechnique technique, const String& name, uint8& outSlot);
		bool GetSamplerSlot(ETechnique technique, const String& name, uint8& outSlot);
		ResourceBinding* GetResourceBinding();
	protected:
		ResourceBinding* mResourceBindings_;
		Map<EShaderConstant, ShadingParameterCollection*> mShadingParameters_;
		Map<ETechnique, const ShaderTechnique*> mShaderTechnique_;
	};
}