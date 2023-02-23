#pragma once
#include "Core/Concepts.h"

namespace Eggy
{
	template <typename _Type>
	class TSingleton
	{
	public:
		static _Type& Instance()
		{
			static _Type instance;
			return instance;
		}
	};
}


