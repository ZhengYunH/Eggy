#pragma once
#include "ResourceHeader.h"


namespace Eggy
{
	class ResourceItem;
	class ResourceObject;

	class ResourceModule : public TSingleton<ResourceModule>
	{
	public:
		ResourceModule();

		TSharedPtr<ResourceObject> Load(Guid resourceID, TLoadedCallback callback=nullptr);
		ResourceItem* GetItem(Guid resourceID);
		ResourceItem* GetItem(String path);

		void AcquireResource(TSharedPtr<ResourceObject>&& resourceObject);

		void AddCallback(TLoadedCallback cb) { mCallback_.emplace_back(cb); }
		void Tick_rst();

	protected:
		void _InternalLoadResource(TSharedPtr<ResourceObject>& resourceObject);

	protected:
		Map<Guid, ResourceItem*> mItems_;
		Map<String, ResourceItem*> mPathItems_;
		uint8 mLoadResourcePerframe_{ 0 };
		List<TSharedPtr<ResourceObject>> mLoadingQueue_;
		List<TLoadedCallback> mCallback_;
	};

}