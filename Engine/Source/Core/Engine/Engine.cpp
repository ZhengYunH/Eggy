#include "Engine.h"
#include "Graphics/RHI/IRenderDevice.h"


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