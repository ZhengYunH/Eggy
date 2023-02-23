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
		}

		String GetShaderPath(EShaderType shaderType)
		{
			switch (shaderType)
			{
			case EShaderType::UNDEFINE:
				Unimplement();
				break;
			case EShaderType::VS:
				return mResource_->mShader_ + "_VS.hlsl";
				break;
			case EShaderType::PS:
				return mResource_->mShader_ + "_PS.hlsl";
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

	protected:
		MaterialResource* mResource_;
		Map<String, Texture*> mTextures_;
	};
}