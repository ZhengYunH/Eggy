#include "Engine.h"

// just for render-test, will be deleted soon
#include "Client/ClientScene.h"
#include "Graphics/Renderer/RenderMesh.h"


namespace Eggy
{
	DefineSystem(Engine);

	Engine::Engine(IPlatform* platform)
		: Platform(platform)
	{
		HYBRID_CHECK(!GInstance);
		GInstance = this;
		
	}

	void Engine::Initialize()
	{
		CreateRenderDevice();
		mClientScene_ = new ClientScene();
		mCurrFrameTime_ = mLastFrameTime_ = std::chrono::high_resolution_clock::now();
	}

	void Engine::Finalize()
	{
	}

	void Engine::TickLoop()
	{
		mCurrFrameTime_ = std::chrono::high_resolution_clock::now();
		_DeltaTime = std::chrono::duration<float, std::chrono::seconds::period>(mCurrFrameTime_ - mLastFrameTime_).count();
		mLastFrameTime_ = mCurrFrameTime_;
		mClientScene_->Tick_ot();
		mClientScene_->Tick_rdt();
		mClientScene_->Tick_dt();
	}
}