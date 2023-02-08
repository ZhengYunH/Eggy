#pragma once
#include "Core/Config.h"


namespace Eggy
{
	class Type;

	template<typename _Primitives>
	struct GetType_Template
	{
		using primitive = _Primitives;
		static constexpr bool is_intrinsic = false;
		/*
		* using type = ReflectionClass;
		* static ReflectionClass* value;
		*/
	};

	template<typename _Primitives>
	using GetType_primitive = GetType_Template<_Primitives>::primitive;

	template<typename _Primitives>
	using GetType_type = GetType_Template<_Primitives>::type;

	
	template<typename _Primitives>
	GetType_type<_Primitives>* GetType()
	{
		return GetType_Template<_Primitives>::value;
	}

	template<typename TPrimitives>
	concept IsIntrinsicType = GetType_Template<TPrimitives>::is_intrinsic;

	struct FieldInfo
	{
		FieldInfo() {}

		FieldInfo(String name, Type* type, size_t address)
			: Name(name)
			, Type(type)
			, Address(address)
		{
		}
		
		FieldInfo(FieldInfo&& rhs) noexcept
		{
			Name = rhs.Name;
			Type = rhs.Type;
			Address = rhs.Address;
		}

		void operator= (const FieldInfo&& rhs) noexcept
		{
			Name = rhs.Name;
			Type = rhs.Type;
			Address = rhs.Address;
		}

		template<typename TValue>
		void SetValue(void* entity, TValue&& value)
		{
			*(TValue*)((char*)entity + Address) = value;
		}

		template<typename TValue>
		void GetValue(void* entity, TValue& value)
		{
			value = *(TValue*)((char*)entity + Address);
		}

		String Name;
		Type* Type { nullptr };
		size_t Address;
	};


	struct FunctionInfo
	{
		struct FunctionParameterInfo 
		{
			String Name;
			Type* Type;
		};

		FunctionInfo()
		{
		}

		FunctionInfo(
			const String& name,
			Type* returnType,
			List<FunctionParameterInfo>&& params
		)
		{
			Name = name;
			Return = returnType;
			ParameterSize = (uint16)params.size();
			Paramters = new FunctionParameterInfo[ParameterSize];
			for (uint16 i = 0; i < ParameterSize; ++i)
				std::swap(Paramters[i], params[i]);
		}

		String Name;
		Type* Return{ nullptr };
		FunctionParameterInfo* Paramters{ nullptr };
		uint16 ParameterSize{ 0 };
	};

	/*
		void f1(int i, float j) {} 
		FunctionInfoinfo(
			"f1", #Name, 
			GetType<void>, #Type
			{ 
				{"i", GetType<int>() }, 
				{"j", GetType<float>()}
			}
			
		) 
	*/

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

		template<typename TPrimitive>
		static void RegisterType(
			String name,
			PConstuctor generator,
			FieldInfo* fields,
			uint16 numFieldInfo,
			FunctionInfo* functions,
			uint16 numFunctionInfo
		);

		template<typename TPrimitive> requires IsIntrinsicType<TPrimitive>
		static void RegisterIntrinsicType(String name);

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

	class IntrinsicType : public ClassType
	{
	public:
		IntrinsicType(String name, PConstuctor generator) : ClassType(name, generator, nullptr, 0, nullptr, 0) {}
	};

	
	class IntrinsicType__Void : public IntrinsicType
	{
	public:
		IntrinsicType__Void() : IntrinsicType("IntrinsicType__Void", [] { return nullptr; }) {}
		static IntrinsicType__Void* GetDefaultObject()
		{
			return nullptr;
		}
		struct IntrinsicTypeRegister {
			IntrinsicTypeRegister();
		};
		static IntrinsicTypeRegister __mRegister;
	};
	
	template<> struct GetType_Template<void>
	{
		using primitive = void;
		static constexpr bool is_intrinsic = true;
		using type = typename IntrinsicType__Void;
		static type* value;
	};

#define StaticName(Name) #Name
#define DECLARE_INTRINSIC_TYPE(AliasName, TPrimitives)  \
	class IntrinsicType__##TPrimitives : public IntrinsicType \
	{ \
	public: \
		IntrinsicType__##TPrimitives() : IntrinsicType(#AliasName,  [] { return new TPrimitives(); }) {} \
		static IntrinsicType__##TPrimitives* GetDefaultObject() \
		{ \
			static IntrinsicType__##TPrimitives* __DefaultObject; \
			__DefaultObject = new IntrinsicType__##TPrimitives(); \
			return __DefaultObject; \
		} \
		struct IntrinsicTypeRegister { \
			IntrinsicTypeRegister(); \
		};\
		static IntrinsicTypeRegister __mRegister; \
	}; \
	using AliasName = IntrinsicType__##TPrimitives; \
	template<> struct GetType_Template<TPrimitives> \
	{ \
		using primitive=TPrimitives; \
		static constexpr bool is_intrinsic = true; \
		using type=IntrinsicType__##TPrimitives; \
		static type* value; \
	};

	DECLARE_INTRINSIC_TYPE(IntegerType, int);
	DECLARE_INTRINSIC_TYPE(FloatType, float);
	DECLARE_INTRINSIC_TYPE(BoolType, bool);


	template<typename TPrimitive>
	void Reflection::RegisterType(String name, PConstuctor generator, FieldInfo* fields, uint16 numFieldInfo, FunctionInfo* functions, uint16 numFunctionInfo)
	{
		ClassType* info = new ClassType(name, generator, fields, numFieldInfo, functions, numFunctionInfo);
		GetType_Template<TPrimitive>::value = info;
		AddReflectionInfo(name, info);
	}

	
#define IMP_GETCLASS(TPrimitive) \
	static const ClassType* GetClass() \
	{\
		return static_cast<const ClassType*>(IObject::GetClassInfo(#TPrimitive)); \
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