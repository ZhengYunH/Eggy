#pragma once

#define EnableReflection(ClassName)
#define EFunction(...)
#define EProperty(...)

#define IMP_GETCLASS(TPrimitive) \
static ClassType* GetClass() \
{\
	return static_cast<ClassType*>(IObject::GetClassInfo(#TPrimitive)); \
}
