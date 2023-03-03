#pragma once
#include <atomic>
#include "Core/Foundation.h"
#include "Core/Concepts.h"


namespace Eggy
{
	template<typename _Dst, typename _Src> 
	static auto compatible_cast(_Src src) noexcept
	{
		static_assert(std::is_pointer_v<_Dst>);
		static_assert(std::is_pointer_v<_Src>);
		if constexpr (std::is_base_of_v<std::remove_pointer_t<_Dst>, std::remove_pointer_t<_Src>>)
		{
			return static_cast<_Dst>(src);
		}
		else
		{
			return dynamic_cast<_Dst>(src);
		}
	}

	class IRefObject
	{
	public:
		IRefObject(){}
		
		virtual ~IRefObject() {}

		void Acquire()
		{
			mRef_.fetch_add(1, std::memory_order_acq_rel);
		}

		void Release()
		{
			if (mRef_.fetch_sub(1, std::memory_order_acq_rel) == 1)
			{
				delete this;
			}
		}

		int GetRefCount()
		{
			return mRef_.load(std::memory_order_acquire);
		}

	protected:
		std::atomic<int> mRef_{ 0 };
	};


	template<typename _Type>
	class TSharedPtr
	{
		template<class _Other>
		friend class TSharedPtr;
	public:
		// ctor && dtor
		TSharedPtr(){}

		TSharedPtr(std::nullptr_t){}

		TSharedPtr(_Type* ref) : mPtr_(ref)
		{
			IncRef();
		}

		TSharedPtr(const TSharedPtr<_Type>& rhs) noexcept
		{
			if (rhs)
				_Attach(rhs);
		}

		template<typename _Other>
		TSharedPtr(const TSharedPtr<_Other>& rhs) noexcept
		{
			if (rhs)
				_Attach(rhs);
		}

		TSharedPtr(TSharedPtr<_Type>&& rhs) noexcept
		{
			if (rhs)
				_Transfer(std::move(rhs));
		}

		~TSharedPtr()
		{
			if (mPtr_)
				DecRef();
		}

	public: 
		// operator
		explicit operator bool() const noexcept
		{
			return mPtr_ != nullptr;
		}

		bool operator!() const noexcept
		{
			return mPtr_ == nullptr;
		}

		TSharedPtr<_Type>& operator=(const TSharedPtr<_Type>& rhs) noexcept
		{
			if (mPtr_)
			{
				_Detach();
			}
			if (rhs) 
			{
				_Attach(rhs);
			}
			return *this;
		}

		template<typename _Other>
		TSharedPtr<_Type>& operator=(const TSharedPtr<_Other>& rhs) noexcept
		{
			if (mPtr_)
			{
				_Detach();
			}
			if (rhs)
			{
				_Attach(rhs);
			}
			return *this;
		}

		TSharedPtr<_Type>& operator=(TSharedPtr<_Type>&& rhs) noexcept
		{
			if (mPtr_)
			{
				_Detach();
			}
			if (rhs)
			{
				_Transfer(std::move(rhs));
			}
			return *this;
		}

		_Type* operator->()
		{
			return mPtr_;
		}

		_Type& operator*()
		{
			return *mPtr_;
		}
	public:
		int GetRefCount()
		{
			return mPtr_->GetRefCount();
		}

		_Type* Get()
		{
			return mPtr_;
		}


	private:
		template<typename _Other>
		void _Attach(const TSharedPtr<_Other>& src)
		{
			HYBRID_CHECK(!mPtr_);
			mPtr_ = compatible_cast<_Type*>(src.mPtr_);
			if (mPtr_)
			{
				IncRef();
			}
		}

		void _Detach()
		{
			HYBRID_CHECK(mPtr_);
			DecRef();
			mPtr_ = nullptr;
		}

		void _Transfer(TSharedPtr<_Type>&& src)
		{
			HYBRID_CHECK(!mPtr_);
			mPtr_ = src.mPtr_;
			src.mPtr_ = nullptr;
		}

		void IncRef()
		{
			HYBRID_CHECK(mPtr_);
			mPtr_->Acquire();
		}

		void DecRef()
		{
			HYBRID_CHECK(mPtr_);
			mPtr_->Release();
		}

	private:
		_Type* mPtr_{ nullptr };
	};

	template<typename _Type>
	class TRefObject
	{
	public:
		explicit TRefObject(_Type* ptr)
			: mPtr_(ptr)
		{
		}

		~TRefObject()
		{
			delete mPtr_;
		}

		_Type* Get()
		{
			return mPtr_;
		}

		void Acquire()
		{
			mRef_.fetch_add(1, std::memory_order_acq_rel);
		}

		void Release()
		{
			if (mRef_.fetch_sub(1, std::memory_order_acq_rel) == 1)
			{
				delete this;
			}
		}

		int GetRefCount()
		{
			return mRef_.load(std::memory_order_acquire);
		}

	protected:
		_Type* mPtr_{ nullptr };
		std::atomic<int> mRef_{ 0 };
	};


	template<typename _Type>
	class TSharedPtrUnref
	{
	public:
		TSharedPtrUnref() {}

		TSharedPtrUnref(std::nullptr_t) {}

		TSharedPtrUnref(_Type* ref) : mPtr_(new TRefObject<_Type>(ref))
		{
			IncRef();
		}

		TSharedPtrUnref(const TSharedPtrUnref<_Type>& rhs) noexcept
		{
			if (rhs)
				_Attach(rhs);
		}

		TSharedPtrUnref(TSharedPtrUnref<_Type>&& rhs) noexcept
		{
			if (rhs)
				_Transfer(std::move(rhs));
		}

		~TSharedPtrUnref()
		{
			if (mPtr_)
				DecRef();
		}

	public:
		// operator
		explicit operator bool() const noexcept
		{
			return mPtr_ != nullptr;
		}

		bool operator!() const noexcept
		{
			return mPtr_ == nullptr;
		}

		TSharedPtrUnref<_Type>& operator=(const TSharedPtrUnref<_Type>& rhs) noexcept
		{
			if (mPtr_)
			{
				_Detach();
			}
			if (rhs)
			{
				_Attach(rhs);
			}
			return *this;
		}

		TSharedPtrUnref<_Type>& operator=(TSharedPtrUnref<_Type>&& rhs) noexcept
		{
			if (mPtr_)
			{
				_Detach();
			}
			if (rhs)
			{
				_Transfer(std::move(rhs));
			}
			return *this;
		}

		_Type* operator->()
		{
			return mPtr_->Get();
		}

		_Type& operator*()
		{
			return *(mPtr_->Get());
		}

	public:
		int GetRefCount()
		{
			return mPtr_->GetRefCount();
		}

		_Type* Get()
		{
			return mPtr_->Get();
		}

		void _Attach(const TSharedPtrUnref<_Type>& src)
		{
			HYBRID_CHECK(!mPtr_);
			mPtr_ = src.mPtr_;
			if (mPtr_)
				IncRef();
		}

	protected:
		void _Detach()
		{
			HYBRID_CHECK(mPtr_);
			DecRef();
			mPtr_ = nullptr;
		}

		void _Transfer(TSharedPtrUnref<_Type>&& src)
		{
			HYBRID_CHECK(!mPtr_);
			mPtr_ = src.mPtr_;
			src.mPtr_ = nullptr;
		}

		void IncRef()
		{
			HYBRID_CHECK(mPtr_);
			mPtr_->Acquire();
		}

		void DecRef()
		{
			HYBRID_CHECK(mPtr_);
			mPtr_->Release();
		}

	private:
		TRefObject<_Type>* mPtr_{ nullptr };
	};
}


