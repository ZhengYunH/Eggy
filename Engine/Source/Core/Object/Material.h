#pragma once
#include "Core/Config.h"
#include "Core/Object/IObject.h"
#include "Resource/MaterialResource.h"
#include "Resource/TextureResource.h"


namespace Eggy
{
	class Shader;
	class Texture;
	class Material : public IObject
	{
	public:
		Material() = default;
		Material(Shader* shader) 
			: mShader_(shader)
		{
		}

		Material(TSharedPtr<MaterialResource> resource);
		~Material();

		Map<String, ITexture*>& GetTextures() { return mTextures_; }
		String GetShaderPath(EShaderType shaderType);

	protected:
		TSharedPtr<MaterialResource> mResource_;

		Shader* mShader_{ nullptr };
		Map<String, ITexture*> mTextures_;
	};
}