#include "ReflectionIntrinsic.h"


namespace Eggy
{
	template<typename TPrimitive> requires IsIntrinsicType<TPrimitive>
	void Reflection::RegisterIntrinsicType(String name)
	{
		using ReflectionClass = GetType_type<TPrimitive>;
		ReflectionClass* info = new ReflectionClass();
		GetType_Template<TPrimitive>::value = info;
		AddReflectionInfo(name, info);
	}

	// Special Case
	IntrinsicType__Void::IntrinsicTypeRegister IntrinsicType__Void::__mRegister;
	GetType_type<void>* GetType_Template<void>::value = nullptr;
	IntrinsicType__Void::IntrinsicTypeRegister::IntrinsicTypeRegister()
	{
		Reflection::Instance().RegisterIntrinsicType<void>(StaticName(VoidType));
	}

#define DEFINE_INTRINSIC_TYPE(AliasName, TPrimitive)  \
	AliasName::IntrinsicTypeRegister AliasName::__mRegister; \
	AliasName::IntrinsicTypeRegister::IntrinsicTypeRegister() { Reflection::Instance().RegisterIntrinsicType<TPrimitive>(StaticName(AliasName)); } \
	GetType_type<TPrimitive>* GetType_Template<TPrimitive>::value = nullptr;

	DEFINE_INTRINSIC_TYPE(IntegerType, int);
	DEFINE_INTRINSIC_TYPE(FloatType, float);
	DEFINE_INTRINSIC_TYPE(BoolType, bool);
}

