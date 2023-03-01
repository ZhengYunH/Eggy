#pragma once
#include "Interface/IRenderObject.h"
#include "Math/Matrix4x4.h"


namespace Eggy
{
	struct RenderObject : public IRenderObject
	{
		void PrepareBatchData(class IShadingBatch* batch) override;

		Matrix4x4 ModelTransform;
	};
}


