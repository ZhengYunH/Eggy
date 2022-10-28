#pragma once
#include "Core/Config.h"

namespace Eggy
{
	template<typename... TArgs>
	class Event
	{
	public:
		using FuncType = std::function<void(TArgs...)>;

	public:
		virtual void BoardCast(TArgs... args)
		{
			for (auto& func : BindingFunc)
			{
				(func)(std::forward<TArgs>(args)...);
			}
		}

		virtual void Bind(FuncType& func)
		{
			BindingFunc.push_back(func);
		}

	protected:
		List<FuncType> BindingFunc;
	};
}