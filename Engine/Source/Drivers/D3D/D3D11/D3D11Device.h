#include "Core/Config.h"
#include "Graphics/RHI/IRenderDevice.h"

#include <d3d11_1.h>
#include <d3dcommon.h>
#include <wrl/client.h>



namespace Eggy
{
	class D3D11Device : public IRenderDevice
	{
		template <class T>
		using TComPtr = Microsoft::WRL::ComPtr<T>;

	public:
		D3D11Device();
		~D3D11Device();
	
	public:
		void DrawFrame() override final;

	private:
		void CreateSwapChain() override final;
		void OnResize() override final;
		

	private:
		void ClearScreen();
		void Present();


	private:
		DXGI_FORMAT mSwapchainFormat_{ DXGI_FORMAT_R8G8B8A8_UNORM };
		DXGI_FORMAT mDepthStencilFormat_{ DXGI_FORMAT_D24_UNORM_S8_UINT };

		UINT m4xMsaaQaulity_{ 0 };
		bool mEnable4xMsaa_{ true };

	private:
		// Direct3D 11
		TComPtr<ID3D11Device> mDevice_;
		TComPtr<ID3D11DeviceContext> mImmediateContext_;
		TComPtr<IDXGISwapChain> mSwapChain_;

		// Direct3D 11.1
		TComPtr<ID3D11Device1> mDevice1_;
		TComPtr<ID3D11DeviceContext1> mImmediateContext1_;
		TComPtr<IDXGISwapChain1> mSwapChain1_;

		HWND      mMainWnd_{ nullptr };

		TComPtr<ID3D11Texture2D> mDepthStencilBuffer_;
		TComPtr<ID3D11RenderTargetView> mRenderTargetView_;
		TComPtr<ID3D11DepthStencilView> mDepthStencilView_;
		D3D11_VIEWPORT mScreenViewport_;
	};
}