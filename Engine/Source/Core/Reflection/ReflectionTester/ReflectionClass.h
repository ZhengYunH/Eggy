#include "../Reflection.h"
#include <iostream>

namespace Eggy
{
	class ReflectionClassTester : public IObject
	{
	public:
		class ReflectionClassTesterClassInfo
		{
		public:
			ReflectionClassTesterClassInfo()
			{
				Reflection::Class<ReflectionClassTester>("ReflectionClassTester", nullptr, nullptr);
			}
		};
		static ReflectionClassTesterClassInfo __mClassInfo;
		static const ClassInfo& GetClass()
		{
			return IObject::GetClassInfo("ReflectionClassTester");
		}
	};

	void DoTest();
}