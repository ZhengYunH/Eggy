#include "RenderMesh.h"
#include "Core/Engine/Resource/Mesh.h"


namespace Eggy
{
	RenderMeshElement* RenderMesh::GetRenderElement(size_t index) noexcept
	{
		HYBRID_CHECK(index < mRenderElements_.size());
		return mRenderElements_[index];
	}

	size_t RenderMesh::GetElementsSize() noexcept
	{
		return mRenderElements_.size();
	}

	void RenderMesh::Deserialize(class MeshResource* Resource)
	{
		for(IMeshData* meshData : Resource->GetGeometrys())
		{
			mRenderElements_.push_back(new RenderMeshElement());
			RenderMeshElement* ele = *(mRenderElements_.cbegin());

			auto& vertexInfo = ele->vertexInfo;
			vertexInfo.Count = meshData->GetVertexData(vertexInfo.Data);
			vertexInfo.Stride = meshData->GetVertexStride();

			auto& indexInfo = ele->indexInfo;
			indexInfo.Count = meshData->GetIndexData(indexInfo.Data);
			indexInfo.Stride = meshData->GetIndexStride();
		}
	}

}
