#pragma once
#include "IRenderResource.h"


namespace Eggy
{
	class Texture;
	struct IShaderCollection;
	
	class IShadingState
	{
	public:
		List<Texture*> BindingTextures;
		IShaderCollection* ShaderCollection;
	};
}
