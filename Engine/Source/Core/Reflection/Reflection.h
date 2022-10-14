#include "Core/Config.h"


namespace Eggy
{
	typedef void* (*PInstanceGenerator)();

	class Type
	{
	private:
		PInstanceGenerator mGenerator  = nullptr;
	
	public:
		Name mType;
		Name mName;

		Type() {}
		Type(const Name& type, const Name name, PInstanceGenerator gen)
		{
			mType = type;
			mName = name;
			mGenerator = gen;
		}
	};

	struct FieldInfo
	{
		Name mName;
		Type mType;
		FieldInfo* mNext = nullptr;
	};

	typedef FieldInfo ParamterInfo;
	struct FunctionInfo
	{
		Name mName;
		Type mReturn;
		ParamterInfo* mParamters = nullptr;
	};

	struct ClassInfo
	{
		Name mName;
		Type mType;
		List<FieldInfo> mFields;
		List<FunctionInfo> mFunctions;
	};

	
	class Reflection
	{
	public:
		static Map<Name, ClassInfo> GRegistrationDict;

		template<typename _Type>
		static void Class(
			const Name& name,
			FieldInfo* fields,
			FunctionInfo* functions
		)
		{
			GRegistrationDict[name] = ClassInfo();
			GRegistrationDict[name].mName = name;
		}
	};

	class IObject
	{
	public:
		static const ClassInfo& GetClass()
		{
			return GetClassInfo("IObject");
		}

		static const ClassInfo& GetClassInfo(const Name& name)
		{
			return Reflection::GRegistrationDict.find(name)->second;
		}
	};
}