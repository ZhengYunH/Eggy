#pragma once
#include "IRenderHeader.h"
#include "IRenderResource.h"
#include "RenderGraph.h"
#include "IShaderRenderResource.h"
#include "Shader/ShaderCollection.h"
#include "IShadingState.h"
#include "ILight.h"
#include "RenderObject.h"
#include "Core/Interface/IRenderScene.h"


namespace Eggy
{
	struct GlobalConstant
	{
		struct CameraData
		{
			Vector3 ViewPos;
			Vector4 BasisX;
			Vector4 BasisY;
			Vector4 BasisZ;
			Vector4 Info;
		};
		Matrix4x4 ViewTransform;
		Matrix4x4 ProjectTransform;
		CameraData CamData;
		Vector4 Color{ 1.f,1.f,1.f,1.f };
		float Brightness{ 1.0f };
	};

	class RenderPipeline;
	class RenderContext;
	class RenderPass;
	class RenderItem;
	class RenderItemInfo;

	struct DrawCall
	{
		RenderItem* Item_{ nullptr };
		DrawCall* Next_{ nullptr };
		RenderPass* Pass_;

		ShaderTechnique* ShaderTechnique_{ nullptr };
		IShadingState* ShadingState_{ nullptr };
		GeometryBinding* GeometryBinding_{ nullptr };
		PipelineState PipelineState_;


		bool IsMemberReady()
		{
			HYBRID_CHECK(GeometryBinding_);
			HYBRID_CHECK(ShadingState_);
			HYBRID_CHECK(ShaderTechnique_);
			return true;
		}

		void CreateDeviceResource(IRenderResourceFactory* factory)
		{
			HYBRID_CHECK(IsMemberReady());
			ShaderTechnique_->CreateDeviceResource(factory);
			ShadingState_->CreateDeviceResource(factory);
			GeometryBinding_->CreateDeviceResource(factory);
			PipelineState_.CreateDeviceResource(factory);
		}

		~DrawCall();
	};

	class RenderPipeline
	{
		friend class RenderContext;
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
		virtual const RenderTargetDesc& GetBackBuffer(RenderGraphBuilder* builder) = 0;

		void CopyRenderTargetList(List<IRenderTarget*>&& RenderTargets);
		IRenderTarget* GetRenderTargetResource(size_t rtIndex) const;
		RenderContext* GetContext() { return mContext_; }

	protected:
		List<RenderPass*> mRenderPasses_;
		Map<ERenderSet, List<RenderPass*>> mRenderPassSet_;
		RenderContext* mContext_;

		List<IRenderTarget*> mRenderTargetResource_;
	};

	class RenderContext
	{
		static constexpr uint8 MAX_LIGHT_COUNT = 4;
	public:
		RenderContext(IRenderScene* scene, RenderPipeline* pipeline);
		virtual ~RenderContext();

		RenderItemInfo* AddRenderSceneInfo(RenderObject* object);
		RenderItem* GenerateRenderItem(RenderItemInfo* info);
		DrawCall* GenerateDrawCall(RenderItem* item);
		void RecycleDrawCall(DrawCall* dp);

		void Prepare();
		void Clear();
		void SubmitRenderItem(ERenderSet set, RenderItem* item);
		void PrepareBatchData();
		GlobalConstant& GetGlobalConstant() { return mConstant_; }
		ShadingParameterCollection* GetParameters() { return mParams_; }
		ShadingParameterCollection* GetLightParameters() { return mLightParams_; }
		List<ILight*>& GetLights() { return mLights_; }
		void CollectionLights();

	protected:
		IRenderScene* mScene_;
		RenderPipeline* mPipeline_;
		RenderGraphBuilder mBuilder_;
		List<RenderItemInfo*> mRenderInfoItems_;
		ShadingParameterCollection* mParams_;
		ShadingParameterCollection* mLightParams_;
		GlobalConstant mConstant_;
		List<ILight*> mLights_;
	};

}
