#pragma once
#include "ResourceHeader.h"


namespace Eggy
{
	class ResourceItem
	{
		friend class ResourceModule;
	public:
		ResourceItem(Guid id, ResourceObject* object, const String& path)
			: mResourceID_(id)
			, mResourceObject_(object)
			, mPath_(path)
		{
		}

		void SetDependency(List<Guid>&& deps)
		{
			mDeps_ = deps;
		}

		
	public:
		EResourceType GetType() const { return mType_; }
		Guid GetResourceID() const { return mResourceID_; }
		TSharedPtr<ResourceObject>& GetResourceObject() { return mResourceObject_; }
		String GetPath() const { return mPath_; }

	protected:
		EResourceType mType_;
		Guid mResourceID_;
		TSharedPtr<ResourceObject> mResourceObject_{ nullptr };
		List<Guid> mDeps_;
		String mPath_;
	};

	using ResourItemFactory = TSimpleFactory<ResourceItem>;

}