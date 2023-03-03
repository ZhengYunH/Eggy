#pragma once
#include "ReflectionDefine.h"


namespace Eggy
{
	class IntrinsicType : public ClassType
	{
	public:
		IntrinsicType(String name, PConstuctor ctor, PDestructor dtor) 
			: ClassType(name, ctor, dtor, nullptr, 0, nullptr, 0) 
		{
		}
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
}

