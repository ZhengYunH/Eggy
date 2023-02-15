#pragma once
#include "IRenderHeader.h"
#include "IRenderResource.h"
#include "RenderGraph.h"
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

		ResourceBinding(uint16 buffers, uint16 textures, uint16 views)
		{
			Buffers = buffers;
			Textures = textures;
			Views = views;

			Data = new IRenderResource * [buffers + textures + views];
			for (uint16 i = 0; i < Buffers + Textures + Views; ++i)
				Data[i] = nullptr;
		}

		~ResourceBinding()
		{
			SafeDestroyArray(Data);
		}

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

		void SetConstant(uint16 i, IConstantBuffer* tex)
		{
			Data[i] = tex;
		}
		IConstantBuffer* GetConstant(uint16 i)
		{
			return (IConstantBuffer*)Data[i];
		}

		void SetTexture(uint16 i, ITexture* tex)
		{
			Data[Buffers + i] = tex;
		}
		ITexture* GetTexture(uint16 i)
		{
			return (ITexture*)Data[Buffers + i];
		}

		void SetView(uint16 i, IRenderTarget* view)
		{
			Data[Buffers + Textures + i] = view;
		}
		IRenderTarget* GetView(uint16 i)
		{
			return (IRenderTarget*)Data[Buffers + Textures + i];
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
		RenderPass* Pass_;
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

		virtual RenderPass* Setup(RenderGraphBuilder* builder) { return nullptr; }
		void Compile(RenderGraphBuilder* builder);
		void Render(RenderContext* context);
		void Clear(RenderContext* context);

		RenderPass* GenerateRenderPass();
		void AddRenderPass(RenderPass* pass);
		
		void AddDrawCallChannel(ERenderSets sets, RenderPass* pass);
		void AddDrawCallChannel(ERenderSet set, RenderPass* pass);
		List<RenderPass*>& GetRenderPasses() { return mRenderPasses_; }
		void Consolidate();
		void AddDrawCall(ERenderSet set, DrawCall* dp);
		GlobalConstant& GetGlobalConstant() { return mConstant_; }
		virtual const RenderTargetDesc& GetBackBuffer(RenderGraphBuilder* builder) = 0;

		void CopyRenderTargetList(List<IRenderTarget*>&& RenderTargets);
		IRenderTarget* GetRenderTargetResource(size_t rtIndex) const;

	protected:
		List<RenderPass*> mRenderPasses_;
		Map<ERenderSet, List<RenderPass*>> mRenderPassSet_;
		GlobalConstant mConstant_;
		RenderContext* mContext_;

		List<IRenderTarget*> mRenderTargetResource_;
	};

	class RenderContext
	{
	public:
		RenderContext(RenderPipeline* pipeline) : mPipeline_(pipeline)
		{}
		virtual ~RenderContext();

		RenderItemInfo* AddRenderSceneInfo(RenderObject* object);
		RenderItem* GenerateRenderItem(RenderItemInfo* info);
		DrawCall* GenerateDrawCall(RenderItem* item);
		void RecycleDrawCall(DrawCall* dp);

		void Prepare();
		void Clear();
		void SubmitRenderItem(ERenderSet set, RenderItem* item);

	protected:
		RenderPipeline* mPipeline_;
		RenderGraphBuilder mBuilder_;
		List<RenderItemInfo*> mRenderInfoItems_;
	};

}
