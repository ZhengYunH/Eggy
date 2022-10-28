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
		
	}

	void Engine::Initialize()
	{
		CreateRenderDevice();
		mClientScene_ = new ClientScene();
		mClientScene_->AddRenderElement(new RenderPyramidElement());
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