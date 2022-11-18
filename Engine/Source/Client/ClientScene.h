#pragma once
#include "Core/Config.h"
#include "Core/Interface/IRenderElement.h"


namespace Eggy
{
	class RenderScene;
	class IScene
	{

	};

	class ClientScene : public IScene
	{
	public:
		ClientScene();
		virtual ~ClientScene();

		RenderScene* CreateRenderScene(String key);
		RenderScene* GetRenderScene();
		RenderScene* GetRenderScene(String key);

	protected:
		Map<String, RenderScene*> mRenderScenes_;
		RenderScene* mMainRenderScene_{ nullptr };
	};
}