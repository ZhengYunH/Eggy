#include "Graphics/RHI/IRenderDevice.h"
#include "Core/Reflection/ReflectionTester/ReflectionClass.h"
#include "Drivers/Windows/Win32Game.h"
#include "Core/Engine/Engine.h"
#include "Core/System/ConfigSystem.h"
#include "Core/System/InputSystem.h"
#include "Core/System/FileSystem.h"
#include "Core/System/RenderSystem.h"
#include "Core/Math/MathTester.h"


int main()
{
	using namespace Eggy;
	// Eggy::DoTest();
	// MathTester::Test();
	// Initial System
	ConfigSystem* config = new ConfigSystem();
	InputSystem* inputSystem = new InputSystem();
	FileSystem* fileSystem = new FileSystem();
	RenderSystem* renderSystem = new RenderSystem();
	renderSystem->Initialize();
	Win32Game* platform = new Win32Game();
	Engine* engine = new Engine(platform);
	engine->Initialize();
	while (true)
	{
		platform->PumpMessage();
		engine->TickLoop();
	}
	engine->Finalize();
	return 0;
}
