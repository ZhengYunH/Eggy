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

		RenderScene* GetRenderScene();
		List<RenderScene*> GetRenderScenes();

		void LoadWorld(const String s);
		void UnloadWorld();

		void Deserialize(const String& s) override;
		String Serialize() const override;
		void Tick();

	protected:
		class World* mWorld_{ nullptr };
	};
}