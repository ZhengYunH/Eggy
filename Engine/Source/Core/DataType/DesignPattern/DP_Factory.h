#pragma once
#include "Core/Concepts.h"
#include "DP_Singleton.h"
#include "Core/DataType/Container/String.h"
#include "Core/DataType/Container/Map.h"
#include "Core/Foundation.h"


namespace Eggy
{
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

	template<typename _Product>
	class TFactory : public TSingleton<TFactory<_Product>>
	{
		friend class TSingleton<TFactory<_Product>>;
	public:
		void Register(const String& name, IProductRegister<_Product>* registrar)
		{
			HYBRID_CHECK(mRegisteriesMap.find(name) == mRegisteriesMap.end());
			mRegisteriesMap[name] = registrar;
		}

		_Product* Create(const String& name)
		{
			auto itr = mRegisteriesMap.find(name);
			if (itr != mRegisteriesMap.end())
			{
				return itr->second->Create();
			}
			LOG("Create Product Fail For: ", name);
			return nullptr;
		}

	private:
		TFactory() {}
		~TFactory() {}
		TFactory(const TFactory&) = delete;
		const TFactory& operator=(const TFactory&) = delete;

	private:
		Map<String, IProductRegister<_Product>*> mRegisteriesMap;
	};

	template<typename _Product, typename _ProductImpl>
	class TProductRegistrar : public IProductRegister<_Product>
	{
	public:
		explicit TProductRegistrar(const String& name)
		{
			TFactory<_Product>::Instance().Register(name, this);
		}

		_Product* Create() override
		{
			return new _ProductImpl();
		}
	};
}