#pragma once
#include "Core/Config.h"
#include "Core/Object/IObject.h"
#include "Core/Engine/Resource/Material.h"
#include "Core/Engine/Resource/Texture.h"


namespace Eggy
{
	class Material : public IObject
	{
	public:
		void SetResource(MaterialResource* Resource)
		{
			mResource_ = Resource;
			for (size_t i = 0; i < Resource->mTexturePaths_.size(); ++i)
			{
				String key = std::to_string(i);
				TextureResource* res = new TextureResource(Resource->mTexturePaths_[i]);
				res->Deserialize(nullptr);
				mTextures_[key] = new Texture();
				mTextures_[key]->SetResource(res);
			}
			mShaderCollection_ = new IShaderCollection();
			mShaderCollection_->SetShaderPath(EShaderType::VS, GetShaderPath(EShaderType::VS));
			mShaderCollection_->SetShaderPath(EShaderType::PS, GetShaderPath(EShaderType::PS));
		}

		String GetShaderPath(EShaderType shaderType)
		{
			switch (shaderType)
			{
			case Eggy::EShaderType::UNDEFINE:
				Unimplement();
				break;
			case Eggy::EShaderType::VS:
				return "Engine/Shader/HLSL/" + mResource_->mShader_ + "_VS.hlsl";
				break;
			case Eggy::EShaderType::PS:
				return "Engine/Shader/HLSL/" + mResource_->mShader_ + "_PS.hlsl";
				break;
			default:
				break;
			}
			return "";
		}

		Map<String, Texture*>& GetTextures()
		{
			return mTextures_;
		}

		IShaderCollection* GetShaderCollection()
		{
			return mShaderCollection_;
		}

	protected:
		MaterialResource* mResource_;
		Map<String, Texture*> mTextures_;
		IShaderCollection* mShaderCollection_;
	};
}