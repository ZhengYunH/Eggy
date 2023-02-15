#pragma once
#include "Core/Config.h"

namespace Eggy
{
	template<typename T>
	concept CWeakable = requires(T * a)
	{
		{a->Acquire()} -> std::same_as<void>;
		{a->Release()} -> std::same_as<void>;
	};

	template<CWeakable _Type>
	class TWeakPtr
	{
	public:
		TWeakPtr()
		{
		}

		TWeakPtr(_Type* obj)
		{
			Obj = obj;
		}

		void Release()
		{
			Obj = nullptr;
		}

		_Type* Get()
		{
			return Obj;
		}

		operator bool()
		{
			return Obj != nullptr;
		}

		_Type* operator ->()
		{
			return Obj;
		}

	private:
		_Type* Obj{ nullptr };
	};
}