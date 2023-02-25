#include "ResourceModule.h"
#include "ResourceItem.h"
#include "ResourceObject.h"
#include "System/FileSystem.h"


namespace Eggy
{
	ResourceModule::ResourceModule()
	{
		FPath packageRoot = FileSystem::Get()->GetPackageRoot();
		List<String> repos = FileSystem::Get()->GetSubDirectories(packageRoot);
		for (auto& repo : repos)
		{
			FPath repoPath = packageRoot + repo;
			String metaFile = repo + ".meta.xml";
			FPath metaPath = repoPath + metaFile;
			XMLFile* meta = dynamic_cast<XMLFile*>(FileSystem::Get()->LoadFile(metaPath.ToString()).get());
			if (meta)
			{
				auto root = meta->GetRootNode();
				auto _Repository = root.get_child("Repository");
				BOOST_FOREACH(pt::ptree::value_type & v, _Repository.get_child("Items"))
				{
					auto _Item = v.second;
					String type = _Item.get<String>("Type");
					Guid guid = Guid::FromString(_Item.get<String>("GUID"));
					String path = _Item.get<String>("Path");
					ResourceItem* item = ResourItemFactory::Instance().Create(guid, type,path);
					HYBRID_CHECK(mItems_.find(guid) == mItems_.end());
					List<Guid> deps;
					BOOST_FOREACH(pt::ptree::value_type & v, _Item.get_child("Deps"))
					{
						auto _Dep = v.second;
						deps.emplace_back(Guid::FromString(_Dep.get_value<String>()));
					}
					item->SetDependency(std::move(deps));
					mItems_.insert({ guid, item });
					mPathItems_.insert({ path, item });
				}
			}
		}
	}

	TSharedPtr<ResourceObject> ResourceModule::Load(Guid resourceID, TLoadedCallback callback)
	{
		ResourceItem* item = GetItem(resourceID);
		if (!item)
			return nullptr;

		if (item->mResourceObject_)
		{
			item->mResourceObject_->AddLoadedCallback(callback);
			return item->mResourceObject_;
		}

		ResourceObject* resourceObject = ResourceObjectFactory::Instance().Create(item->mType_);
		resourceObject->mItem_ = item;
		for (const Guid& dep : item->mDeps_)
		{
			TSharedPtr<ResourceObject> depResourceObject = Load(dep);
			resourceObject->AddDependency(depResourceObject);
		}
		resourceObject->AcquireLoad();
		resourceObject->AddLoadedCallback(callback);
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

	ResourceItem* ResourceModule::GetItem(String path)
	{
		auto itr = mPathItems_.find(path);
		if (itr != mPathItems_.end())
			return itr->second;
		return nullptr;
	}

	void ResourceModule::AcquireResource(TSharedPtr<ResourceObject>&& resourceObject)
	{
		mLoadingQueue_.push_back(std::move(resourceObject));
	}

	void ResourceModule::Tick_rst()
	{
		for (auto& cb : mCallback_)
		{
			cb();
		}
		mCallback_.clear();

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
		for (auto& resourceObject : tempQueue)
		{
			_InternalLoadResource(resourceObject);
		}
	}

	void ResourceModule::_InternalLoadResource(TSharedPtr<ResourceObject>& resourceObject)
	{
		resourceObject->LoadObject();
	}

}

