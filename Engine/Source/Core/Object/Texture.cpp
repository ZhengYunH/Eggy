#include "Texture.h"
#include "Resource/ResourceItem.h"
#include "System/TextureSystem.h"

namespace Eggy
{
	Texture::Texture(TSharedPtr<TextureResource> resource)
	{
		TextureSystem::Get()->AddTexture(resource->GetItem()->GetResourceID(), this);
		mResource_ = resource;
		mInfo_ = resource->GetInfo();
	}

	byte* Texture::GetData()
	{
		return mResource_->GetData();
	}

	const TextureInfo& Texture::GetInfo()
	{
		return mInfo_;
	}

}
