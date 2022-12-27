#pragma once
#include "Core/Config.h"
#include "Core/Reflection/Serialization.h"


namespace Eggy
{
	class IObject;
	class IArchive
	{
	public:
		IArchive(String fileName) : mFileName_(fileName)
		{
		}

		virtual IArchive& operator << (IObject* obj) = 0;
		virtual IArchive& operator >> (IObject* obj) = 0;

	protected:
		String mFileName_;
	};

	class Archive : public IArchive
	{
	public:
		Archive& operator <<(IObject* obj) override
		{
			str += Serialization::Serialize(obj);
			return *this;
		}


		Archive& operator >>(IObject* obj) override
		{
			return *this;
		}

	protected:
		String str;
	};
}