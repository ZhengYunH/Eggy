#pragma once
#include <stb_image.h>
#include "Core/File/File.h"
#include "Graphics/RHI/IRenderHeader.h"
#include "Core/Interface/ITexture.h"

namespace Eggy
{
	class TextureLoader
	{
		static uint8 EFormat2STBIFormat(EFormat format)
		{
			static Map<EFormat, uint8> STBIFormat{
				{ EFormat::R32_UINT, STBI_grey },
				{ EFormat::R32G32F, STBI_grey_alpha },
				{ EFormat::R32G32B32, STBI_rgb },
				{ EFormat::R8G8B8A8, STBI_rgb_alpha },
			};
			auto itr = STBIFormat.find(format);
			if (itr != STBIFormat.end())
				return itr->second;
			return STBI_default;
		}

	public:
		TextureLoader(FPath filePath, EFormat format)
		{
			int width, height, channels;
			mData_ = stbi_load(filePath.ToString().c_str(), &width, &height, &channels, EFormat2STBIFormat(format));
			mInfo_.Size = Vector3U(width, height, 1);
			mInfo_.Format = format;
			mInfo_.Mips = 1;
			mInfo_.ByteWidth = width * height * GetFormatInfo(format).DataSize;
			mInfo_.TextureType = ETextureType::Texture2D;
			mInfo_.Path = filePath.ToString();
		}

		TextureLoader(const byte* src, uint32 nSrc, EFormat format)
		{
			int width, height, channels;
			mData_ = stbi_load_from_memory(src, nSrc, &width, &height, &channels, EFormat2STBIFormat(format));
			mInfo_.Size = Vector3U(width, height, 1);
			mInfo_.Format = format;
			mInfo_.Mips = 1;
			mInfo_.ByteWidth = width * height * GetFormatInfo(format).DataSize;
			mInfo_.TextureType = ETextureType::Texture2D;
		}

	public:
		byte* GetData() { return mData_; }

		const TextureInfo& GetInfo() const { return mInfo_; }

	protected:
		TextureInfo mInfo_;
		byte* mData_;
	};
}