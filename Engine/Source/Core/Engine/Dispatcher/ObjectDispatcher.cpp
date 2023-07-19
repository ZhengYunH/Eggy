#include "ObjectDispatcher.h"
#include "Engine/Engine.h"
#include "Interface/ITickable.h"

namespace Eggy
{
	ObjectDispatcher* GObjectDispatcher = new ObjectDispatcher();

	void ObjectDispatcher::AddTickable(ITickable* tickable)
	{
		HYBRID_CHECK(std::find(mTickable_.begin(), mTickable_.end(), tickable) == mTickable_.end());
		mTickable_.push_back(tickable);
	}

	void ObjectDispatcher::DelTickable(ITickable* tickable)
	{
		auto itr = std::find(mTickable_.begin(), mTickable_.end(), tickable);
		HYBRID_CHECK(itr != mTickable_.end());
		mTickable_.erase(itr);
	}

	void ObjectDispatcher::Dispatch()
	{
		float deltaTime = Engine::Get()->GetDeltaTime();
		for (auto tickable : mTickable_)
			tickable->PreTick_on_ot(deltaTime);

		for (auto tickable : mTickable_)
			tickable->Tick_on_par(deltaTime);

		for (auto tickable : mTickable_)
			tickable->PostTick_on_ot(deltaTime);
	}

}

