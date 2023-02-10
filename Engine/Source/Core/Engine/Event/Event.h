#pragma once
#include "Core/Config.h"
#include "EventHelper.h"


namespace Eggy
{
	template<typename TRetType=void, typename... TArgs>
	class Event
	{
	public:
		using FuncType = std::function<TRetType(TArgs...)>;

	public:
		virtual TRetType BoardCast(TArgs... args)
		{
			if constexpr (std::is_same <TRetType, bool>::value)
			{
				bool ret = false;
				for (auto& func : BindingFunc)
				{
					ret = (func)(std::forward<TArgs>(args)...);
				}
				return ret;
			}
			else
			{
				for (auto& func : BindingFunc)
				{
					(func)(std::forward<TArgs>(args)...);
				}
			}
		}

		virtual void Bind(FuncType func)
		{
			BindingFunc.push_back(func);
		}

	protected:
		List<FuncType> BindingFunc;
	};
}

