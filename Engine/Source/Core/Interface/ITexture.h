#pragma once
#include "Config.h"
#include "Math/Vector3.h"
#include "Graphics/RHI/IRenderHeader.h"


namespace Eggy
{
	struct TextureInfo
	{
		Vector3U Size;
		uint8 Mips;
		String Path;
		EFormat Format;
		uint32 ByteWidth;
		ETextureType TextureType;
	};

	struct ITextureBuffer;
	class ITexture
	{
	public:
		virtual byte* GetData() = 0;
		virtual const TextureInfo& GetInfo() = 0;
		virtual ITextureBuffer* GetRenderTexture() = 0;
	};
}