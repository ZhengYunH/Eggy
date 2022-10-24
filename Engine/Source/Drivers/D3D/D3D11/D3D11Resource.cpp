#include "D3D11Resource.h"
#include "D3D11Device.h"


namespace Eggy
{
	void D3D11ResourceFactory::CreateInputLayout(IInputLayout* inputLayout)
	{
		UINT numElements = static_cast<UINT>(inputLayout->Descs.size());
		D3D11_INPUT_ELEMENT_DESC* layoutDescs = new D3D11_INPUT_ELEMENT_DESC[numElements];
		for (UINT i = 0; i < numElements; ++i)
		{
			auto& desc = inputLayout->Descs[i];
			layoutDescs[i] = {
				desc.SemanticName.c_str(),
				desc.SemanticIndex,
				Converter::Format(desc.Format),
				desc.InputSlot,
				desc.AlignedByteOffset,
				Converter::InputClassification(desc.SlotClass)
			};
		}
		void* pShaderBytecodeWithInputSignature = nullptr;
		SIZE_T bytecodeLength = 0;
		ID3D11InputLayout* address = (ID3D11InputLayout*)inputLayout->DeviceResource;
		HR(mDevice_->mDevice_->CreateInputLayout(
			layoutDescs, numElements, 
			pShaderBytecodeWithInputSignature, bytecodeLength,
			&address
		));
	}
}

