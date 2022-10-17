#include "Core/Config.h"


namespace Eggy
{
	typedef void* (*PInstanceGenerator)();

	struct IType
	{
		Name Name;
	};

	template<typename _TType, const char* _TName>
	struct Type : public IType
	{
		Type() : IType()
		{
			Name = _TName;
		}

		template<typename... Args>
		_TType* Constructor(Args&... args) { return new _TType(std::forward(args)...); }

		static Type< _TType, _TName>* GetDefaultObject()
		{
			static Type< _TType, _TName>* __DefaultObject;
			__DefaultObject = new Type< _TType, _TName>();
			return __DefaultObject;
		}
	};

	template<typename _TType, const char* _TName>
	struct IntrinsicType : public Type<_TType, _TName>
	{
	public:
		IntrinsicType() : Type() {}

		struct IntrinsicTypeRegister
		{
			IntrinsicTypeRegister();
		};

	private:
		static IntrinsicTypeRegister __mRegister;
	};

#define DECLARE_INTRINSIC_TYPE(Name, Type)  \
	extern const char __StrIntrinsicType__##Name[]; \
	using Name = IntrinsicType<Type, __StrIntrinsicType__##Name>;

	DECLARE_INTRINSIC_TYPE(IntegerType, int);
	DECLARE_INTRINSIC_TYPE(FloatType, float);
	DECLARE_INTRINSIC_TYPE(BoolType, bool);

	template<typename _TType, const char* _TName>
	struct ClassType : public Type<_TType, _TName>
	{
		ClassType() = default;
		ClassType(struct FieldInfo** fields, uint16 numFieldInfo, struct FunctionInfo** functions, uint16 numFunctionInfo)
			: Fields(fields)
			, NumFieldInfo(numFieldInfo)
			, Functions(functions)
			, NumFunctionInfo(numFunctionInfo)
		{
		}

		struct FieldInfo** Fields{ nullptr };
		uint16 NumFieldInfo{ 0 };

		struct FunctionInfo** Functions;
		uint16 NumFunctionInfo{ 0 };
	};

	class Reflection
	{
	public:
		static Map<Name, IType*> GRegistrationDict;

		template<typename _TType, const char* _TName>
		static void RegisterType()
		{
			Type<_TType, _TName>* info = new Type<_TType, _TName>();
			AddReflectionInfo(_TName, info);
		}

		template<typename _TType, const char* _TName>
		static void RegisterClass(
			struct FieldInfo** fields,
			uint16 numFieldInfo,
			struct FunctionInfo** functions,
			uint16 numFunctionInfo
		)
		{
			ClassType<_TType, _TName>* info = new ClassType<_TType, _TName>(fields, numFieldInfo, functions, numFunctionInfo);
			AddReflectionInfo(_TName, info);
		}

		static IType* GetType(const Name& name)
		{
			return GRegistrationDict.find(name)->second;
		}

	private:
		static void AddReflectionInfo(const char* _TName, IType* info)
		{
			GRegistrationDict[_TName] = info;
		}
	};
	

#define IMP_GETCLASS(Type, Name) \
	static const ClassType<Type, Name>* GetClass() \
	{\
		return static_cast<const ClassType<Type, Name>*>(IObject::GetClassInfo(*Name); \
	}


	class IObject
	{
	public:
		static const IType* GetClass()
		{
			return GetClassInfo("IObject");
		}

		static const IType* GetClassInfo(const Name& name)
		{
			return Reflection::GRegistrationDict.find(name)->second;
		}
	};


	struct FieldInfo
	{
		FieldInfo(const Name& name, const IType* type) 
			: FieldName(name)
			, TypeName(type->Name)
		{
		}

		const IType* GetType() { return Reflection::GetType(TypeName); }
		const Name& GetName() { return FieldName; }
		
		Name FieldName;
		Name TypeName;
	};

	struct FunctionInfo
	{
		Name Name;
		IType* Return{ nullptr };
		IType* Paramters{ nullptr };
		uint16 ParameterSize{ 0 };
	};


}