#pragma once
#include "Core/Config.h"
#include "Graphics/RHI/IRenderResource.h"
#include "Core/System/FileSystem.h"


namespace Eggy
{
	struct ShaderMacroDefine
	{
		String Name;
		String Value;
	};

	struct ShaderRenderResource : public IRenderResource
	{
		ShaderRenderResource(class ShaderStageInstance* instance);

		virtual void CreateDeviceResource_Impl(IRenderResourceFactory* factory) override
		{
			if (IsResourceCreated())
				return;
			factory->CreateShader(this);
			for (size_t i = 0; i < Samplers.size(); ++i)
				Samplers[i]->CreateDeviceResource(factory);
		}

		bool NeedCompile()
		{
			return !FileSystem::Get()->FileExist(FilePath);
		}

		static String GetAbsShaderFilePath(String shaderPath);
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

		EShaderStage Stage;
		String FilePath;
		String EntryPoint{ "Unknown" };
		List<SamplerState*> Samplers;
		List<ShaderMacroDefine> Macros;
	};
}