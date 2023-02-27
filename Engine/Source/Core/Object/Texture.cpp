#include "Texture.h"
#include "Resource/ResourceItem.h"
#include "System/TextureSystem.h"
#include "Graphics/RHI/IRenderResource.h"
#include "Resource/TextureResource.h"


namespace Eggy
{
	Texture::Texture(TSharedPtr<TextureResource> resource)
	{
		TextureSystem::Get()->AddTexture(resource->GetItem()->GetResourceID(), this);
		mInfo_ = resource->GetInfo();
		mData_ = new byte[mInfo_.ByteWidth];
		memcpy(mData_, resource->GetData(), mInfo_.ByteWidth);
		PrepareRenderResource();
	}

	Texture::~Texture()
	{
		SafeDestroyArray(mData_);
	}

	void Texture::PrepareRenderResource()
	{
		HYBRID_CHECK(GetData());
		SafeDestroy(mRenderTexture_);
		mRenderTexture_ = new ITextureBuffer();

		mRenderTexture_->Width = mInfo_.Size.x;
		mRenderTexture_->Height = mInfo_.Size.y;
		mRenderTexture_->Mips = mInfo_.Mips;
		mRenderTexture_->TextureType = mInfo_.TextureType;
		mRenderTexture_->Format = mInfo_.Format;
		mRenderTexture_->Data = mData_;
	}

}
