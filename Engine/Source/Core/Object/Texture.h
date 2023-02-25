#pragma once
#include "Config.h"
#include "Object/IObject.h"
#include "Resource/TextureResource.h"


namespace Eggy
{
	class Texture : public IObject, public ITexture
	{
	public:
		Texture() = default;
		Texture(TSharedPtr<TextureResource> resource);
		byte* GetData() override;
		const TextureInfo& GetInfo() override;

	protected:
		TSharedPtr<TextureResource> mResource_;
		TextureInfo mInfo_;

	};
}