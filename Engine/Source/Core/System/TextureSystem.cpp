#include "TextureSystem.h"


namespace Eggy
{
	DefineSystem(TextureSystem);
	DefineDependency(TextureSystem, RenderSystem);

	void TextureSystem::Initialize()
	{
	}

	void TextureSystem::Finalize()
	{
	}

	void TextureSystem::AddTexture(const Guid& guid, ITexture* texture)
	{
		HYBRID_CHECK(mTextureCache_.find(guid) == mTextureCache_.end());
		mTextureCache_.insert({ guid, texture });
	}

	ITexture* TextureSystem::GetTexture(const Guid& guid)
	{
		auto itr = mTextureCache_.find(guid);
		if (itr != mTextureCache_.end())
			return itr->second;
		return nullptr;
	}

	
}

