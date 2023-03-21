#include "RenderMesh.h"
#include "Resource/MeshResource.h"


namespace Eggy
{
	void RenderMeshElement::GetVertexDesc(List<IInputLayout::InputElementDesc>& Descs)
	{
		HYBRID_CHECK(mMeshData_);
		mMeshData_->GetVertexDesc(Descs);
	}

	RenderMeshElement* RenderMesh::GetRenderElement(size_t index) noexcept
	{
		HYBRID_CHECK(index < mRenderElements_.size());
		return mRenderElements_[index];
	}

	size_t RenderMesh::GetElementsSize() noexcept
	{
		return mRenderElements_.size();
	}

	void RenderMesh::Deserialize(List<IMeshData*>& MeshData)
	{
		for(IMeshData* meshData : MeshData)
		{
			RenderMeshElement* element = new RenderMeshElement(meshData);
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
