#include "ClientScene.h"
#include "RenderScene.h"
#include "Core/Object/IEntity.h"
#include "Core/Object/IWorld.h"




namespace Eggy
{
	static String MAIN_RENDER_SCENE = "MainRenderScene";

	ClientScene::ClientScene()
	{
		mMainRenderScene_ = CreateRenderScene(MAIN_RENDER_SCENE);
		LoadWorld("");
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

	void ClientScene::LoadWorld(const String s)
	{
		mWorld_ = new IWorld(this);
		mWorld_->Deserialize(s);
	}

	void ClientScene::Deserialize(const String& s)
	{
		
	}

	String ClientScene::Serialize() const
	{
		return "";
	}

	void ClientScene::Tick()
	{
		mWorld_->Tick();
	}

}
