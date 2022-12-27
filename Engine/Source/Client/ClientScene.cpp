#include "ClientScene.h"
#include "RenderScene.h"
#include "Core/Object/IEntity.h"
#include "Core/Object/IWorld.h"

// test code
#include "Client/PrimitivesComponent.h"


namespace Eggy
{
	static String MAIN_RENDER_SCENE = "MainRenderScene";

	ClientScene::ClientScene()
	{
		mMainRenderScene_ = CreateRenderScene(MAIN_RENDER_SCENE);
		mWorld_ = new IWorld();
	}

	ClientScene::~ClientScene()
	{
		for (auto& pair : mRenderScenes_)
		{
			delete pair.second;
		}
		mRenderScenes_.clear();
	}

	RenderScene* ClientScene::CreateRenderScene(String key)
	{
		HYBRID_CHECK(mRenderScenes_.find(key) == mRenderScenes_.end());
		RenderScene* rdScene = new RenderScene();
		mRenderScenes_[key] = rdScene;
		return rdScene;
	}

	RenderScene* ClientScene::GetRenderScene()
	{
		return GetRenderScene(MAIN_RENDER_SCENE);
	}

	RenderScene* ClientScene::GetRenderScene(String key)
	{
		HYBRID_CHECK(mRenderScenes_.find(key) != mRenderScenes_.end());
		return mRenderScenes_[key];
	}

	void ClientScene::Deserialize(const String& s)
	{
		IEntity* entity = new IEntity();
		entity->AddComponent(new PrimitiveComponent());
		entity->EnterWorld(mWorld_);
		mEntities_.push_back(entity);
	}

	String ClientScene::Serialize() const
	{
		String s;
		for (auto& entity : mEntities_)
		{
			s += entity->Serialize();
		}
		return s;
	}

}
