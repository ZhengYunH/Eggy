#pragma once
#include "Core/Config.h"


namespace Eggy
{
	enum class ERenderSet : uint32
	{
		NONE = 0,
		MAIN = 1,

		START = MAIN,
		END = MAIN + 1,
	};

	struct IRenderObject;
	struct IRenderElement;

	class IRenderScene 
	{
	public:
		virtual void StartFrame() = 0;
		virtual void EndFrame() = 0;

		virtual IRenderElement* AllocateRenderElement() = 0;
		virtual IRenderObject* AllocateRenderObject() = 0;
		virtual IRenderObject* SubmitRenderElement(ERenderSet set, IRenderElement* element) = 0;
		virtual List<IRenderObject*>& GetRenderObjects(ERenderSet set) = 0;
	};
}