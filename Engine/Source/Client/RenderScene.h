#pragma once
#include "Core/Config.h"
#include "Core/Interface/IRenderScene.h"
#include "Graphics/Elements/RenderElement.h"


namespace Eggy
{
	struct IRenderElement;
	struct RenderObject;

	class RenderScene final: public IRenderScene
	{
	public:
		RenderScene();

	public:
		IRenderElement* AllocateRenderElement() override;
		RenderObject* AllocateRenderObject() override;
		RenderObject* SubmitRenderElement(ERenderSet set, IRenderElement* element) override;

		List<IRenderObject*>& GetRenderObjects(ERenderSet set) override;

		void StartFrame() override;

		void EndFrame() override;

	protected:
		Map<ERenderSet, List<IRenderObject*>> mRenderObjects_;
		List<IRenderElement*> mRenderElements_;
	};
}