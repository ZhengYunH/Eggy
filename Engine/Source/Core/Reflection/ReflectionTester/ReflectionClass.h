#include "../Reflection.h"
#include <iostream>

namespace Eggy
{
	static const char ReflectionClassTesterNameStr[] = "ReflectionClassTester";

	class ReflectionClassTester : public IObject
	{
	public:
		class ReflectionClassTesterClassInfo
		{
		public:
			ReflectionClassTesterClassInfo()
			{
				FieldInfo fieldInfos[]{
					FieldInfo("TestFieldInt", IntegerType::GetDefaultObject()),
					FieldInfo("TestFieldFloat", FloatType::GetDefaultObject()),
					FieldInfo("TestFieldBool", BoolType::GetDefaultObject()),
				};
				uint16 numFieldInfos = 3;
				FunctionInfo* funcInfos = nullptr;

				Reflection::RegisterClass(
					"ReflectionClassTester",
					[] {return new ReflectionClassTester(); },
					fieldInfos, 
					numFieldInfos,
					funcInfos, 
					0
				);
			}
		};
		static ReflectionClassTesterClassInfo __mClassInfo;
		
		IMP_GETCLASS(ReflectionClassTester);

		int TestFieldInt;
		float TestFieldFloat;
		bool TestFieldBool;

		void TestFunc1();
		float TestFunc2();
		float TestFunc3(int a, float b, bool c);
	};

	void DoTest();
}