#include "Reflection.h"

namespace Eggy
{
	Map<Name, IType*> Reflection::GRegistrationDict;

#define DEFINE_INTRINSIC_TYPE(Name, Type)  \
	const char __StrIntrinsicType__##Name[] = #Name; \
	template<> \
	struct IntrinsicType<Type, __StrIntrinsicType__##Name> \
	{ \
		struct IntrinsicTypeRegister \
		{ \
			IntrinsicTypeRegister() { Reflection::RegisterType<Type, __StrIntrinsicType__##Name>(); } \
		}; \
		static IntrinsicTypeRegister __mRegister; \
	};\
	IntrinsicType<Type, __StrIntrinsicType__##Name>::IntrinsicTypeRegister IntrinsicType<Type, __StrIntrinsicType__##Name>::__mRegister;

	DEFINE_INTRINSIC_TYPE(IntegerType, int);
	DEFINE_INTRINSIC_TYPE(FloatType, float);
	DEFINE_INTRINSIC_TYPE(BoolType, bool);
}