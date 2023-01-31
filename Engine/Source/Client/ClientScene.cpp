#include "ClientScene.h"
#include "Core/Object/IEntity.h"
#include "Core/Object/IPrimitive.h"
#include "World.h"
#include "RenderScene.h"
#include "Graphics/RHI/IRenderDevice.h"


namespace Eggy
{
	static String MAIN_RENDER_SCENE = "MainRenderScene";

	ClientScene::ClientScene()
	{
		LoadWorld("");
	}

	ClientScene::~ClientScene()
	{
		UnloadWorld();
	}

	RenderScene* ClientScene::GetRenderScene()
	{
		return dynamic_cast<RenderScene*>(mWorld_->GetRenderScene());
	}

	List<RenderScene*> ClientScene::GetRenderScenes()
	{
		List<RenderScene*> renderScenes;
		renderScenes.push_back(GetRenderScene());
		return renderScenes;
	}

	void ClientScene::LoadWorld(const String s)
	{
		mWorld_ = new World(this);
		mWorld_->Deserialize(s);
	}

	void ClientScene::UnloadWorld()
	{
		SafeDestroy(mWorld_);
	}

	void ClientScene::Deserialize(const String& s)
	{
		
	}

	String ClientScene::Serialize() const
	{
		return "";
	}

	void ClientScene::Tick_ot()
	{
		StartLogicFrame();
		mWorld_->Tick();
		for (auto prim : mWorld_->GetPrimitives())
		{
			prim->CollectPrimitives(GetRenderScene());
		}
		EndLogicFrame();
	}

	void ClientScene::Tick_rdt()
	{
		StartRenderFrame();
		GRenderDevice->PrepareResource();
		GRenderDevice->DrawFrame();
		EndRenderFrame();
	}

	void ClientScene::StartLogicFrame()
	{
		mWorld_->StartFrame();
	}

	void ClientScene::EndLogicFrame()
	{

	}

	void ClientScene::StartRenderFrame()
	{

	}

	void ClientScene::EndRenderFrame()
	{
		mWorld_->EndFrame();
	}

}
