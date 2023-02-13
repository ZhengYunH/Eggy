#pragma once
#include "IRenderResource.h"
#include "Core/Object/Material.h"
#include "Core/DataType/WeakPtr.h"


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

		IShadingState(TWeakPtr<Material> material)
		{
			ShaderCollection = new IShaderCollection();
			Initialize(material);
		}
		
		~IShadingState()
		{
			SafeDestroy(ShaderCollection);
		}

	public:
		void Initialize(TWeakPtr<Material> material)
		{
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
		List<ITexture*> BindingTextures;
		List<IConstantBuffer*> BindingConstants;
		IShaderCollection* ShaderCollection;

	protected:
		TWeakPtr<Material> mMaterial;

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
					BindingTextures[slot] = new ITexture();
					ITexture* bindingTex = BindingTextures[slot];

					auto& resource = pair.second->GetResource();
					bindingTex->Data = resource.GetData();
					bindingTex->Width = resource.Size.x;
					bindingTex->Height = resource.Size.y;
					bindingTex->Mips = resource.Mips;
					bindingTex->Format = resource.Format;
					bindingTex->ByteWidth = resource.ByteWidth;
					bindingTex->TextureType = resource.TextureType;
				}
			}
		}
		
	};
}
