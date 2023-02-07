#pragma once
#include "Core/Config.h"
#include "IRenderElement.h"


namespace Eggy
{
	class IRenderMesh
	{
	public:
		virtual size_t GetElementsSize() noexcept = 0;
		virtual IRenderElement* GetRenderElement(size_t index) noexcept = 0;
		virtual void Deserialize(class MeshResource* Resource) = 0;
	};
}