#pragma once
#include "Core/Config.h"
#include "Core/Misc/Guid.h"


namespace Eggy
{
	class IModule
	{

	};

	enum class EResourceType
	{
		Model,
		Texture,
		Material,
		Mesh,
		End
	};

	class ResourceObject;

	class ResourceItem
	{
		friend class ResourceFactory;
	public:
		ResourceItem(Guid id, ResourceObject* object, const String& path) 
			: mResourceID_(id)
			, mResourceObject_(object)
			, mPath_(path)
		{
		}

		void SetDependency()
		{

		}

	public:
		Guid GetResourceID() const { return mResourceID_; }
		ResourceObject* GetResourceObject() const { return mResourceObject_; }
		String GetPath() const { return mPath_; }

	protected:
		Guid mResourceID_;
		ResourceObject* mResourceObject_{ nullptr };
		String mPath_;
		List<Guid> mDeps_;
	};

	class IRefObject
	{
	public:
		void Acquire()
		{
			mRef_.fetch_add(1);
		}

		void Release()
		{
			if (mRef_.fetch_sub(1) == 0)
			{
				delete this;
			}
		}
	
	protected:
		std::atomic<uint32> mRef_;
	};

	template<typename T>
	concept CRef = requires(T * a)
	{
		{a->Acquire()} -> std::same_as<void>;
		{a->Release()} -> std::same_as<void>;
	};

	template<CRef _Type>
	class TSharedPtr
	{

	};

	class ResourceObject : public IRefObject
	{
		friend class ResourceFactory;
	public:
		ResourceObject() = default;
		void AddDependency(ResourceObject* dep);
		void SetDependency(List<ResourceObject*>& deps);
		void ResetDependency();

	protected:
		ResourceItem* mItem_;
		List<ResourceObject*> mDeps_;
	};

	class ResourceFactory
	{
	public:
		ResourceItem* NewResourceItem(ResourceObject* object, String path)
		{
			return new ResourceItem(Guid::Generate(), object, path);
		}

		ResourceObject* NewResourceObject(EResourceType type)
		{

		}
	};

	class ResourceModule : IModule
	{
		void Load(Guid resourceID);
	};

}