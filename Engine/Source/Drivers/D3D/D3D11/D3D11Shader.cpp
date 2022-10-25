#include "D3D11Shader.h"
#include "Drivers/D3D/D3D11/D3D11Device.h"
#include "Drivers/D3D/D3D11/D3D11Resource.h"


namespace Eggy
{
	static String GenerateDefaultCSOFilePath(String path)
	{
		auto index = path.rfind('.');
		if (index != std::string::npos)
		{
			return path.substr(0, index) + ".cso";
		}
		else
		{
			return path + ".cso";
		}
	}

	D3D11Shader::D3D11Shader(IShader* shader) : Shader(shader)
	{
		HLSLPath = shader->FilePath;
		CSOPath = GenerateDefaultCSOFilePath(HLSLPath);
	}
}

