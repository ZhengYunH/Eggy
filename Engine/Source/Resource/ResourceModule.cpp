#include "ResourceModule.h"
#include "ResourceItem.h"
#include "ResourceObject.h"

namespace Eggy
{
	TSharedPtr<ResourceObject> ResourceModule::Load(Guid resourceID)
	{
		ResourceItem* item = GetItem(resourceID);
		if (!item)
			return nullptr;

		if (item->mResourceObject_)
			return item->mResourceObject_;

		ResourceObject* resourceObject = ResourceObjectFactory::Instance().Create(item->mType_);
		for (const Guid& dep : item->mDeps_)
		{
			TSharedPtr<ResourceObject> depResourceObject = Load(dep);
			resourceObject->AddDependency(std::move(depResourceObject));
		}
		resourceObject->LoadResource();
		item->mResourceObject_ = resourceObject;
		return item->mResourceObject_;
	}

	ResourceItem* ResourceModule::GetItem(Guid resourceID)
	{
		auto itr = mItems_.find(resourceID);
		if (itr != mItems_.end())
			return itr->second;
		return nullptr;
	}

	void ResourceModule::AcquireResource(TSharedPtr<ResourceObject>&& resourceObject)
	{
		mLoadingQueue_.push_back(std::move(resourceObject));
	}

	void ResourceModule::Tick_rst()
	{
		if (mLoadingQueue_.empty())
			return;
		List<TSharedPtr<ResourceObject>> tempQueue;
		if (mLoadResourcePerframe_ == 0 || mLoadResourcePerframe_ >= mLoadingQueue_.size())
		{
			tempQueue.swap(mLoadingQueue_);
		}
		else
		{
			tempQueue.insert(tempQueue.end(), mLoadingQueue_.begin(), mLoadingQueue_.begin() + mLoadResourcePerframe_);
			mLoadingQueue_.erase(mLoadingQueue_.begin(), mLoadingQueue_.begin() + mLoadResourcePerframe_);
			mLoadingQueue_.shrink_to_fit();
		}
		for (auto& resourceObjectRef : tempQueue)
		{
		}
	}

}

