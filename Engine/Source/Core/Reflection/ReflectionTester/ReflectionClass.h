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
					FieldInfo("TestFieldInt", GetType<int>(), offsetof(ReflectionClassTester, TestFieldInt)),
					FieldInfo("TestFieldFloat", GetType<float>(), offsetof(ReflectionClassTester, TestFieldFloat)),
					FieldInfo("TestFieldBool", GetType<bool>(), offsetof(ReflectionClassTester, TestFieldBool)),
				};
				uint16 numFieldInfos = 3;
				FunctionInfo* funcInfos = nullptr;

				Reflection::RegisterType<ReflectionClassTester>(
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

	template<> struct GetType_Template<ReflectionClassTester>
	{
		using primitive = ReflectionClassTester;
		static constexpr bool is_intrinsic = false;
		using type = ClassType;
		static type* value;
	};

	void DoTest();
}