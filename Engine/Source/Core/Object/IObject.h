#pragma once
#include "Core/Config.h"
#include "Core/Reflection/Reflection.h"
#include "Core/DataType/WeakPtr.h"


namespace Eggy
{
	class Ghost;
	class IObject
	{
	public:
		static Type* GetClass()
		{
			return GetClassInfo("IObject");
		}

		static Type* GetClassInfo(const Name& name)
		{
			return Reflection::Instance().GetType(name);
		}

		Ghost* GetGhost() const { return mGhost_; }
	public:
		virtual String Serialize() const;
		virtual void Deserialize(const String& s);
		virtual void Initialize() noexcept {}
		virtual void Finilize() noexcept {}
		void Acquire();
		void Release();

	private:
		std::atomic<uint32> mRef_{ 0 };
		Ghost* mGhost_;
	};

	class Ghost : TWeakPtr<IObject>
	{
	public:
		~Ghost() {}
	};

}