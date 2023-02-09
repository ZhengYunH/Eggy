#pragma once
#include "Core/Config.h"
#include "Core/Interface/IRenderMesh.h"
#include "Graphics/Elements/RenderElement.h"


namespace Eggy
{
	class RenderMeshElement : public RenderElement<EVF_P3F_C4B_T2F, uint32>
	{
	public:
		RenderMeshElement() 
		{
		}

	protected:
	};


	class RenderTriangleMesh : public IRenderMesh
	{
	public:
		size_t GetElementsSize() noexcept override
		{
			return 1;
		}

		IRenderElement* GetRenderElement(size_t index) noexcept override
		{
			return &mElement_;
		}

	protected:
		RenderTriangleElement mElement_;
	};

	class RenderMesh : public IRenderMesh
	{
	public:
		RenderMesh() {}

		RenderMeshElement* GetRenderElement(size_t index) noexcept override;

		size_t GetElementsSize() noexcept override;

		void Deserialize(class MeshResource* Resource) override;

	protected:
		List<RenderMeshElement*> mRenderElements_;
	};
}