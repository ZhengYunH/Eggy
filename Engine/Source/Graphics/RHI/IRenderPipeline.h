#pragma once
#include "IRenderHeader.h"
#include "IRenderResource.h"
#include "IShader.h"

#include "Core/Interface/IRenderObject.h"
#include "Core/Math/Matrix4x4.h"
#include "Core/Engine/Resource/Texture.h"
#include "Core/Object/Material.h"


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

	struct BatchConstant
	{
		Matrix4x4 ModelTransform;
	};

	struct ShaderConstant
	{

	};

	struct GlobalConstant
	{
		Matrix4x4 ViewTransform;
		Matrix4x4 ProjectTransform;
	};

	struct RenderObject : public IRenderObject
	{
		BatchConstant ObjectConstantData_;
	};

	class RenderItemInfo
	{
	public:
		RenderObject* Object{ nullptr };
		GeometryBinding* GeometryBinding_{ nullptr };
		class Material* Material_{ nullptr };
	};

	class RenderItem
	{
	public:
		RenderItemInfo* Info{ nullptr };
	};

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

	
	class RenderPipeline;
	class RenderContext;

	class RenderPass
	{
	public:
		RenderPass(RenderPipeline* pipeline) 
			: Pipeline(pipeline)
		{

		}

		void AddDrawCall(DrawCall* drawCall);
		void Clear();
		void Consolidate();

		DrawCall* GetDrawCallHead() { return DrawCallHead; }

	protected:
		RenderPipeline* Pipeline{ nullptr };
		DrawCall* DrawCallHead{ nullptr };
		DrawCall* DrawCallLast{ nullptr };
		bool bIsConsolidated{ false };
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
