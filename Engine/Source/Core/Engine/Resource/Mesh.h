#pragma once
#include "Core/Config.h"
#include "ResourceObject.h"

namespace Eggy
{
	struct IMeshData
	{
		virtual size_t GetVertexData(void*& Data) = 0;
		virtual size_t GetIndexData(void*& Data) = 0;

		virtual size_t GetVertexStride() = 0;
		virtual size_t GetIndexStride() = 0;

		virtual void GetVertexDesc(List<IInputLayout::InputElementDesc>& Descs) = 0;
	};

	class MeshResource : public ResourceObject
	{
	public:
		void Deserialize(IFile* file) noexcept override
		{
		}

		IMeshData* mGeometry_;
	};
}

