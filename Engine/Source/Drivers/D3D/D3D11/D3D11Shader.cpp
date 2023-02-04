#include "D3D11Shader.h"
#include "Drivers/D3D/D3D11/D3D11Device.h"
#include "Drivers/D3D/D3D11/D3D11Resource.h"


namespace Eggy
{
	D3D11Shader::D3D11Shader(IShader* shader) : Shader(shader)
	{
		HLSLPath = shader->FilePath;
		CSOPath = IShader::GetCacheFilePath(HLSLPath);
	}
}

