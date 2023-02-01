#pragma once
#include "Core/Config.h"
#include "Core/Interface/IRenderScene.h"
#include "Graphics/Elements/RenderElement.h"


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
		IRenderElement* AllocateRenderElement() override;
		RenderObject* AllocateRenderObject() override;
		RenderObject* SubmitRenderElement(ERenderSet set, IRenderElement* element) override;

		List<IRenderObject*>& GetRenderObjects(ERenderSet set) override;

		void StartFrame() override;

		void EndFrame() override;

		Camera* GetCamera() override { HYBRID_RUN(mCamera_); return mCamera_; }
		void SetCamera(Camera* camera) { mCamera_ = camera; }

	private:
		void ClearRenderObjects();
		void ClearRenderElements();
		void ClearResourcePool();

	protected:
		Map<ERenderSet, List<IRenderObject*>> mRenderObjects_;
		List<IRenderElement*> mRenderElements_;
		Camera* mCamera_;
	};
}