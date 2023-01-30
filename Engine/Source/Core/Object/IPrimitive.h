#pragma once
#include "Core/Config.h"

namespace Eggy
{
	class IPrimitive
	{
	public:
		virtual void CollectPrimitives(class IRenderScene* renderScene) = 0;
	};
}
