#pragma once
#include "Core/Config.h"
#include "Resource/ResourceObject.h"

namespace Eggy
{
	struct TextureResource : public ResourceObject
	{
		TextureResource(String path)
		{
			Path = path;
		}

		void Deserialize(IFile* file) noexcept override
		{
			// test, RGB
			Size = Vector3U(256, 256, 1);
			Mips = 1;
			Format = EFormat::R8G8B8A8;
			ByteWidth = GetFormatInfo(Format).DataSize * Size.x * Size.y;
			RawData = new byte[ByteWidth];
			TextureType = ETextureType::Texture2D;
			memset(RawData, 255, ByteWidth);
		}

		~TextureResource()
		{
			delete[] RawData;
			RawData = nullptr;
		}

		byte* GetData() const
		{
			HYBRID_CHECK(RawData);
			return RawData;
		}

		byte* RawData{ nullptr };
		Vector3U Size;
		uint8 Mips;
		String Path;
		EFormat Format;
		uint32 ByteWidth;
		ETextureType TextureType;
	};

	class Texture
	{
	public:
		void SetResource(TextureResource* Resource)
		{
			mResource_ = Resource;
		}

		TextureResource& GetResource()
		{
			HYBRID_CHECK(mResource_);
			return *mResource_;
		}

		TextureResource* mResource_;
	};
}
