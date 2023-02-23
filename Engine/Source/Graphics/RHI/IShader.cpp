#include "IShader.h"
#include "Core/System/FileSystem.h"


namespace Eggy
{
	String IShader::GetAbsShaderFilePath(String shaderPath)
	{
		return (FileSystem::Get()->GetShaderRoot() + shaderPath).ToString();
	}

	String IShader::GetCacheFilePath(String path)
	{
		FPath srcPath(path);
		srcPath.InsertDirectory(FileSystem::Get()->GetCacheDirectory());

#if D3D11_DEVICE
		srcPath.replaceFilePostfix(".cso");
#elif VULKAN_DEVICE
#endif
		return srcPath.ToString();
	}
}

