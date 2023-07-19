#pragma once
#include "Core/Config.h"


namespace Eggy
{
	class ITickable
	{
	public:
		ITickable();
		
		virtual ~ITickable();

	public:
		virtual void PreTick_on_ot(float dt) = 0;
		virtual void Tick_on_par(float dt) = 0;
		virtual void PostTick_on_ot(float dt) = 0;
	};
}