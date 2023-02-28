#pragma once
#include "Core/Config.h"
#include "ReflectionMacro.h"


namespace Eggy
{
	class Type;

	template<typename _Primitives>
	struct GetType_Template
	{
		using primitive = _Primitives;
		static constexpr bool is_intrinsic = false;
		static constexpr bool with_vtable = true;
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

		template<typename TPrimitive, typename TValue>
		void SetValue(TPrimitive* entity, TValue&& value)
		{
			size_t address = Address;
			/*	if (GetType_Template<TPrimitive>::with_vtable)
					address -= sizeof(int);*/
			*(TValue*)((char*)entity + address) = value;
		}

		template<typename TPrimitive, typename TValue>
		void GetValue(TPrimitive* entity, TValue& value)
		{
			size_t address = Address;
			//if (GetType_Template<TPrimitive>::with_vtable)
			//	address -= sizeof(int);
			value = *(TValue*)((char*)entity + address);
		}

		String Name;
		Type* Type { nullptr };
		size_t Address{ 0 };
	};

	struct FunctionInfo
	{
		struct FunctionParameterInfo
		{
			String Name;
			Type* Type{ nullptr };
		};
		
		FunctionInfo(){}

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

	template<typename TPrimitive, typename TRetType, typename... Args>
	struct TFunctionInfo : public FunctionInfo
	{
		using FunctionInfo::FunctionParameterInfo;
		using TMethod = TRetType(TPrimitive::*)(Args...);

		TFunctionInfo()
		{
		}

		TFunctionInfo(
			const String& name,
			TMethod method,
			List<FunctionParameterInfo>&& params
		) : 
			FunctionInfo(name, GetType<TRetType>(), std::move(params))
			, MethodAddress(method)
		{}

		TRetType Invoke(TPrimitive* object, Args... args)
		{
			return (object->*MethodAddress)(args...);
		}

		TMethod MethodAddress;
	};

	/*
		void f1(int i, float j) {} 
		FunctionInfoinfo(
			"f1", #Name, 
			&CLASS::f1, #Type
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
	
	using PConstuctor = std::function<void*()>;
	using PDestructor = std::function<void(void*)>;
	class Type
	{
	public:
		Type() {}
		Type(String name, PConstuctor constuctor, PDestructor destructor=nullptr)
			: mName(name)
			, mConstructor(constuctor)
			, mDestructor(std::move(destructor))
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
		virtual void* Construct() const { return mConstructor(); }
		virtual void Destruct(void* data) const { mDestructor(data); }

	protected:
		String mName;
		PConstuctor mConstructor;
		PDestructor mDestructor;
	};

	class Reflection
	{
	public:
		static Map<String, Type*> GRegistrationDict;

		template<typename TPrimitive>
		static void RegisterType(
			String name,
			PConstuctor ctor, PDestructor dtor,
			FieldInfo* fields,
			uint16 numFieldInfo,
			FunctionInfo** functions,
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
			String name, 
			PConstuctor ctor, PDestructor dtor,
			FieldInfo* fields, uint16 numFieldInfo,
			FunctionInfo** functions, uint16 numFunctionInfo
		)
			: Type(name, ctor, dtor)
			, NumFields(numFieldInfo)
			, NumFunctions(numFunctionInfo)
		{
			if (NumFields)
			{
				std::swap(Fields, fields);
			}

			if (NumFunctions)
			{
				std::swap(Functions, functions);
			}
		}

		FieldInfo* Fields{ nullptr };
		uint16 NumFields{ 0 };

		FunctionInfo** Functions{ nullptr };
		uint16 NumFunctions{ 0 };

	public:
		FieldInfo* GetProperty(const String& propName)
		{
			for (uint16 i = 0; i < NumFields; ++i)
			{
				if (Fields[i].Name == propName)
				{
					return &Fields[i];
				}
			}
			return nullptr;
		}

		FunctionInfo* GetFunction(const String& funcName)
		{
			for (uint16 i = 0; i < NumFunctions; ++i)
			{
				if (Functions[i]->Name == funcName)
				{
					return Functions[i];
				}
			}
			return nullptr;
		}
	};

	class IntrinsicType : public ClassType
	{
	public:
		IntrinsicType(String name, PConstuctor ctor, PDestructor dtor) : ClassType(name, ctor, dtor, nullptr, 0, nullptr, 0) {}
	};

	
	class IntrinsicType__Void : public IntrinsicType
	{
	public:
		IntrinsicType__Void() : IntrinsicType("IntrinsicType__Void", [] { return nullptr; }, [](void*) {}) {}
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

#define DECLARE_INTRINSIC_TYPE(AliasName, TPrimitives)  \
	class IntrinsicType__##TPrimitives : public IntrinsicType \
	{ \
	public: \
		IntrinsicType__##TPrimitives() : IntrinsicType(#AliasName,  [] { return new TPrimitives(); }, [](void* s){ delete (TPrimitives*)s; }) {} \
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
		static constexpr bool with_vtable = false; \
		using type=IntrinsicType__##TPrimitives; \
		static type* value; \
	};

	DECLARE_INTRINSIC_TYPE(IntegerType, int);
	DECLARE_INTRINSIC_TYPE(FloatType, float);
	DECLARE_INTRINSIC_TYPE(BoolType, bool);


	template<typename TPrimitive>
	void Reflection::RegisterType(String name, PConstuctor ctor, PDestructor dtor, FieldInfo* fields, uint16 numFieldInfo, FunctionInfo** functions, uint16 numFunctionInfo)
	{
		ClassType* info = new ClassType(name, ctor, dtor, fields, numFieldInfo, functions, numFunctionInfo);
		GetType_Template<TPrimitive>::value = info;
		AddReflectionInfo(name, info);
	}
}