#pragma once
#include "Core/Config.h"


namespace Eggy
{
	class Type;

	struct FieldInfo
	{
		FieldInfo() {}

		FieldInfo(String name, Type* type)
			: Name(name)
			, Type(type)
		{
		}
		

		String Name;
		Type* Type { nullptr };
	};

	struct FunctionInfo
	{
		String Name;
		Type* Return{ nullptr };
		Type* Paramters{ nullptr };
		uint16 ParameterSize{ 0 };
	};


	enum EType : uint32
	{
		EType_None = 0,
		EType_Intrinsic,
		EType_Struct,
		EType_Class,
		EType_Num
	};
	
	using PConstuctor = std::function<void* ()>;
	class Type
	{
	public:
		Type() {}
		Type(String name, PConstuctor generator)
			: mName(name)
			, mGenerator(generator)
		{}

		const String& GetName() const { return mName; }

	public:
		static Type* GetDefaultObject()
		{
			static Type* __DefaultObject;
			__DefaultObject = new Type("Unknown", nullptr);
			return __DefaultObject;
		}

	public:
		virtual void* Construct() const { return mGenerator(); }

	protected:
		String mName;
		PConstuctor mGenerator;
	};

	class Reflection
	{
	public:
		static Map<String, Type*> GRegistrationDict;

		template<typename _TType, const char* _TName>
		static void RegisterType()
		{
			AddReflectionInfo(_TName, _TType::GetDefaultObject());
		}

		static void RegisterClass(
			String name,
			PConstuctor generator,
			FieldInfo* fields,
			uint16 numFieldInfo,
			FunctionInfo* functions,
			uint16 numFunctionInfo
		);

		static Type* GetType(const Name& name)
		{
			return GRegistrationDict.find(name)->second;
		}

	private:
		static void AddReflectionInfo(String _TName, Type* info)
		{
			GRegistrationDict[_TName] = info;
		}
	};

	class IntrinsicType : public Type
	{
	public:
		IntrinsicType(String name, PConstuctor generator) : Type(name, generator) {}
	};

#define DECLARE_INTRINSIC_TYPE(AliasName, Type)  \
	class IntrinsicType__##Type : public IntrinsicType \
	{ \
	public: \
		IntrinsicType__##Type() : IntrinsicType(#AliasName,  [] { return new Type(); }) {} \
		static IntrinsicType__##Type* GetDefaultObject() \
		{ \
			static IntrinsicType__##Type* __DefaultObject; \
			__DefaultObject = new IntrinsicType__##Type(); \
			return __DefaultObject; \
		} \
		struct IntrinsicTypeRegister { IntrinsicTypeRegister() {} }; \
		static IntrinsicTypeRegister __mRegister; \
	}; \
	using AliasName = IntrinsicType__##Type; 

	DECLARE_INTRINSIC_TYPE(IntegerType, int);
	DECLARE_INTRINSIC_TYPE(FloatType, float);
	DECLARE_INTRINSIC_TYPE(BoolType, bool);

	
	class ClassType : public Type
	{
	public:
		ClassType() = default;
		ClassType(
			String name, PConstuctor generator,
			FieldInfo* fields, uint16 numFieldInfo, 
			FunctionInfo* functions, uint16 numFunctionInfo
		)
			: Type(name, generator)
			, NumFields(numFieldInfo)
			, NumFunctions(numFunctionInfo)
		{
			if (NumFields)
			{
				Fields = new FieldInfo[NumFields];
				for (uint16 i = 0; i < NumFields; ++i)
					Fields[i] = std::move(fields[i]); 
			}

			if (NumFunctions)
			{
				Functions = new FunctionInfo[NumFunctions];
				for (uint16 i = 0; i < NumFunctions; ++i)
					Functions[i] = std::move(functions[i]);
			}
		}

		FieldInfo* Fields{ nullptr };
		uint16 NumFields{ 0 };

		FunctionInfo* Functions{ nullptr };
		uint16 NumFunctions{ 0 };
	};
	
#define IMP_GETCLASS(Name) \
	static const ClassType* GetClass() \
	{\
		return static_cast<const ClassType*>(IObject::GetClassInfo(#Name)); \
	}

	class IObject
	{
	public:
		static const Type* GetClass()
		{
			return GetClassInfo("IObject");
		}

		static const Type* GetClassInfo(const Name& name)
		{
			return Reflection::GRegistrationDict.find(name)->second;
		}
	};


	

}