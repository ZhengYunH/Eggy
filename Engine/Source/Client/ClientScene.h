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
		void AddRenderElement(IRenderElement* elements)
		{
			mRenderElements_.push_back(elements);
		}

		List<IRenderElement*>& GetRenderElements()
		{
			return mRenderElements_;
		}

		List<IRenderElement*> mRenderElements_;
	};
}