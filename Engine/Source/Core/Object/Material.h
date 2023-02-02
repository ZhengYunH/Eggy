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
			mTextures_.resize(Resource->mTexturePaths_.size());
			for (size_t i = 0; i < Resource->mTexturePaths_.size(); ++i)
			{
				TextureResource* res = new TextureResource(Resource->mTexturePaths_[i]);
				res->Deserialize(nullptr);
				mTextures_[i].SetResource(res);
			}
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

		List<Texture>& GetTextures()
		{
			return mTextures_;
		}

	protected:
		MaterialResource* mResource_;
		List<Texture> mTextures_;
	};
}