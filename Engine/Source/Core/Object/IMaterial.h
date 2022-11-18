#pragma once
#include "Core/Config.h"
#include "Core/Engine/Resource/Material.h"


namespace Eggy
{
	class Material
	{
	public:
		void SetResource(MaterialResource* Resource)
		{
			mResource_ = Resource;
		}

		String GetShaderPath(EShaderType shaderType)
		{
			switch (shaderType)
			{
			case Eggy::EShaderType::None:
				Unimplement();
				break;
			case Eggy::EShaderType::VS:
				return "Engine/Shader/HLSL/" + mResource_->mShader_ + "_VS.hlsl";
				break;
			case Eggy::EShaderType::PS:
				return "Engine/Shader/HLSL/" + mResource_->mShader_ + "_PS.hlsl";
				break;
			default:
				break;
			}
			return "";
		}

	protected:
		MaterialResource* mResource_;
	};
}