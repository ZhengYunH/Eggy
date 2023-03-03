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

	DEFINE_INTRINSIC_TYPE(Vector2Type, Vector2);
	DEFINE_INTRINSIC_TYPE(Vector3Type, Vector3);
	DEFINE_INTRINSIC_TYPE(Vector4Type, Vector4);

	DEFINE_INTRINSIC_TYPE(Matrix3x3Type, Matrix3x3);
	DEFINE_INTRINSIC_TYPE(Matrix4x3Type, Matrix4x3);
	DEFINE_INTRINSIC_TYPE(Matrix4x4Type, Matrix4x4);
}

