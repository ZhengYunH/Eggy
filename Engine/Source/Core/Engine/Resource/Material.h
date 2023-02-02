#pragma once
#include "Core/Config.h"
#include "ResourceObject.h"

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

		String mShader_;
		List<String> mTexturePaths_;
	};
}

