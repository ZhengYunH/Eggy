#pragma once
#include "IRenderResource.h"
#include "Core/Object/Material.h"
#include "Core/DataType/WeakPtr.h"
#include "System/TextureSystem.h"


namespace Eggy
{
	class Texture;
	struct IShaderCollection;
	
	class IShadingState
	{
	public:
		IShadingState()
		{
			ShaderCollection = new IShaderCollection();
		}

		IShadingState(Material* material)
		{
			ShaderCollection = new IShaderCollection();
			Initialize(material);
		}
		
		~IShadingState()
		{
			SafeDestroy(ShaderCollection);
		}

	public:
		void Initialize(Material* material)
		{
			if (mMaterial && mMaterial == material)
				return;

			ShaderCollection->SetShaderPath(EShaderType::VS, material->GetShaderPath(EShaderType::VS));
			ShaderCollection->SetShaderPath(EShaderType::PS, material->GetShaderPath(EShaderType::PS));
			mMaterial = material;
			OnShaderCreated();
		}

		void OnShaderCreated()
		{
			DoConstantBinding();
			DoTextureBinding();
		}

	public:
		List<ITextureBuffer*> BindingTextures;
		List<IConstantBuffer*> BindingConstants;
		IShaderCollection* ShaderCollection;

	protected:
		Material* mMaterial{ nullptr };

	protected:
		void DoConstantBinding()
		{
			BindingConstants.resize(ShaderCollection->GetConstantSize(), nullptr);
			if (!mMaterial)
				return;

			for (uint8 i = 0; i < ShaderCollection->GetConstantSize(); ++i)
			{
				BindingConstants[i] = new IConstantBuffer();
			}
		}

		void DoTextureBinding()
		{
			BindingTextures.resize(ShaderCollection->GetTextureSize(), nullptr);
			if (!mMaterial)
				return;
			for (auto& pair : mMaterial->GetTextures())
			{
				uint8 slot = ShaderCollection->GetTextureSlot(pair.first);
				if (slot != IShaderCollection::INVALID_SLOT)
				{
					BindingTextures[slot] = new ITextureBuffer();
					ITextureBuffer* bindingTex = BindingTextures[slot];

					ITexture* texture = pair.second;
					if (texture)
					{
						auto& info = texture->GetInfo();
						bindingTex->Data = texture->GetData();
						bindingTex->Width = info.Size.x;
						bindingTex->Height = info.Size.y;
						bindingTex->Mips = info.Mips;
						bindingTex->Format = info.Format;
						bindingTex->ByteWidth = info.ByteWidth;
						bindingTex->TextureType = info.TextureType;
					}
				}
			}
		}
		
	};
}
