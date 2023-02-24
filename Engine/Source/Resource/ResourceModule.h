#pragma once
#include "ResourceHeader.h"


namespace Eggy
{
	class ResourceItem;
	class ResourceObject;

	class ResourceModule : public TSingleton<ResourceModule>
	{
	public:
		TSharedPtr<ResourceObject> Load(Guid resourceID);
		ResourceItem* GetItem(Guid resourceID);
		void AcquireResource(TSharedPtr<ResourceObject>&& resourceObject);

		void Tick_rst();
	protected:
		Map<Guid, ResourceItem*> mItems_;
		uint8 mLoadResourcePerframe_{ 0 };
		List<TSharedPtr<ResourceObject>> mLoadingQueue_;
	};

}