#pragma once
#include "Core/Config.h"
#include "Core/Interface/IRenderElement.h"
#include "Core/Object/IObject.h"


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

		void Deserialize(const String& s) override;
		String Serialize() const override;

	protected:
		Map<String, RenderScene*> mRenderScenes_;
		RenderScene* mMainRenderScene_{ nullptr };
		List<class IEntity*> mEntities_;
		class IWorld* mWorld_{ nullptr };
	};
}