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
				List<FieldInfo*>* fieldInfos = new List<FieldInfo*>
				({
					new FieldInfo("TestFieldInt", IntegerType::GetDefaultObject()),
					new FieldInfo("TestFieldFloat", FloatType::GetDefaultObject()),
					new FieldInfo("TestFieldBool", BoolType::GetDefaultObject()),
				});
				FunctionInfo** funcInfos = nullptr;

				Reflection::RegisterClass<ReflectionClassTester, ReflectionClassTesterNameStr>(
					fieldInfos->data(), 
					static_cast<uint16>(fieldInfos->size()), 
					funcInfos, 
					0
				);
			}
		};
		static ReflectionClassTesterClassInfo __mClassInfo;
		

		static const ClassType<ReflectionClassTester, ReflectionClassTesterNameStr>* GetClass()
		{
			return static_cast<const ClassType<ReflectionClassTester, ReflectionClassTesterNameStr>*>(IObject::GetClassInfo(ReflectionClassTesterNameStr));
		}

		int TestFieldInt;
		float TestFieldFloat;
		bool TestFieldBool;

		void TestFunc1();
		float TestFunc2();
		float TestFunc3(int a, float b, bool c);
	};

	void DoTest();
}