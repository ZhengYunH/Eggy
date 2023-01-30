#pragma once
#include "Core/Config.h"
#include "Core/Interface/IRenderElement.h"
#include "Core/Object/IObject.h"
#include "Core/Engine/Event/Event.h"


namespace Eggy
{
	class RenderScene;
	class IScene : public IObject
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

		void LoadWorld(const String s);
		void Deserialize(const String& s) override;
		String Serialize() const override;
		void Tick();

	protected:
		Map<String, RenderScene*> mRenderScenes_;
		RenderScene* mMainRenderScene_{ nullptr };
		class IWorld* mWorld_{ nullptr };
	};
}