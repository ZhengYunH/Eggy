#include "ReflectionClass.h"

namespace Eggy
{
	ReflectionClassTester::ReflectionClassTesterClassInfo ReflectionClassTester::ReflectionClassTester::__mClassInfo;

	void DoTest()
	{
		ReflectionClassTester tester;
		std::cout << "ReflectionClassTester Start " << std::endl;

		std::cout << "Name: " << ReflectionClassTester::GetClass().mName << std::endl;
		std::cout << "Name: " << tester.GetClass().mName << std::endl;


		std::cout << "ReflectionClassTester End " << std::endl;
	}
}