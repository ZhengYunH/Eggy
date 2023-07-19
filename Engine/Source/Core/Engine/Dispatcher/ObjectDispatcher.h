#pragma once
#include "Core/Config.h"
#include "DataType/Container/List.h"


namespace Eggy
{
	class ITickable;
	class ObjectDispatcher
	{
	public:
		void AddTickable(ITickable* tickable);

		void DelTickable(ITickable* tickable);

		void Dispatch();

	private:
		List<ITickable*> mTickable_;
	};

	extern ObjectDispatcher* GObjectDispatcher;
}