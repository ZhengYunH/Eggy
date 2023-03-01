#pragma once
#include "Core/Config.h"
#include "Graphics/RHI/IShaderRenderResource.h"
#include "D3D11Common.h"
#include "Core/System/ModuleSystem.h"


namespace Eggy
{
	struct D3D11Shader : public IDevcieResource
	{
		D3D11Shader(ShaderRenderResource* shader);
		virtual ~D3D11Shader() {}

		String HLSLPath;
		String CSOPath;
		String EntryPoint{ "Unknown" };
		String ShaderModel{ "Unknown" };
		ShaderRenderResource* Shader{ nullptr };
		TComPtr<ID3DBlob> ppBlob;
	};

	struct D3D11VertexShader : public D3D11Shader
	{
		D3D11VertexShader(ShaderRenderResource* shader);
		TComPtr<ID3D11VertexShader> ppShader;
	};

	struct D3D11PixelShader : public D3D11Shader
	{
		D3D11PixelShader(ShaderRenderResource* shader) : D3D11Shader(shader)
		{
			EntryPoint = "PS";
			ShaderModel = "ps_5_0";
		}
		TComPtr<ID3D11PixelShader> ppShader;
	};
}
