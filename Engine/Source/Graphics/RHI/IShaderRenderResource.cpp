#include "IShaderRenderResource.h"
#include "Shader/ShaderCollection.h"


namespace Eggy
{
	ShaderRenderResource::ShaderRenderResource(class ShaderStageInstance* instance)
		: Stage(instance->_Stage)
		, EntryPoint(instance->_Entry)
		, FilePath(instance->_ShaderPath)
	{
		Samplers.resize(instance->_MaxSamplerStateBinding, nullptr);
		for(uint8 i = 0; i < instance->_MaxSamplerStateBinding; ++i)
			Samplers[i] = new SamplerState();
	}

	String ShaderRenderResource::GetAbsShaderFilePath(String shaderPath)
	{
		return (FileSystem::Get()->GetShaderRoot() + shaderPath).ToString();
	}

	String ShaderRenderResource::GetCacheFilePath(String path)
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

