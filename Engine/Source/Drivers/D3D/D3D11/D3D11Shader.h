#pragma once
#include "Core/Config.h"
#include "Graphics/RHI/IShader.h"
#include "D3D11Common.h"
#include "Core/System/ModuleSystem.h"


namespace Eggy
{
	struct D3D11Shader : public IDevcieResource
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
		D3D11VertexShader(IShader* shader);
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

	namespace ShaderReflect
	{
		struct ShaderDesc
		{
			uint8 N_ConstantBuffers{ 0 };
			uint8 N_BoundResources{ 0 };
		};

		struct IShaderReflect
		{
			// virtual void GetShaderDesc(ShaderDesc& desc) = 0;
		};
	}

	struct D3D11ShaderReflect : ShaderReflect::IShaderReflect
	{
		static ModuleHandle GetCompilerModule()
		{
			const String dllName = "ShaderConductor";
			HYBRID_CHECK(ModuleSystem::Get()->LoadModule(dllName, EModuleType::External));
			return ModuleSystem::Get()->GetModule(dllName);
		}

		D3D11ShaderReflect(D3D11Shader& shader)
		{
			// GetCompilerModule();

			using namespace ShaderConductor;
			Compiler::SourceDesc source;
			source.entryPoint = shader.EntryPoint.c_str();
			MacroDefine* defines = nullptr;
			if (shader.Shader->Macros.size() > 0)
			{
				defines = new MacroDefine[shader.Shader->Macros.size()];
				for (size_t i = 0; i < shader.Shader->Macros.size(); ++i)
				{
					defines[i].name = shader.Shader->Macros[i].Name.c_str();
					defines[i].value = shader.Shader->Macros[i].Value.c_str();
				}
				source.defines = defines;
			}
			
			source.fileName = shader.HLSLPath.c_str();
			source.numDefines = (uint32)shader.Shader->Macros.size();
			source.stage = shader.Shader->Type == EShaderType::PS ? ShaderStage::PixelShader : ShaderStage::VertexShader;
			source.source = "";


			Compiler::Options options;
			Compiler::TargetDesc desc;
			desc.language = ShaderConductor::ShadingLanguage::SpirV;
			desc.version = "";
			desc.asModule = false;

			ShaderConductor::Compiler::Compile(source, options, desc);

			/*for (size_t i = 0; i < shader.Shader->Macros.size(); ++i)
			{
				delete defines[i];
			}*/
			SafeDestroy(defines);
		}
	};
}
