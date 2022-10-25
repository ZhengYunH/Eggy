#pragma once
#include "Core/Config.h"
#include "Graphics/RHI/IRenderDevice.h"


namespace Eggy
{
	class IRenderElements
	{
	public:
		virtual void GetVertexData(void*& Data, size_t& DataSize, size_t& Stride) = 0;
		virtual void CreateResource(IRenderResourceFactory* factory) {}
	};
}