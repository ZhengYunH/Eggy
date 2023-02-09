#pragma once
#include "Core/Config.h"
#include "Core/Reflection/Reflection.h"


namespace Eggy
{
	class IObject
	{
	public:
		static Type* GetClass()
		{
			return GetClassInfo("IObject");
		}

		static Type* GetClassInfo(const Name& name)
		{
			return Reflection::GRegistrationDict.find(name)->second;
		}

	public:
		virtual String Serialize() const;
		virtual void Deserialize(const String& s);
	};
}