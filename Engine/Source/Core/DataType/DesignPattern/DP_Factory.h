#pragma once
#include "Core/Concepts.h"
#include "DP_Singleton.h"
#include "Core/DataType/Container/String.h"
#include "Core/DataType/Container/Map.h"
#include "Core/Foundation.h"


namespace Eggy
{
	template<typename _Product>
	class TSimpleFactory : public TSingleton<TSimpleFactory<_Product>>
	{
	public:
		template<typename... _Args>
		_Product* Create(_Args... args)
		{
			return new _Product(std::forward<_Args>(args)...);
		}
	};

	template<typename _AbstaractProduct>
	class TAbstractFactory
	{
	public:
		virtual _AbstaractProduct* Create() = 0;
		virtual ~TAbstractFactory() {}
	};

	template<typename _AbstaractProduct, typename _ConcreateProduct> 
		requires cDerivedFrom<_AbstaractProduct, _ConcreateProduct> && cDefaultConstructible<_ConcreateProduct>
	class TConcreteFactory : public TAbstractFactory<_AbstaractProduct>
	{
	public:
		_AbstaractProduct* Create() override
		{
			return new _ConcreateProduct();
		}

		template<typename... _Args>
		_AbstaractProduct* Create(_Args... args)
		{
			return new _ConcreateProduct(std::forward<_Args>(args)...);
		}
	};

	template<typename _Product>
	class IProductRegister
	{
	public:
		virtual _Product* Create() = 0;

	protected:
		IProductRegister() {}
		virtual ~IProductRegister() {}

	private:
		IProductRegister(const IProductRegister&) = delete;
		const IProductRegister& operator=(const IProductRegister&) = delete;
	};

	template<typename _Product, typename _Key=String>
	class TFactory : public TSingleton<TFactory<_Product, _Key>>
	{
		friend class TSingleton<TFactory<_Product, _Key>>;
	public:
		void Register(const _Key& key, IProductRegister<_Product>* registrar)
		{
			HYBRID_CHECK(mRegisteriesMap.find(key) == mRegisteriesMap.end());
			mRegisteriesMap[key] = registrar;
		}

		_Product* Create(const _Key& key)
		{
			auto itr = mRegisteriesMap.find(key);
			if (itr != mRegisteriesMap.end())
			{
				return itr->second->Create();
			}
			LOG("Create Product Fail For: ", key);
			return nullptr;
		}

	private:
		TFactory() {}
		~TFactory() {}
		TFactory(const TFactory&) = delete;
		const TFactory& operator=(const TFactory&) = delete;

	private:
		Map<_Key, IProductRegister<_Product>*> mRegisteriesMap;
	};

	template<typename _Product, typename _ProductImpl, typename _Key=String>
	class TProductRegistrar : public IProductRegister<_Product>
	{
	public:
		explicit TProductRegistrar(const _Key& key)
		{
			TFactory<_Product, _Key>::Instance().Register(key, this);
		}

		_Product* Create() override
		{
			return new _ProductImpl();
		}
	};
}