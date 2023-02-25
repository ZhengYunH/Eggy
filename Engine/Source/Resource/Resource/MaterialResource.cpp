#include "MaterialResource.h"
#include "System/FileSystem.h"
#include "Misc/Guid.h"
#include "ResourceItem.h"
#include "TextureResource.h"
#include "ShaderResource.h"


namespace Eggy
{
	DefineResource(MaterialResource, EResourceType::Material);
	bool MaterialResource::LoadObject() noexcept
	{
		FPath root = FileSystem::Get()->GetPackageRoot() + GetItem()->GetPath();
		FPath metaFilePath = root + "meta.xml";
		XMLFile* metaFile = dynamic_cast<XMLFile*>(FileSystem::Get()->LoadFile(metaFilePath.ToString()).get());
		if (!metaFile)
			return false;

		auto _Material = metaFile->GetRootNode().get_child("Material");
		Guid _ShaderSource = Guid::FromString(_Material.get<String>("Shader"));
		HYBRID_CHECK(mShader_ && mShader_->GetItem()->GetResourceID() == _ShaderSource);
		BOOST_FOREACH(pt::ptree::value_type & v, _Material.get_child("Parameters"))
		{
			auto param = v.second;
			mTextureMap_[param.get<String>("Key")] = Guid::FromString(param.get<String>("Value"));
		}

		for (auto& pair : mTextureMap_)
		{
			for (auto& textureResource : mTextures_)
			{
				if (textureResource->GetItem()->GetResourceID() == pair.second)
				{
					mTextureResourceMap_[pair.first] = textureResource;
					break;
				}
			}
		}

		SetLoaded();
		return true;
	}

	bool MaterialResource::SaveObject() noexcept
	{
		Unimplement();
		return false;
	}

	void MaterialResource::AddDependency(TSharedPtr<ResourceObject>& dep)
	{
		ResourceObject::AddDependency(dep);
		EResourceType type = dep->GetItem()->GetType();
		if (type == EResourceType::Texture)
			mTextures_.push_back(dep);
		else if (type == EResourceType::Shader)
			mShader_ = dep;
	}

	TSharedPtr<TextureResource> MaterialResource::GetTexture(String name)
	{
		auto itr = mTextureResourceMap_.find(name);
		if (itr != mTextureResourceMap_.end())
			return itr->second;
		return nullptr;
	}

}

