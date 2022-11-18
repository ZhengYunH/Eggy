#pragma once
#include "Core/Config.h"
#include "IRenderObject.h"


namespace Eggy
{
	class IRenderMesh
	{
	public:
		virtual size_t GetElementsSize() noexcept = 0;
		virtual IRenderObject* GetRenderElement(size_t index) noexcept = 0;
	};
}