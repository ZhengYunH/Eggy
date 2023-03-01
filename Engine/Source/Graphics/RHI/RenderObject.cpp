#include "RenderObject.h"
#include "Graphics/RHI/IRenderHeader.h"
#include "Graphics/RHI/IShadingBatch.h"


namespace Eggy
{
	void RenderObject::PrepareBatchData(IShadingBatch* batch)
	{
		auto cb = batch->GetConstantBuffer(EShaderConstant::Batch);

		cb->SetMatrix4x4("cWorld", ModelTransform);
	}

}