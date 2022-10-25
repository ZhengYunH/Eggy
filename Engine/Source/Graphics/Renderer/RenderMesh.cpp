#include "RenderMesh.h"


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
}
