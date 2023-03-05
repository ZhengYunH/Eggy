#pragma once
#include "Config.h"
#include "Object/IObject.h"
#include "Resource/ShaderResource.h"
#include "Graphics/RHI/Shader/ShaderCollection.h"
#include "Core/System/RenderSystem.h"


namespace Eggy
{
	class Shader : IObject
	{
	public:
		Shader(String shaderPath)
		{
			if (RenderSystem::Get()->IsDeferredShading() && shaderPath == "Basic")
				shaderPath = "PBR";
			mShaderPath_ = shaderPath;
			mShaderCollection_ = ShaderCollectionFactory::Instance().GetCollection(mShaderPath_);
		}

		Shader(TSharedPtr<ShaderResource> resource)
			: Shader(resource->GetShaderName())
		{
		}

	public:
		const ShaderCollection* GetShaderCollection() const { return mShaderCollection_; }

	protected:
		String mShaderPath_;
		const ShaderCollection* mShaderCollection_;
	};
}