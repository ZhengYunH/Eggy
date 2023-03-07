#pragma once
#include "Core/Config.h"
#include "Core/Interface/IRenderScene.h"
#include "Graphics/Elements/RenderElement.h"
#include "Graphics/RHI/IRenderPipeline.h"


namespace Eggy
{
	struct IRenderElement;
	struct RenderObject;
	class Camera;

	class RenderScene final: public IRenderScene
	{
	public:
		RenderScene();
		~RenderScene();

	public:
		void StartFrame() override;
		void EndFrame() override;

		void StartDeviceFrame() override;
		void EndDeviceFrame() override;

		Camera* GetCamera() override { HYBRID_RUN(mCamera_); return mCamera_; }
		void SetCamera(Camera* camera) { mCamera_ = camera; }
		RenderContext* GetContext() { return mContext_; }
		RenderPipeline* GetPipeline() { return mPipeline_; }

		void AddLight(ILight* light) override;
		void DelLight(ILight* light) override;
		List<ILight*>& GetLights() override;

	protected:
		Camera* mCamera_{ nullptr };
		List<ILight*> mLights_;
		RenderContext* mContext_{ nullptr };
		RenderPipeline* mPipeline_{ nullptr };
	};
}