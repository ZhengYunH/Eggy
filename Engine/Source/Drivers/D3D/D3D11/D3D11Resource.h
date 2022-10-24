#pragma once
#include "Core/Config.h"
#include "Graphics/RHI/IRenderResource.h"
#include <dxgiformat.h>
#include <d3d11.h>


namespace Eggy
{
	struct Converter
	{
		static DXGI_FORMAT Format(EPixelFormat format)
		{
			switch (format)
			{
			case EPixelFormat::UNDEFINED:
				Unimplement();
				break;
			case EPixelFormat::R32G32B32:
				return DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
				break;
			case EPixelFormat::A32R32G32B32F:
				return DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
				break;
			case EPixelFormat::A16B16G16R16F:
				return DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT;
				break;
			case EPixelFormat::R8G8B8A8:
				return DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM;
				break;
			case EPixelFormat::D32_SFLOAT_S8_UINT:
				return DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
				break;
			default:
				Unimplement();
				break;
			}
			return DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
		}

		static D3D11_INPUT_CLASSIFICATION InputClassification(EInputClassification classification)
		{
			switch (classification)
			{
			case EInputClassification::None:
				Unimplement();
				break;
			case EInputClassification::PER_VERTEX:
				return D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
				break;
			case EInputClassification::PER_INSTANCE:
				return D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_INSTANCE_DATA;
				break;
			default:
				Unimplement();
				break;
			}
			return D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
		}
	};

	class D3D11Device;

	class D3D11ResourceFactory
	{
	public:
		void CreateInputLayout(IInputLayout* inputLayout);

	protected:
		class D3D11Device* mDevice_;
	};
}