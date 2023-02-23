#include "Graphics/RHI/IRenderDevice.h"
#include "Core/Reflection/ReflectionTester/ReflectionClass.h"
#include "Core/Math/MathTester.h"
#include "Drivers/Windows/Win32Game.h"
#include "Core/Engine/Engine.h"
#include "UnitTest/UnitTest.h"


int main()
{
	using namespace Eggy;
	DEBUG_RUN(UnitTest::TestAll());
	// Initial System
	Engine* engine = new Engine();
	engine->Initialize();
	engine->SetPlatform(new Win32Game());
	engine->PostInitialize();
	while (true)
	{
		if (!engine->TickLoop())
			break;
	}
	engine->Finalize();
	return 0;
}
