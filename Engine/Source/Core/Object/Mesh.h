#pragma once
#include "Core/Config.h"
#include "Core/Engine/Resource/Mesh.h"
#include "Graphics/Renderer/VertexFactory.h"
#include "Graphics/Renderer/RenderMesh.h"


namespace Eggy
{
	class Mesh
	{
	public:
		void SetResource(MeshResource* Resource)
		{
			mRenderMesh_ = new RenderMesh();
			mRenderMesh_->Deserialize(Resource);
		}

		IRenderMesh* GetRenderMesh() { return mRenderMesh_; }

	protected:
		IRenderMesh* mRenderMesh_{ nullptr };
	};
}