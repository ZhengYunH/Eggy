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
		}

		String mShader_;
	};
}

