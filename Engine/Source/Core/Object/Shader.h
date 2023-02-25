#pragma once
#include "Config.h"
#include "Object/IObject.h"
#include "Resource/ShaderResource.h"
#include "Graphics/RHI/IRenderHeader.h"


namespace Eggy
{
	class Shader : IObject
	{
	public:
		Shader() = default;
		Shader(String shaderPath) : mShaderPath_(shaderPath)
		{

		}
		Shader(TSharedPtr<ShaderResource> resource)
		{
			mShaderPath_ = resource->GetShaderName();
		}

	public:
		String GetShaderPath(EShaderType shaderType);

	protected:
		String mShaderPath_;

		TSharedPtr<ShaderResource> mResource_;
	};
}