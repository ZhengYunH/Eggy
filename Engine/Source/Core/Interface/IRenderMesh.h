#pragma once
#include "Core/Config.h"
#include "IRenderElements.h"

namespace Eggy
{
	class IRenderMesh
	{
	public:
		virtual size_t GetElementsSize() noexcept = 0;
		virtual IRenderElements* GetRenderElement(size_t index) noexcept = 0;
	};
}