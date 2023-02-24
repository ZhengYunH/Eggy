#pragma once
#include "Core/Config.h"
#include "Resource/ResourceObject.h"

namespace Eggy
{
	class MaterialResource : public ResourceObject
	{
	public:
		void Deserialize(IFile* file) noexcept override
		{
			mShader_ = "Basic";
			mTexturePaths_.push_back("Test");
		}

		void Deserialize(String shaderPath)
		{
			mShader_ = shaderPath;
		}

		String mShader_;
		List<String> mTexturePaths_;
	};
}

