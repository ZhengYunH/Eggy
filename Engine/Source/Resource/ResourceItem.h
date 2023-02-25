#pragma once
#include "ResourceHeader.h"


namespace Eggy
{
	class ResourceObject;
	class ResourceItem
	{
		friend class ResourceModule;
	public:
		ResourceItem(Guid id, EResourceType type, const String& path)
			: mResourceID_(id)
			, mType_(type)
			, mPath_(path)
		{

		}

		ResourceItem(Guid id, String type, const String& path)
			: ResourceItem(id, s_ResourceTypeMap[type], path)
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
		EResourceType mType_{ EResourceType::None };
		Guid mResourceID_;
		TSharedPtr<ResourceObject> mResourceObject_{ nullptr };
		List<Guid> mDeps_;
		String mPath_;
	};

	using ResourItemFactory = TSimpleFactory<ResourceItem>;

}