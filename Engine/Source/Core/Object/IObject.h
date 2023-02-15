#pragma once
#include "Core/Config.h"
#include "Core/Reflection/Reflection.h"
#include "Core/DataType/WeakPtr.h"


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
		void Acquire();
		void Release();

	private:
		std::atomic<uint32> mRef_{ 0 };
		class Ghost* mGhost_;
	};

	class Ghost : TWeakPtr<IObject>
	{
	public:
		~Ghost() {}
	};

}