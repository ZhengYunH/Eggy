#include "ReflectionClass.h"

namespace Eggy
{
	ReflectionClassTester::ReflectionClassTesterClassInfo ReflectionClassTester::__mClassInfo;
	GetType_type<ReflectionClassTester>* GetType_Template<ReflectionClassTester>::value = nullptr;

	void DoTest()
	{
		ReflectionClassTester tester;
		std::cout << "ReflectionClassTester Start " << std::endl;
		std::cout << "Name: " << tester.GetClass()->GetName() << std::endl;
		
		auto ClassInfo = ReflectionClassTester::GetClass();
		ReflectionClassTester* ctorTester = reinterpret_cast<ReflectionClassTester*>(ClassInfo->Construct());
		std::cout << "Fields: " << std::endl;
		for (uint16 i = 0; i < ClassInfo->NumFields; ++i)
		{
			auto t = ClassInfo->Fields[i].Type;
			std::cout << i << ": " << ClassInfo->Fields[i].Name << std::endl;
		}
		// Test SetValue
		FieldInfo& field = ClassInfo->Fields[1];
		std::cout << "Test Value setter / getter: " << field.Name << std::endl;
		
		tester.TestFieldFloat = 1.f;
		float of;
		field.GetValue(&tester, of);
		std::cout << "Test getter1: " << of << std::endl;
		field.SetValue(&tester, 50.f);
		std::cout << "Test setter" << std::endl;
		field.GetValue(&tester, of);
		std::cout << "Test getter2: " << of << std::endl;

		
		std::cout << "ReflectionClassTester End " << std::endl;
	}
}