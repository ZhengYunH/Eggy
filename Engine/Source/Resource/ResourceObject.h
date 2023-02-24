#pragma once
#include "ResourceHeader.h"


namespace Eggy
{
	class IFile;
	class ResourceItem;
	class ResourceObject
	{
		friend class ResourceModule;
	public:
		ResourceObject() = default;
		void AddDependency(TSharedPtr<ResourceObject>&& dep);
		void SetDependency(List<TSharedPtr<ResourceObject>>& deps);
		void ResetDependency();
		void LoadResource();
		virtual void Deserialize(IFile* file) noexcept = 0;

	protected:
		ResourceItem* mItem_;
		List<TSharedPtr<ResourceObject>> mDeps_;
	};
	using ResourceObjectFactory = TFactory<ResourceObject, EResourceType>;
}

