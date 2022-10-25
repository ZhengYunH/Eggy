#pragma once
#include "Core/Config.h"
#include "Core/Interface/IRenderMesh.h"
#include "Graphics/Elements/RenderElements.h"


namespace Eggy
{
	class RenderMeshElement : public RenderElements
	{
	public:
		RenderMeshElement() 
		{
		}

	protected:
		String mResource_;
	};


	class RenderTriangleMesh : public IRenderMesh
	{
	public:
		size_t GetElementsSize() noexcept override
		{
			return 1;
		}

		IRenderElements* GetRenderElement(size_t index) noexcept override
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

	protected:
		List<RenderMeshElement*> mRenderElements_;
	};
}