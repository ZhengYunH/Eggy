#pragma once
#include "Core/Config.h"
#include "Core/Interface/IRenderMesh.h"
#include "Graphics/Elements/RenderElement.h"


namespace Eggy
{
	class RenderMeshElement : public RenderElement<EVF_P3F_N3F_T2F, uint32>
	{
	public:
		RenderMeshElement(IMeshData* md)
			: mMeshData_(md)
		{
		}

		void GetVertexDesc(List<IInputLayout::InputElementDesc>& Descs) override;

	protected:
		IMeshData* mMeshData_{ nullptr };

	};


	class RenderMesh : public IRenderMesh
	{
	public:
		RenderMesh() {}

		RenderMeshElement* GetRenderElement(size_t index) noexcept override;

		size_t GetElementsSize() noexcept override;

		void Deserialize(List<struct IMeshData*>& MeshData) override;

	protected:
		List<RenderMeshElement*> mRenderElements_;
	};
}