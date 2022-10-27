#include "Engine.h"
#include "Graphics/RHI/IRenderDevice.h"

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
		mClientScene_ = new ClientScene();
	}

	void Engine::Initialize()
	{
		CreateRenderDevice();
		mClientScene_->AddRenderElement(new RenderCubeElement());
	}

	void Engine::Finalize()
	{
	}

	void Engine::TickLoop()
	{
		GRenderDevice->PrepareResource();
		GRenderDevice->DrawFrame();
	}
}