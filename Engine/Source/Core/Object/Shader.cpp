#include "Shader.h"

namespace Eggy
{

	String Shader::GetShaderPath(EShaderType shaderType)
	{
		switch (shaderType)
		{
		case EShaderType::VS:
			return mShaderPath_ + "_VS.hlsl";
			break;
		case EShaderType::PS:
			return mShaderPath_ + "_PS.hlsl";
			break;
		default:
			Unimplement();
			break;
		}
		return "";
	}
}