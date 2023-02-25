#pragma once
#include "Core/Config.h"
#include "Resource/ResourceObject.h"


namespace Eggy
{
	class ShaderResource : public ResourceObject
	{
		DeclareResource(ShaderResource, EResourceType::Shader);
	public:
		bool LoadObject() noexcept override;
		bool SaveObject() noexcept override;

		String GetShaderName() const { return mShaderName_; }

	protected:
		String mShaderName_;
		String mMD5_;
	};
}
