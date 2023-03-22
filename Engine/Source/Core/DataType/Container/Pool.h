#pragma once
#include "Core/Concepts.h"
#include "Map.h"

namespace Eggy
{
	template<typename _T>
		requires cHashable<_T>
	class TPool : public TSingleton<TPool<_T>>
	{
		using _Shaptr = std::conditional_t<cEnableShared<_T>, TSharedPtr<_T>, TSharedPtrUnref<_T>>;
	public:
		template<typename... _Args>
		_Shaptr Create(_Args... args)
		{
			_T* object = new _T(std::forward<_Args>(args)...);
			_Shaptr ptr = GetPoolObject(*object);
			SafeDestroy(object);
			return ptr;
		}

		_Shaptr GetPoolObject(_T& t)
		{
			size_t hashValue = std::hash<_T>()(t);
			if (!mPool_.contains(hashValue))
			{
				_T* object = new _T(std::move(t));
				mPool_.insert({ hashValue, object });
			}
			return mPool_.at(hashValue);
		}

	protected:
		Map<size_t, _Shaptr> mPool_;
	};
}

