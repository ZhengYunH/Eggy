#include "ShaderResource.h"
#include "ResourceItem.h"
#include "System/FileSystem.h"
#include "Graphics/RHI/Shader/ShaderReflection.h"


namespace Eggy
{
	DefineResource(ShaderResource, EResourceType::Shader);

	bool ShaderResource::LoadObject() noexcept
	{
		FPath root = FileSystem::Get()->GetPackageRoot() + GetItem()->GetPath();
		FPath metaFilePath = root + "meta.xml";
		XMLFile* metaFile = dynamic_cast<XMLFile*>(FileSystem::Get()->LoadFile(metaFilePath.ToString()).get());
		if (!metaFile)
			return false;
		auto _Shader = metaFile->GetRootNode().get_child("Shader");
		mShaderName_ = _Shader.get<String>("Source");
		// mMD5_ = _Shader.get<String>("MD5");
		SetLoaded();
		return true;
	}

	bool ShaderResource::SaveObject() noexcept
	{
		Unimplement();
		return false;
	}

}

