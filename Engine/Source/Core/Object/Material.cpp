#pragma once
#include "System/TextureSystem.h"
#include "Material.h"
#include "Shader.h"
#include "Texture.h"


namespace Eggy
{
	Material::Material(Shader* shader) 
		: mShader_(shader)
	{
		auto technique = shader->GetShaderCollection()->GetShaderTechnique(ETechnique::Shading);
		mParams_ = new ShadingParameterCollection(technique->GetConstantTable(EShaderConstant::Shader));

		float roughness = 1;
		mParams_->SetFloat("Roughness", 0, 1, &roughness);
	}

	Material::Material(TSharedPtr<MaterialResource> resource)
		: Material(new Shader(resource->GetShader()))
	{
		for (auto pair : resource->GetTextureMap())
		{
			ITexture* texture = TextureSystem::Get()->GetTexture(pair.second);
			if (!texture && resource->GetTexture(pair.first))
				texture = new Texture(resource->GetTexture(pair.first));
			mTextures_[pair.first] = texture;
		}
	}

	Material::~Material()
	{
		SafeDestroy(mShader_);
		SafeDestroy(mParams_);
	}

	String Material::GetShaderPath(EShaderStage shaderType)
	{
		HYBRID_CHECK(mShader_);
		return mShader_->GetShaderPath(shaderType);
	}

}

