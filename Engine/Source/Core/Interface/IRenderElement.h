#pragma once
#include "Core/Config.h"
#include "Graphics/RHI/IRenderDevice.h"
#include "Graphics/RHI/IRenderHeader.h"


namespace Eggy
{
	class IRenderElement
	{
	public:
		virtual void Initialize() {}
		virtual void GetVertexData(void*& Data, size_t& VertexCount, size_t& ByteWidth) = 0;
		virtual void GetIndexData(void*& Data, size_t& IndexCount, size_t& ByteWidth) = 0;
		virtual void CreateResource(IRenderResourceFactory* factory) {}
	};
}