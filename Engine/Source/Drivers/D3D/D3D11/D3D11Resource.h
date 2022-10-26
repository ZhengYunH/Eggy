#pragma once
#include "D3D11Common.h"
#include "Core/Config.h"
#include "Graphics/RHI/IRenderHeader.h"
#include "Graphics/RHI/IRenderResource.h"
#include "Graphics/RHI/IRenderDevice.h"



namespace Eggy
{
	class D3D11Device;
	struct D3D11Shader;
	struct IShaderCollection;


	struct D3D11InputLayout 
	{
		D3D11InputLayout(IInputLayout* inputLayout): InputLayout(inputLayout)
		{}

		IInputLayout* InputLayout;
		TComPtr<ID3D11InputLayout> ppAddress;
	};

	struct D3D11Buffer
	{
		D3D11Buffer(IBuffer* buffer): Buffer(buffer)
		{}

		IBuffer* Buffer;
		TComPtr<ID3D11Buffer> ppBuffer;
	};

	class D3D11ResourceFactory : public IRenderResourceFactory
	{
	public:
		D3D11ResourceFactory(class D3D11Device* device) : mD3D11Device_(device)
		{
		}

		void CreateInputLayout(IInputLayout* inputLayout, IShaderCollection* shaderCollection);
		void CreateShader(IShader* shader);
		void CreateBuffer(IBuffer* buffer);

	protected:
		void CreateShaderFromFile(D3D11Shader* deviceShader);
		void CreateVertexShader(D3D11Shader* deviceShader);
		void CreatePixelShader(D3D11Shader* deviceShader);


	protected:
		class D3D11Device* mD3D11Device_;
	};
}