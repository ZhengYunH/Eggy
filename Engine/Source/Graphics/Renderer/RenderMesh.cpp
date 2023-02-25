#include "RenderMesh.h"
#include "Resource/MeshResource.h"


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
			RenderMeshElement* element = new RenderMeshElement();
			{
				auto& vertexInfo = element->vertexInfo;
				vertexInfo.Count = meshData->GetVertexData(vertexInfo.Data);
				vertexInfo.Stride = meshData->GetVertexStride();

				auto& indexInfo = element->indexInfo;
				indexInfo.Count = meshData->GetIndexData(indexInfo.Data);
				indexInfo.Stride = meshData->GetIndexStride();
			}
			element->Initialize();
			mRenderElements_.push_back(element);
		}
	}

}
