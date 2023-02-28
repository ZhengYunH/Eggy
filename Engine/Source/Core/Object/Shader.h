#pragma once
#include "Config.h"
#include "Object/IObject.h"
#include "Resource/ShaderResource.h"
#include "Graphics/RHI/IRenderHeader.h"
#include "Graphics/RHI/Shader/ShaderCollection.h"


namespace Eggy
{
	class Shader : IObject
	{
	public:
		Shader(String shaderPath) 
			: mShaderPath_(shaderPath)
			, mShaderCollection_(ShaderCollectionFactory::Instance().GetCollection(shaderPath))
		{
		}
		Shader(TSharedPtr<ShaderResource> resource)
			: mShaderPath_(resource->GetShaderName())
			, mShaderCollection_(ShaderCollectionFactory::Instance().GetCollection(resource->GetShaderName()))
		{
		}
	public:
		String GetShaderPath(EShaderType shaderType);

	protected:
		String mShaderPath_;
		const ShaderCollection* mShaderCollection_;
	};
}