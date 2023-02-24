#include "ResourceObject.h"
#include "ResourceModule.h"

namespace Eggy
{
	void ResourceObject::AddDependency(TSharedPtr<ResourceObject>&& dep)
	{
		mDeps_.push_back(std::move(dep));
	}

	void ResourceObject::SetDependency(List<TSharedPtr<ResourceObject>>& deps)
	{
		mDeps_ = deps;
	}

	void ResourceObject::ResetDependency()
	{
		mDeps_.clear();
	}

	void ResourceObject::LoadResource()
	{
		ResourceModule::Instance().AcquireResource(this);
	}

}

