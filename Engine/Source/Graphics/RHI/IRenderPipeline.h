#pragma once
#include "IRenderHeader.h"
#include "IRenderResource.h"
#include "IShader.h"
#include "RenderItem.h"


namespace Eggy
{
	struct ResourceBinding
	{
		uint16 Buffers{ 0 };
		uint16 Textures{ 0 };
		uint16 Views{ 0 };

		// Buffer -> Texture -> Views
		IRenderResource** Data{ nullptr };

		void CreateDeviceResource(IRenderResourceFactory* factory)
		{
			for (uint16 i = 0; i < Buffers; ++i)
			{
				if(!Data[i])
					continue;
				IBuffer* buffer = (IBuffer*)Data[i];
				HYBRID_CHECK(buffer);
				buffer->CreateDeviceResource(factory);
			}

			for (uint16 i = Buffers; i < Buffers + Textures; ++i)
			{
				if (!Data[i])
					continue;
				ITexture* texture = (ITexture*)Data[i];
				HYBRID_CHECK(texture);
				texture->CreateDeviceResource(factory);
			}
		}
	};

	class RenderPipeline;
	class RenderContext;
	class RenderPass;
	class RenderItem;

	struct DrawCall
	{
		RenderItem* Item_{ nullptr };
		DrawCall* Next_{ nullptr };

		PipelineState Pipeline_;
		GeometryBinding* GeometryBinding_{ nullptr };
		ResourceBinding* ResourceBinding_{ nullptr };
		IShaderCollection* ShaderCollection_{ nullptr };

		bool IsAllResourceReady()
		{
			HYBRID_CHECK(GeometryBinding_);
			HYBRID_CHECK(ResourceBinding_);
			HYBRID_CHECK(ShaderCollection_);
			return true;
		}

		void CreateDeviceResource(IRenderResourceFactory* factory)
		{
			HYBRID_CHECK(IsAllResourceReady());
			ShaderCollection_->CreateDeviceResource(factory);
			GeometryBinding_->CreateDeviceResource(factory);
			ResourceBinding_->CreateDeviceResource(factory);
			Pipeline_.CreateDeviceResource(factory);
		}

		~DrawCall();
	};

	class RenderPipeline
	{
	public:
		RenderPipeline() = default;
		virtual ~RenderPipeline();

		void Clear();

		RenderPass* AddRenderPass();
		
		void AddDrawCallChannel(ERenderSets sets, RenderPass* pass);
		void AddDrawCallChannel(ERenderSet set, RenderPass* pass);
		List<RenderPass*>& GetRenderPasses() { return mRenderPasses_; }
		void Consolidate();
		void AddDrawCall(ERenderSet set, DrawCall* dp);
		GlobalConstant& GetGlobalConstant() { return mConstant_; }

	protected:
		List<RenderPass*> mRenderPasses_;
		Map<ERenderSet, List<RenderPass*>> mRenderPassSet_;
		GlobalConstant mConstant_;
		RenderContext* mContext_;
	};

	class RenderContext
	{
	public:
		RenderContext(RenderPipeline* pipeline) : mPipeline_(pipeline)
		{}
		virtual ~RenderContext();

		RenderItem* GenerateRenderItem(RenderItemInfo* info);
		DrawCall* GenerateDrawCall(RenderItem* item);

		void Prepare();
		void Clear();
		void SubmitRenderItem(ERenderSet set, RenderItem* item);

	protected:
		RenderPipeline* mPipeline_;
	};

}
