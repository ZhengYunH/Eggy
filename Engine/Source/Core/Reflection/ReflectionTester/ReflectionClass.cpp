#include "ReflectionClass.h"

namespace Eggy
{
	void DoTest()
	{
		ReflectionClassTester* tester = DoTestConstruct();
		std::cout << "ReflectionClassTester Start " << std::endl;
		std::cout << "Name: " << tester->GetClass()->GetName() << std::endl;
		DoTestProperty(tester);
		DoTestFunction(tester);
		delete tester;
		std::cout << "ReflectionClassTester End " << std::endl;
	}

	ReflectionClassTester* DoTestConstruct()
	{
		auto ClassInfo = ReflectionClassTester::GetClass();
		ReflectionClassTester* ctorTester = static_cast<ReflectionClassTester*>(ClassInfo->Construct());
		return ctorTester;
	}

	void DoTestProperty(ReflectionClassTester* tester)
	{
		auto ClassInfo = ReflectionClassTester::GetClass();
		std::cout << "Fields: " << std::endl;
		for (uint16 i = 0; i < ClassInfo->NumFields; ++i)
		{
			auto t = ClassInfo->Fields[i].Type;
			std::cout << i << ": " << ClassInfo->Fields[i].Name << std::endl;
		}
		// Test SetValue
		FieldInfo* field = ClassInfo->GetProperty(StaticName(TestFieldFloat));
		FieldInfo* field1 = ClassInfo->GetProperty(StaticName(TestFieldFloatNonExist));
		DEBUG_CHECK(field);
		DEBUG_CHECK(!field1);

		std::cout << "Test Value setter / getter: " << field->Name << std::endl;
		tester->TestFieldFloat = 1.f;
		float of;
		field->GetValue(tester, of);
		DEBUG_CHECK(of == 1.f);
		field->SetValue(tester, 50.f);
		field->GetValue(tester, of);
		DEBUG_CHECK(of == 50.f);
	}

	void DoTestFunction(ReflectionClassTester* object)
	{
		auto ClassInfo = ReflectionClassTester::GetClass();
		std::cout << "Functions: " << std::endl;
		for (uint16 i = 0; i < ClassInfo->NumFunctions; ++i)
		{
			auto t = ClassInfo->Functions[i]->Return;
			std::cout << i << ": " << ClassInfo->Functions[i]->Name << std::endl;
		}

		FunctionInfo* info = ClassInfo->GetFunction(StaticName(TestFunc3));
		std::cout << "Test Function Invoke " << info->Name << std::endl;
		int ret = static_cast<TFunctionInfo<ReflectionClassTester, int, int, float, bool>*>(info)->Invoke(object, 1, 2.f, false);
		DEBUG_CHECK(ret == 1);

		// int ret = info->Invoke(object, 1, 2.f, false);
	}

}