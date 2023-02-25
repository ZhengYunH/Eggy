#pragma once
#include "ResourceHeader.h"

namespace Eggy
{
	enum EResourceState : uint32
	{
		Pending = 0,
		Loading = 1 << 0,
		Loaded = 1 << 1,
		Releasing = 1 << 2,
		Released = 1 << 3,
		LoadingState = Pending | Loading | Loaded | Releasing | Released
	};

	class IFile;
	class ResourceItem;
	class ResourceObject : public IRefObject
	{
		friend class ResourceModule;
	public:
		virtual bool LoadObject() noexcept { return false; };
		virtual bool SaveObject() noexcept { return false; };
		virtual void AddDependency(TSharedPtr<ResourceObject>& dep);
		virtual void ResetDependency();

	public:
		ResourceObject() = default;
		virtual ~ResourceObject() = default;

	public:
		void SetDependency(List<TSharedPtr<ResourceObject>>& deps);
		void AcquireLoad();
		void AddLoadedCallback(TLoadedCallback cb);
		void SetLoaded();
		bool IsLoaded() { return mLoadingState_ & EResourceState::Loaded; }
		ResourceItem* GetItem() const { return mItem_; }
	
	protected:
		ResourceItem* mItem_{ nullptr };
		List<TSharedPtr<ResourceObject>> mDeps_;
		EResourceState mLoadingState_{ EResourceState::Pending };
		List<TLoadedCallback> mLoadedCallback_;
	};
	using ResourceObjectFactory = TFactory<ResourceObject, EResourceType>;

#define DeclareResource(_RESOURCE, _TYPE) private: static TProductRegistrar<ResourceObject, _RESOURCE, EResourceType> __Registrar
#define DefineResource(_RESOURCE, _TYPE) TProductRegistrar<ResourceObject, _RESOURCE, EResourceType> _RESOURCE::__Registrar(_TYPE)
}

