#pragma once
#include "Core/Config.h"
#include "Core/Interface/IRenderElement.h"


namespace Eggy
{
	class Scene
	{

	};

	class ClientScene : public Scene
	{
	public:
		void AddRenderElement(IRenderElement* element)
		{
			element->Initialize();
			mRenderElements_.push_back(element);
		}

		List<IRenderElement*>& GetRenderElements()
		{
			return mRenderElements_;
		}

		List<IRenderElement*> mRenderElements_;
	};
}