#pragma once
#include "Core/Config.h"
#include "Object/IObject.h"
#include "Resource/MeshResource.h"
#include "Graphics/Renderer/VertexFactory.h"
#include "Graphics/Renderer/RenderMesh.h"


namespace Eggy
{
	class Mesh : public IObject
	{
	public:
		Mesh() {}
		Mesh(TSharedPtr<MeshResource> resource)
			: Mesh(resource->GetGeometrys())
		{
		}

		Mesh(List<IMeshData*>& MeshData)
		{
			mRenderMesh_ = new RenderMesh();
			mRenderMesh_->Deserialize(MeshData);
		}

		IRenderMesh* GetRenderMesh() { return mRenderMesh_; }

	protected:
		IRenderMesh* mRenderMesh_{ nullptr };
	};
}