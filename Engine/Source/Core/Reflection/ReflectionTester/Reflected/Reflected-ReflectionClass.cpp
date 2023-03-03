#include "Core/Reflection/Reflection.h"
#include "../ReflectionClass.h"


namespace Eggy
{
	// EnableReflection
	class ReflectionClassTesterClassInfo
	{
	public:
		ReflectionClassTesterClassInfo()
		{
			FieldInfo* fieldInfos = new FieldInfo[3]{
				FieldInfo("TestFieldInt", GetType<int>(), offsetof(ReflectionClassTester, TestFieldInt)),
				FieldInfo("TestFieldFloat", GetType<float>(), offsetof(ReflectionClassTester, TestFieldFloat)),
				FieldInfo("TestFieldBool", GetType<bool>(), offsetof(ReflectionClassTester, TestFieldBool)),
			};
			uint16 numFieldInfos = 3;
			FunctionInfo** funcInfos = new FunctionInfo*[3]{
				new TFunctionInfo("TestFunc1", &ReflectionClassTester::TestFunc1, {}),
				new TFunctionInfo("TestFunc2", &ReflectionClassTester::TestFunc2, {}),
				new TFunctionInfo("TestFunc3", &ReflectionClassTester::TestFunc3, {
						{StaticName(a), GetType<int>()},
						{StaticName(b), GetType<float>()},
						{StaticName(c), GetType<bool>()},
					}
				)
			};
			uint16 numFunctions = 3;

			Reflection::Instance().RegisterType<ReflectionClassTester>(
				"ReflectionClassTester",
				[] {return new ReflectionClassTester(); }, [](void* s) {delete (ReflectionClassTester*)s; },
				fieldInfos,
				numFieldInfos,
				funcInfos,
				numFunctions
			);
		}
	};
	static ReflectionClassTesterClassInfo __mClassInfo;

	template<> struct GetType_Template<ReflectionClassTester>
	{
		using primitive = ReflectionClassTester;
		static constexpr bool is_intrinsic = false;
		static constexpr bool with_vtable = true;
		using type = ClassType;
		static type* value;
	};

	static ReflectionClassTester ClassInfo;
	// ReflectionClassTester::ReflectionClassTesterClassInfo ReflectionClassTester::__mClassInfo;
	GetType_type<ReflectionClassTester>* GetType_Template<ReflectionClassTester>::value = nullptr;
}
