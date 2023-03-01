#include "D3D11Shader.h"
#include "Drivers/D3D/D3D11/D3D11Device.h"
#include "Drivers/D3D/D3D11/D3D11Resource.h"


namespace Eggy
{
	D3D11Shader::D3D11Shader(ShaderRenderResource* shader) : Shader(shader)
	{
		HLSLPath = ShaderRenderResource::GetAbsShaderFilePath(shader->FilePath);
		CSOPath = ShaderRenderResource::GetCacheFilePath(HLSLPath);

	}

	D3D11VertexShader::D3D11VertexShader(ShaderRenderResource* shader) : D3D11Shader(shader)
	{
		EntryPoint = "VS";
		ShaderModel = "vs_5_0";
	}
	
}

