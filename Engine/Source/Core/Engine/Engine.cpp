#include "Engine.h"

// just for render-test, will be deleted soon
#include "Client/ClientScene.h"
#include "Graphics/Renderer/RenderMesh.h"
#include "Core/System/SystemManager.h"


namespace Eggy
{
	DefineSystem(Engine);

	void Engine::Initialize()
	{
		mSystemManager_ = new SystemManager();
		mSystemManager_->Initialize();
	}

	void Engine::PostInitialize()
	{
		CreateRenderDevice();
		mClientScene_ = new ClientScene();
		mCurrFrameTime_ = mLastFrameTime_ = std::chrono::high_resolution_clock::now();
	}

	void Engine::Finalize()
	{
		mSystemManager_->Finalize();
		SafeDestroy(mSystemManager_);
	}

	void Engine::SetPlatform(IPlatform* platform)
	{
		Platform = platform;
	}

	bool Engine::TickLoop()
	{
		if (!Platform->PumpMessage())
			return false;
		mCurrFrameTime_ = std::chrono::high_resolution_clock::now();
		_DeltaTime = std::chrono::duration<float, std::chrono::seconds::period>(mCurrFrameTime_ - mLastFrameTime_).count();
		mLastFrameTime_ = mCurrFrameTime_;
		mClientScene_->Tick_ot();
		mClientScene_->Tick_rdt();
		mClientScene_->Tick_dt();
		return true;
	}
}