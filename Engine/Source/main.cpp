#include "Graphics/RHI/IRenderDevice.h"
#include "Core/Reflection/ReflectionTester/ReflectionClass.h"
#include "Drivers/Windows/Win32Game.h"
#include "Core/Engine/Engine.h"
#include "Core/System/ConfigSystem.h"

int main()
{
	using namespace Eggy;
	// Eggy::DoTest();
	// Initial System
	ConfigSystem* config = new ConfigSystem();
	Win32Game* platform = new Win32Game();
	Engine* engine = new Engine(platform);
	engine->Initialize();
	while (true)
	{
		engine->TickLoop();
	}
	engine->Finalize();
	return 0;
}
