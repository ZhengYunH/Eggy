#pragma once
#include "Core/Config.h"
#include "Resource/ResourceObject.h"
#include "ShaderResource.h"
#include "TextureResource.h"


namespace Eggy
{
	class MaterialResource : public ResourceObject
	{
		DeclareResource(MaterialResource, EResourceType::Material);
	public:
		bool LoadObject() noexcept override;
		bool SaveObject() noexcept override;
		void AddDependency(TSharedPtr<ResourceObject>& dep) override;

		const TSharedPtr<ShaderResource>& GetShader() { return mShader_; }
		const Map<String, Guid>& GetTextureMap() const { return mTextureMap_; }
		TSharedPtr<TextureResource> GetTexture(String name);

	protected:
		Map<String, Guid> mTextureMap_;
		Map<String, TSharedPtr<TextureResource>> mTextureResourceMap_;
		List<TSharedPtr<TextureResource>> mTextures_;
		TSharedPtr<ShaderResource> mShader_;
	};
}

