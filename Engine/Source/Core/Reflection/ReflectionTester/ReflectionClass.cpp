#include "ReflectionClass.h"

namespace Eggy
{
	ReflectionClassTester::ReflectionClassTesterClassInfo ReflectionClassTester::__mClassInfo;

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
		
		std::cout << "ReflectionClassTester End " << std::endl;
	}
}