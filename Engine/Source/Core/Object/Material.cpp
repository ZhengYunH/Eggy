#pragma once
#include "System/TextureSystem.h"
#include "Material.h"
#include "Shader.h"
#include "Texture.h"


namespace Eggy
{
	Material::Material(TSharedPtr<MaterialResource> resource)
	{
		mResource_ = resource;
		mShader_ = new Shader(resource->GetShader());
		for (auto pair : resource->GetTextureMap())
		{
			ITexture* texture = TextureSystem::Get()->GetTexture(pair.second);
			if (!texture && resource->GetTexture(pair.first))
			{
				texture = new Texture(resource->GetTexture(pair.first));
			}
			mTextures_[pair.first] = texture;
		}
	}

	Material::~Material()
	{
		SafeDestroy(mShader_);
	}

	String Material::GetShaderPath(EShaderType shaderType)
	{
		HYBRID_CHECK(mShader_);
		return mShader_->GetShaderPath(shaderType);
	}

}

