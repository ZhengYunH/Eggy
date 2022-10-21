#pragma once
#include "Core/Config.h"
#include "Core/Interface/ISystem.h"
#include "Core/Interface/IPlatform.h"


namespace Eggy
{
	class Engine : public ISystem<Engine>
	{
		DeclareSystem(Engine);

	public:
		Engine(IPlatform* platform);

		void Initialize() override;
		void Finalize() override;
	
	public:
		void TickLoop();

	public:
		IPlatform* Platform{ nullptr };
	};
}