#include "PostProcessRenderer.h"


namespace Eggy
{
	void Postprocess::Consolidate()
	{
		RenderPass::Consolidate();
	}

	void Postprocess::Render(RenderContext* context)
	{
		mRenderElement.PrepareRenderItemInfo(context, &mInfo_);
		RenderItem* item = context->GenerateRenderItem(&mInfo_);
		AddDrawCall(context->GenerateDrawCall(item));
	}

	RenderPass* BlurPostprocess::Connect(RenderPass* input)
	{
		AddInput(input);
		return this;
	}

	void BlurPostprocess::Compile(RenderGraphBuilder* builder)
	{
		SetOutput(builder, GetInput(0)->GetOutputTarget(builder));
		Sample(builder, GetInput(0)->GetOutput());
		mBatch.SetRenderTarget("Base", GetInput(0)->GetOutput());
	}

	RenderPass* TextureToScreen::Connect(RenderPass* input)
	{
		AddInput(input);
		return this;
	}

	void TextureToScreen::Compile(RenderGraphBuilder* builder)
	{
		GetParameters()->SetFloat("Brightness", 0, 1, &Pipeline->GetContext()->GetGlobalConstant().Brightness);

		size_t backBuffer = RenderGraphBuilder::INVALID_RT;
		size_t dsBuffer = RenderGraphBuilder::INVALID_RT;
		builder->GetBackBuffer(backBuffer, dsBuffer);
		SetOutput(builder, backBuffer);
		SetDepthStencil(builder, dsBuffer);

		mBatch.SetRenderTarget("Base", GetInput(0)->GetOutput());
	}

}
