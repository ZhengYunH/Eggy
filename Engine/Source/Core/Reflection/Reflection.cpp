#include "Reflection.h"

namespace Eggy
{
	Map<String, Type*> Reflection::GRegistrationDict;

	void Reflection::RegisterClass(String name, PConstuctor generator, FieldInfo* fields, uint16 numFieldInfo, FunctionInfo* functions, uint16 numFunctionInfo)
	{
		ClassType* info = new ClassType(name, generator, fields, numFieldInfo, functions, numFunctionInfo);
		AddReflectionInfo(name, info);
	}

#define DEFINE_INTRINSIC_TYPE(AliasName, Type)  \
	AliasName::IntrinsicTypeRegister AliasName::__mRegister; \

	DEFINE_INTRINSIC_TYPE(IntegerType, int);
	DEFINE_INTRINSIC_TYPE(FloatType, float);
	DEFINE_INTRINSIC_TYPE(BoolType, bool);
}