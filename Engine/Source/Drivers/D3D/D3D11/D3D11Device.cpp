#include "D3D11Device.h"
#include "d3d11.h"
#include "dxgi.h"
#include "d3dcommon.h"

// reference: https://www.cnblogs.com/X-Jun/p/9069608.html

namespace Eggy
{
	// IRenderDevice* GRenderDevice = new D3D11Device();

	D3D11Device::D3D11Device()
	{
		// IDXGIFactory::EnumAdapters();

		IDXGIAdapter* pAdapter = nullptr;

		D3D_DRIVER_TYPE driverTypes[] =
		{
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_SOFTWARE
		};
		UINT numDriverTypes = ARRAYSIZE(driverTypes);

#if DEBUG_MODE
		UINT Flags = D3D11_CREATE_DEVICE_DEBUG;
#else
		UINT Flags = D3D11_CREATE_DEVICE_SINGLETHREADED;
#endif

		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
		};
		UINT numFeatureLevels = ARRAYSIZE(featureLevels);

		D3D_DRIVER_TYPE driverType;
		D3D_FEATURE_LEVEL featureLevel;

		ID3D11Device** ppDevice = nullptr;
		ID3D11DeviceContext** ppImmediateContext = nullptr;

		HRESULT hr = S_OK;
		for (UINT driverTypeIdx = 0; driverTypeIdx < numDriverTypes; driverTypeIdx++)
		{
			driverType = driverTypes[driverTypeIdx];
			hr = D3D11CreateDevice(pAdapter, driverType, nullptr, Flags, featureLevels, numFeatureLevels - 1, 
				D3D11_SDK_VERSION, ppDevice, &featureLevel, ppImmediateContext);

			if (hr == S_FALSE)
			{
				hr = D3D11CreateDevice(pAdapter, driverType, nullptr, Flags, featureLevels, numFeatureLevels - 1,
					D3D11_SDK_VERSION, ppDevice, &featureLevel, ppImmediateContext);
			}

			if (hr == S_OK)
				break;
		}

		if (FAILED(hr))
		{
			MessageBox(0, L"D3D11CreateDevice Failed.", 0, 0);
			return;
		}

		if (featureLevel != D3D_FEATURE_LEVEL_11_0 && featureLevel != D3D_FEATURE_LEVEL_11_1)
		{
			MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
			return;
		}

		UINT MsaaQaulity4X;
		(*ppDevice)->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8_UNORM, 4, &MsaaQaulity4X);
		HYBRID_CHECK(MsaaQaulity4X > 0);
	}

}

