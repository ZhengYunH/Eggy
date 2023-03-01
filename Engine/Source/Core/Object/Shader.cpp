#include "Shader.h"

namespace Eggy
{

	String Shader::GetShaderPath(EShaderStage shaderStage)
	{
		switch (shaderStage)
		{
		case EShaderStage::VS:
			return mShaderPath_ + "_VS.hlsl";
			break;
		case EShaderStage::PS:
			return mShaderPath_ + "_PS.hlsl";
			break;
		default:
			Unimplement();
			break;
		}
		return "";
	}
}