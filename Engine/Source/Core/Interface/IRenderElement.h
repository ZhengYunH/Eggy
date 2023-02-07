#pragma once
#include "Core/Config.h"
#include "Graphics/RHI/IRenderResource.h"


namespace Eggy
{
	struct IRenderElement
	{
		IRenderElement() = default;

		virtual void GetVertexData(void*& Data, size_t& VertexCount, size_t& ByteWidth) = 0;
		virtual void GetIndexData(void*& Data, size_t& IndexCount, size_t& ByteWidth) = 0;

		virtual void GetVertexDesc(List<IInputLayout::InputElementDesc>& Descs) = 0;
		virtual void PrepareRenderItemInfo(class RenderContext* context, class RenderItemInfo* info) = 0;
	};
}