#pragma once
#include "UnitTestHeader.h"
#include "Core/DataType/DesignPattern/DP_Factory.h"

#define DeclareProduct(_TKEY, _KEY, _PRODUCT_BASE, _PRODUCT) private: static TProductRegistrar<_PRODUCT_BASE, _PRODUCT, _TKEY> __Registrar;
#define DefineProduct(_TKEY, _KEY , _PRODUCT_BASE, _PRODUCT) TProductRegistrar<_PRODUCT_BASE, _PRODUCT, _TKEY> _PRODUCT::__Registrar(_KEY);


namespace Eggy::UnitTest
{
	namespace DP_UnitTest
	{
		namespace DP_Factory
		{
#define DeclareProductImpl(_Key, _PRODUCT) DeclareProduct(EKey, EKey::_Key, ProductBase, _PRODUCT)
#define DefineProductImpl(_Key, _PRODUCT) DefineProduct(EKey, EKey::_Key, ProductBase, _PRODUCT)

			enum class EKey
			{
				CPA,
				CPB
			};

			class ProductBase
			{
			public:
				virtual ~ProductBase() {}
			public:
				int I{ 0 };
			};

			class ConcreteProductA : public ProductBase
			{
				DeclareProductImpl(CPA, ConcreteProductA);

			public:
				ConcreteProductA() {}
				ConcreteProductA(int i) { I = i; }
			};
			
			class ConcreteProductB : public ProductBase
			{
				DeclareProductImpl(CPB, ConcreteProductB);
			public:
				ConcreteProductB() {}
				ConcreteProductB(int i) { I = i + 1; }
			};

			DefineProductImpl(CPA, ConcreteProductA);
			DefineProductImpl(CPB, ConcreteProductB);

			static void TestSimpleFactory()
			{
				using SimpleFactory = TSimpleFactory<ConcreteProductA>;
				auto cp1 = SimpleFactory::Instance().Create();
				auto cp2 = SimpleFactory::Instance().Create(1);
				UT_EXPECT_EQ(cp1->I == 0);
				UT_EXPECT_EQ(cp2->I == 1);
			}

			static void TestConcreteFactory()
			{
				using ConcreteFactory = TConcreteFactory<ProductBase, ConcreteProductA>;
				ConcreteFactory cf;
				auto cp1 = cf.Create();
				auto cp2 = cf.Create(1);
				UT_EXPECT_EQ(cp1->I == 0);
				UT_EXPECT_EQ(cp2->I == 1);
			}
	
			static void TestFactory()
			{
				using Factory = TFactory<ProductBase, EKey>;
				ProductBase* cpa = Factory::Instance().Create(EKey::CPA);
				ProductBase* cpb = Factory::Instance().Create(EKey::CPB);

				UT_ASSERT(dynamic_cast<ConcreteProductA*>(cpa));
				UT_ASSERT(dynamic_cast<ConcreteProductB*>(cpb));

			}

#undef DefineProductImpl
#undef DeclareProductImpl
		}

		static void Test()
		{
			DP_Factory::TestSimpleFactory();
			DP_Factory::TestConcreteFactory();
			DP_Factory::TestFactory();
		}
	}
}


