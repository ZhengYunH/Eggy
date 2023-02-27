#include "TextureResource.h"
#include "ResourceItem.h"
#include "System/FileSystem.h"
#include "Core/File/TextureLoader.h"


namespace Eggy
{
	DefineResource(TextureResource, EResourceType::Texture);

	bool TextureResource::LoadObject() noexcept
	{
		

		FPath root = FileSystem::Get()->GetPackageRoot() + GetItem()->GetPath();
		FPath metaFilePath = root + "meta.xml";
		XMLFile* metaFile = dynamic_cast<XMLFile*>(FileSystem::Get()->LoadFile(metaFilePath.ToString()).get());
		if (!metaFile)
			return false;
		auto _Texture = metaFile->GetRootNode().get_child("Texture");
		String _TextureSource = _Texture.get<String>("Source");
		TextureLoader textureLoader((root + _TextureSource).ToString(), EFormat::R8G8B8A8);
		mInfo_ = textureLoader.GetInfo();
		RawData = textureLoader.GetData();

		SetLoaded();
		return true;
	}

	bool TextureResource::SaveObject() noexcept
	{
		Unimplement();
		return false;
	}
}
