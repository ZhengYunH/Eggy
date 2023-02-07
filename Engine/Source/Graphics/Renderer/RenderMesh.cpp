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
		mRenderElements_.push_back(new RenderMeshElement());
		RenderMeshElement* ele = *(mRenderElements_.cbegin());
		
		{
			auto& vertexInfo = ele->vertexInfo;
			vertexInfo.Count = Resource->mGeometry_->GetVertexData(vertexInfo.Data);
			vertexInfo.Stride = Resource->mGeometry_->GetVertexStride();
		}

		{
			auto& indexInfo = ele->indexInfo;
			indexInfo.Count = Resource->mGeometry_->GetIndexData(indexInfo.Data);
			indexInfo.Stride = Resource->mGeometry_->GetIndexStride();
		}

		ele->Consolidate();
	}

}
