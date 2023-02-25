#include "ResourceObject.h"
#include "ResourceModule.h"
#include "ResourceItem.h"


namespace Eggy
{
	void ResourceObject::AddDependency(TSharedPtr<ResourceObject>& dep)
	{
		mDeps_.push_back(dep);
	}

	void ResourceObject::SetDependency(List<TSharedPtr<ResourceObject>>& deps)
	{
		ResetDependency();
		for (auto& dep : deps)
			AddDependency(dep);
	}

	void ResourceObject::ResetDependency()
	{
		mDeps_.clear();
	}

	void ResourceObject::AcquireLoad()
	{
		ResourceModule::Instance().AcquireResource(this);
	}

	void ResourceObject::AddLoadedCallback(TLoadedCallback cb)
	{
		if (!cb)
			return;

		if (IsLoaded())
			cb();
		else
			mLoadedCallback_.emplace_back(std::move(cb));
	}

	void ResourceObject::SetLoaded()
	{
		HYBRID_CHECK(!IsLoaded());
		mLoadingState_ = (EResourceState)((mLoadingState_ & ~EResourceState::LoadingState) | EResourceState::Loaded);
		for (auto& cb : mLoadedCallback_)
			cb();
		mLoadedCallback_.clear();
	}

}

