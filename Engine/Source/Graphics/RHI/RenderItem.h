#pragma once
#include "IRenderResource.h"
#include "Core/Interface/IRenderObject.h"
#include "Core/Math/Matrix4x4.h"
#include "Graphics/RHI/IShadingState.h"


namespace Eggy
{
	struct BatchConstant
	{
		Matrix4x4 ModelTransform;
	};

	struct ShaderConstant
	{

	};

	struct GlobalConstant
	{
		Matrix4x4 ViewTransform;
		Matrix4x4 ProjectTransform;
		Vector4 Color{ 1.f,1.f,1.f,1.f };
	};

	struct RenderObject : public IRenderObject
	{
		BatchConstant ObjectConstantData_;
	};

	class RenderItemInfo
	{
	public:
		RenderObject* Object{ nullptr };
		GeometryBinding* GeometryBinding_{ nullptr };
		IShadingState* ShadingState_{ nullptr };
	};

	class RenderItem
	{
	public:
		RenderItemInfo* Info{ nullptr };
	};
}

