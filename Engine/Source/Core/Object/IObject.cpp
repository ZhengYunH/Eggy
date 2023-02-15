#include "IObject.h"
#include "Core/File/Archive.h"
#include "Core/Reflection/Serialization.h"

namespace Eggy
{
	String IObject::Serialize() const
	{
		return "";
	}

	void IObject::Deserialize(const String& s)
	{
		
	}

	void IObject::Acquire()
	{
		mRef_.fetch_add(1, std::memory_order_acq_rel);
	}

	void IObject::Release()
	{
		if (mRef_.fetch_sub(1, std::memory_order_acq_rel))
		{
			SafeDestroy(mGhost_);
		}
	}

}

