#include "ITickable.h"
#include "Engine/Dispatcher/ObjectDispatcher.h"


namespace Eggy
{
	ITickable::ITickable()
	{
		GObjectDispatcher->AddTickable(this);
	}

	ITickable::~ITickable()
	{
		GObjectDispatcher->DelTickable(this);
	}

}