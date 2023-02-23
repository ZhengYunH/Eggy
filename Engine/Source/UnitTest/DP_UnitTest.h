#pragma once
#include "UnitTestHeader.h"
#include "Core/DataType/DesignPattern/DP_Factory.h"

namespace Eggy::UnitTest
{
	namespace DP_UnitTest
	{
		namespace DP_Factory
		{
			class ProductBase
			{
			public:
				virtual ~ProductBase() {}
			public:
				int I{ 0 };
			};

			class ConcreteProductA : public ProductBase
			{
			public:
				ConcreteProductA() {}
				ConcreteProductA(int i) { I = i; }
			};
			using ConcreteFactory = TConcreteFactory<ProductBase, ConcreteProductA>;

			static void TestConcreteFactory()
			{
				ConcreteFactory cf;
				auto cp1 = cf.Create();
				auto cp2 = cf.Create(1);
				UT_EXPECT_EQ(cp1->I == 0);
				UT_EXPECT_EQ(cp2->I == 1);
			}

			using Factory = TFactory<ProductBase>;

			class ConcreteProductB : public ProductBase
			{
			public:
				ConcreteProductB() {}
				ConcreteProductB(int i) { I = i + 1; }
			};
			static void TestFactory()
			{
				TProductRegistrar<ProductBase, ConcreteProductA> registerarA(StaticName(ConcreteProductA));
				TProductRegistrar<ProductBase, ConcreteProductB> registerarB(StaticName(ConcreteProductB));

				ProductBase* cpa = Factory::Instance().Create(StaticName(ConcreteProductA));
				ProductBase* cpb = Factory::Instance().Create(StaticName(ConcreteProductB));

				HYBRID_CHECK(dynamic_cast<ConcreteProductA*>(cpa));
				HYBRID_CHECK(dynamic_cast<ConcreteProductB*>(cpb));

			}
		}

		static void Test()
		{
			DP_Factory::TestConcreteFactory();
			DP_Factory::TestFactory();
		}
	}
}
