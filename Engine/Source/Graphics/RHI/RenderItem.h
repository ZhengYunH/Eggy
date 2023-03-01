#pragma once
#include "IRenderResource.h"
#include "Core/Math/Matrix4x4.h"
#include "Graphics/RHI/IShadingState.h"
#include "RenderObject.h"


namespace Eggy
{
	class RenderItemInfo
	{
	public:
		RenderObject* Object{ nullptr };
		GeometryBinding* GeometryBinding_{ nullptr };
		IShadingState* ShadingState_{ nullptr };
		Material* Material_{ nullptr };
	};

	class RenderItem
	{
	public:
		RenderItemInfo* Info{ nullptr };
	};
}

