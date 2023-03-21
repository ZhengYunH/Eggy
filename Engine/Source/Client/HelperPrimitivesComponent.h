#pragma once
#include "PrimitivesComponent.h"
#include "Object/Mesh.h"


namespace Eggy
{	
	const char* HELPER_PRIMITIVES_SHADER = "Helper";

	template<EHelperMesh _T>
	class HelperPrimitvesComponent : public PrimitiveComponent
	{
	public:
		HelperPrimitvesComponent()
		{
			List<IMeshData*> meshData;
			meshData.push_back(GetHelperMesh<_T>());
			mModel_.SetMesh(new Mesh(meshData));
			Material* mat = new Material(new Shader(HELPER_PRIMITIVES_SHADER));
			mat->SetRenderSet(ERenderSets(ERenderSet::Helper));
			mModel_.SetMaterial(mat);
		}
	};
}
