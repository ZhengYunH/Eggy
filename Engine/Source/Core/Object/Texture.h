#pragma once
#include "Config.h"
#include "Object/IObject.h"
#include "Core/Interface/ITexture.h"
#include "Resource/TextureResource.h"


namespace Eggy
{
	struct ITextureBuffer;
	class Texture : public IObject, public ITexture
	{
	public:
		Texture() = default;
		
		Texture(TSharedPtr<TextureResource> resource);
		
		virtual ~Texture();

	public:
		byte* GetData() override { return mData_; }

		const TextureInfo& GetInfo() override { return mInfo_; }

		ITextureBuffer* GetRenderTexture() override { return mRenderTexture_; }

	protected:
		virtual void PrepareRenderResource();

	protected:
		TextureInfo mInfo_;
		byte* mData_{ nullptr };
		ITextureBuffer* mRenderTexture_{ nullptr };
	};
}