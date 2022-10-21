#pragma once
#include "Core/Config.h"
#include "Core/Interface/IRenderMesh.h"
#include "Graphics/Elements/RenderElements.h"


namespace Eggy
{
	struct Vertex
	{
		
	};

	struct MeshObject
	{
		void LoadResource(String resource)
		{
			ResourceState = true;
		}

		bool ResourceState{ false };
		List<Vertex> VertexData;
		List<uint32> IndexData;
	};

	class RenderMeshElement : public RenderElements
	{
	public:
		RenderMeshElement() 
		{
		}

	protected:
		String mResource_;
	};

	class RenderMesh : public IRenderMesh
	{
	public:
		RenderMesh();
		
		RenderMeshElement* GetRenderElement(size_t index) noexcept override;

		size_t GetElementsSize() noexcept override;

	protected:
		List<RenderMeshElement*> mRenderElements_;
	};
}