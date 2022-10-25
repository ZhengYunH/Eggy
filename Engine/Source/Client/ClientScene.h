#pragma once
#include "Core/Config.h"
#include "Core/Interface/IRenderElements.h"


namespace Eggy
{
	class Scene
	{

	};

	class ClientScene : public Scene
	{
	public:
		void AddRenderElement(IRenderElements* elements)
		{
			mRenderElements_.push_back(elements);
		}

		List<IRenderElements*>& GetRenderElements()
		{
			return mRenderElements_;
		}

		List<IRenderElements*> mRenderElements_;
	};
}