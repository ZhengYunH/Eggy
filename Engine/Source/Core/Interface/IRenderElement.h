#pragma once
#include "Core/Config.h"
#include "Core/Object/Mesh.h"
#include "Core/Object/Material.h"
#include "Core/Math/Matrix4x4.h"


namespace Eggy
{
	struct ObjectInfo
	{
		Matrix4x4 ModelTransform;
		Matrix4x4 ViewTransform;
		Matrix4x4 ProjectTransform;
	};

	struct IRenderElement
	{
		IRenderElement() = default;

		virtual void GetVertexData(void*& Data, size_t& VertexCount, size_t& ByteWidth) = 0;
		virtual void GetIndexData(void*& Data, size_t& IndexCount, size_t& ByteWidth) = 0;

		virtual void GetVertexDesc(List<IInputLayout::InputElementDesc>& Descs) = 0;

		Mesh* mMesh{ nullptr };
		Material* mMaterial{ nullptr };
		ObjectInfo mObjectInfo;
	};
}