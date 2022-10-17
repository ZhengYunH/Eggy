#include "ReflectionClass.h"

namespace Eggy
{
	ReflectionClassTester::ReflectionClassTesterClassInfo ReflectionClassTester::__mClassInfo;

	void DoTest()
	{
		ReflectionClassTester tester;
		std::cout << "ReflectionClassTester Start " << std::endl;
		std::cout << "Name: " << tester.GetClass()->Name << std::endl;
		
		auto ClassInfo = ReflectionClassTester::GetClass();
		std::cout << "Fields: " << std::endl;
		for (uint16 i = 0; i < ClassInfo->NumFieldInfo; ++i)
		{
			const IType* t = ClassInfo->Fields[i]->GetType();
			std::cout << i << ": " << ClassInfo->Fields[i]->GetName() << std::endl;
		}
		
		std::cout << "ReflectionClassTester End " << std::endl;
	}
}