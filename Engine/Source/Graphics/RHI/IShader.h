#pragma once
#include "Core/Config.h"
#include "Graphics/RHI/IRenderResource.h"
#include "Core/System/FileSystem.h"


namespace Eggy
{
	enum EShaderConstant
	{
		Batch = 0,
		Shader,
		Global,
		END
	};

	struct IShader : public IRenderResource
	{
		IShader(EShaderType type, const String& filePath) : Type(type), FilePath(filePath)
		{
			Samplers.resize(1);
		}

		virtual void CreateDeviceResource_Impl(IRenderResourceFactory* factory) override
		{
			if (IsResourceCreated())
				return;
			factory->CreateShader(this);
			for (size_t i = 0; i < Samplers.size(); ++i)
			{
				if (!Samplers[i])
					Samplers[i] = new SamplerState();
				Samplers[i]->CreateDeviceResource(factory);
			}
		}

		bool NeedCompile()
		{
			return !FileSystem::Get()->FileExist(FilePath);
		}

		static String GetCacheFilePath(String path);

		bool IsResourceCreated() override
		{
			if (!IRenderResource::IsResourceCreated())
				return false;
			for (SamplerState* ss : Samplers)
			{
				if (ss && !ss->IsResourceCreated())
				{
					return false;
				}
			}
			return true;
		}

		EShaderType Type;
		String FilePath;
		List<SamplerState*> Samplers;
	};

	struct IShaderCollection : public IRenderResource
	{
	public:
		static const uint8 INVALID_SLOT = 0xff;

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

		uint8 GetTextureSlot(String name)
		{
			return 0;
		}

		uint8 GetTextureSize()
		{
			return 1;
		}

		uint8 GetConstantSlot(EShaderConstant esc)
		{
			switch (esc)
			{
			case Eggy::Batch:
				return GetConstantSize() > 0 ? 0 : INVALID_SLOT;
			case Eggy::Shader:
				return GetConstantSize() > 1 ? 1 : INVALID_SLOT;
			case Eggy::Global:
				return GetConstantSize() > 2 ? 2 : INVALID_SLOT;
			default:
				Unimplement(0);
			}
			return INVALID_SLOT;
		}
		
		uint8 GetConstantSize()
		{
			return uint8(EShaderConstant::END);
		}


	protected:
		Map<EShaderType, IShader*> mAllShader_;
		bool mIsResourceCreated_{ false };
	};
	
	
}