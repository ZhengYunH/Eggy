#pragma once
#include "Core/Config.h"

namespace Eggy
{
	class IObject
	{
	public:
		virtual String Serialize() const;
		virtual void Deserialize(const String& s);
	};
}