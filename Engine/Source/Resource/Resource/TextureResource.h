#pragma once
#include "Core/Config.h"
#include "ResourceObject.h"
#include "Interface/ITexture.h"
#include "Graphics/RHI/IRenderHeader.h"


namespace Eggy
{
	class TextureResource : public ResourceObject
	{
		DeclareResource(TextureResource, EResourceType::Texture);
	public:
		bool LoadObject() noexcept override;
		bool SaveObject() noexcept override;

		~TextureResource()
		{
			SafeDestroyArray(RawData)
		}

		byte* GetData() const
		{
			HYBRID_CHECK(RawData);
			return RawData;
		}

		const TextureInfo& GetInfo() const { return mInfo_; }
	protected:
		byte* RawData{ nullptr };
		TextureInfo mInfo_;
	};
}
