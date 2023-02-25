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
		{
			mResource_ = resource;
			mRenderMesh_ = new RenderMesh();
			mRenderMesh_->Deserialize(resource.Get());
		}
		IRenderMesh* GetRenderMesh() { return mRenderMesh_; }

	protected:
		TSharedPtr<MeshResource> mResource_;
		IRenderMesh* mRenderMesh_{ nullptr };
	};
}