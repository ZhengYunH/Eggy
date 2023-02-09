#include "Core/Object/IObject.h"
#include <iostream>

namespace Eggy
{
	class ReflectionClassTester : public IObject
	{
	public:
		IMP_GETCLASS(ReflectionClassTester);

		EProperty()
		int TestFieldInt;

		EProperty()
		float TestFieldFloat;

		EProperty()
		bool TestFieldBool;

		EFunction()
		void TestFunc1() { std::cout << "Func1" << std::endl; }

		EFunction()
		float TestFunc2() { std::cout << "Func2" << std::endl; return 0.f; }

		EFunction();
		int TestFunc3(int a, float b, bool c) { std::cout << "Func1£º" << a << std::endl;  return 1; }
	};
	EnableReflection(ReflectionClassTester);
	
	void DoTest();
	ReflectionClassTester* DoTestConstruct();
	void DoTestProperty(ReflectionClassTester* object);
	void DoTestFunction(ReflectionClassTester* object);
}