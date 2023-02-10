#include "IRenderPass.h"


namespace Eggy
{
	size_t RenderPass::GetOutput()
	{
		HYBRID_CHECK(!mOutputRTs.empty());
		return mOutputRTs[0];
	}

	void RenderPass::SetOutput(RenderGraphBuilder* builder, RenderTargetDesc desc, uint8 rtIndex)
	{

	}

	void RenderPass::AddDrawCall(DrawCall* drawCall)
	{
		if (!DrawCallHead)
		{
			DrawCallHead = DrawCallLast = drawCall;
			return;
		}
		DrawCallLast->Next_ = drawCall;
		DrawCallLast = drawCall;
		bIsConsolidated = false;
	}

	void RenderPass::Clear()
	{
		mInputPasses.clear();
		while (DrawCallHead)
		{
			DrawCall* dp = DrawCallHead;
			DrawCallHead = DrawCallHead->Next_;
			delete dp;
		}
		DrawCallHead = nullptr;
		DrawCallLast = nullptr;
		bIsConsolidated = false;
	}

	void RenderPass::Consolidate()
	{
		if (bIsConsolidated)
			return;
		DrawCall* dp = DrawCallHead;
		while (dp)
		{
			uint8 globalConstantSlot = dp->ShaderCollection_->GetConstantSlot(EShaderConstant::Global);
			if (globalConstantSlot == IShaderCollection::INVALID_SLOT || globalConstantSlot >= dp->ResourceBinding_->Buffers)
			{
				dp = dp->Next_;
				continue;
			}
			dp->ResourceBinding_->Data[globalConstantSlot] = new IConstantBuffer();
			auto& globalConstant = Pipeline->GetGlobalConstant();
			IConstantBuffer* globalConstantBuffer = static_cast<IConstantBuffer*>(dp->ResourceBinding_->Data[globalConstantSlot]);
			globalConstantBuffer->Data = &globalConstant;
			globalConstantBuffer->ByteWidth = sizeof(globalConstant);
			globalConstantBuffer->Count = 1;
			dp = dp->Next_;
		}
	}

}