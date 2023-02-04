#pragma once
#include "Core/Config.h"
#include "Graphics/RHI/IRenderResource.h"
#include "Core/System/FileSystem.h"


namespace Eggy
{
	struct IShader : public IRenderResource
	{
		IShader(EShaderType type, const String& filePath) : Type(type), FilePath(filePath)
		{
		}

		virtual void CreateDeviceResource_Impl(IRenderResourceFactory* factory) override
		{
			if (IsResourceCreated())
				return;
			factory->CreateShader(this);
		}

		bool NeedCompile()
		{
			return FileSystem::Get()->FileExist(FilePath);
		}

		static String GetCacheFilePath(String path);

		EShaderType Type;
		String FilePath;
	};

	struct IShaderCollection : public IRenderResource
	{
	public:
		IShaderCollection()
		{
		}

		void SetShaderPath(EShaderType type, const String& path, bool overwrite=false)
		{
			auto itr = mAllShader_.find(type);
			if (itr != mAllShader_.end())
			{
				if (overwrite)
					delete itr->second;
				else
					return;
			}
			mAllShader_[type] = new IShader(type, path);
		}

		virtual void CreateDeviceResource_Impl(IRenderResourceFactory* factory) override
		{
			if (IsResourceCreated())
				return;
			
			for (auto pair : mAllShader_)
			{
				pair.second->CreateDeviceResource(factory);
			}
			mIsResourceCreated_ = true;
		}

		virtual IShader* GetShader(EShaderType type) 
		{
			HYBRID_CHECK(mAllShader_.find(type) != mAllShader_.end());
			return mAllShader_.at(type);
		}
		
		virtual bool IsResourceCreated() { return mIsResourceCreated_; }

	protected:
		Map<EShaderType, IShader*> mAllShader_;
		bool mIsResourceCreated_{ false };
	};
	
	
}