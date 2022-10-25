#pragma once
#include "Core/Config.h"
#include "Graphics/RHI/IShader.h"
#include "D3D11Common.h"


namespace Eggy
{
	struct D3D11Shader
	{
		D3D11Shader(IShader* shader);
		virtual ~D3D11Shader() {}

		String HLSLPath;
		String CSOPath;
		String EntryPoint{ "Unknown" };
		String ShaderModel{ "Unknown" };
		IShader* Shader{ nullptr };
		TComPtr<ID3DBlob> ppBlob;
	};

	struct D3D11VertexShader : public D3D11Shader
	{
		D3D11VertexShader(IShader* shader) : D3D11Shader(shader)
		{
			EntryPoint = "VS";
			ShaderModel = "vs_5_0";
		}
		TComPtr<ID3D11VertexShader> ppShader;
	};

	struct D3D11PixelShader : public D3D11Shader
	{
		D3D11PixelShader(IShader* shader) : D3D11Shader(shader)
		{
			EntryPoint = "PS";
			ShaderModel = "ps_5_0";
		}
		TComPtr<ID3D11PixelShader> ppShader;
	};
}
