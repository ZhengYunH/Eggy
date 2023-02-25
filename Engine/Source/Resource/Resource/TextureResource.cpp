#include "TextureResource.h"
#include "ResourceItem.h"
#include "System/FileSystem.h"


namespace Eggy
{
	DefineResource(TextureResource, EResourceType::Texture);

	bool TextureResource::LoadObject() noexcept
	{
		// test, RGB
		mInfo_.Size = Vector3U(256, 256, 1);
		mInfo_.Mips = 1;
		mInfo_.Format = EFormat::R8G8B8A8;
		mInfo_.ByteWidth = GetFormatInfo(mInfo_.Format).DataSize * mInfo_.Size.x * mInfo_.Size.y;
		RawData = new byte[mInfo_.ByteWidth];
		mInfo_.TextureType = ETextureType::Texture2D;
		memset(RawData, 255, mInfo_.ByteWidth);

		FPath root = FileSystem::Get()->GetPackageRoot() + GetItem()->GetPath();
		FPath metaFilePath = root + "meta.xml";
		XMLFile* metaFile = dynamic_cast<XMLFile*>(FileSystem::Get()->LoadFile(metaFilePath.ToString()).get());
		if (!metaFile)
			return false;
		auto _Texture = metaFile->GetRootNode().get_child("Texture");
		String _TextureSource = _Texture.get<String>("Source");
		SetLoaded();
		return true;
	}

	bool TextureResource::SaveObject() noexcept
	{
		Unimplement();
		return false;
	}
}
