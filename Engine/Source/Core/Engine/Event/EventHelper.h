#pragma once
#include <functional>

template<int N, int ...I>
struct MakeSeqs : MakeSeqs<N - 1, N - 1, I...> {};

template<int...I>
struct MakeSeqs<1, I...>
{
	template<class T, typename TFunc>
	static auto bind(T&& obj, TFunc&& func)
	{
		return std::bind(std::forward<TFunc>(func), std::forward<T>(obj), std::_Ph<I>{}...);
	}
};

template<class T, typename... Args>
auto BindMemberFunc(void(T::* func)(Args...), T* obj)
{
	return MakeSeqs<sizeof...(Args) + 1>::bind(obj, func);
}


#define EVENT_BIND_MEMBER_FUNC(Event, Object, MemberFunc) Event.Bind(BindMemberFunc(&MemberFunc, Object));
#define EVENT_BIND_FUNC(Event, Func) Event.Bind(Func);

