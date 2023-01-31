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
	}

	void Engine::Finalize()
	{
	}

	void Engine::TickLoop()
	{
		mClientScene_->Tick_ot();
		mClientScene_->Tick_rdt();
	}
}