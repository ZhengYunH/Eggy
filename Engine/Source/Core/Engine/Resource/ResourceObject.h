#pragma once
#include "Core/Config.h"
#include "Graphics/Renderer/VertexFactory.h"

namespace Eggy
{
	class FileSystem;
	class IFile;

	class ResourceObject
	{
	public:
		virtual void Deserialize(IFile* file) noexcept = 0;
	};

	class MeshResourceObject : public ResourceObject
	{
	public:
		virtual void Deserialize(IFile* file) noexcept override;
		virtual void GetData(uint16 index, void* VertexData, List<uint32>& mIndexData) noexcept;

	protected:
		void* mData_;
		EVertexFormat mVertexFormat_;
		size_t mVertexStride_;
		List<List<uint32>> mIndexs_;
	};
}