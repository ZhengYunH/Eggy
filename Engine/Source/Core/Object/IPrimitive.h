#pragma once
#include "Core/Config.h"

namespace Eggy
{
	class IPrimitive
	{
	public:
		virtual void CollectPrimitives(class RenderContext* context) = 0;
	};
}
