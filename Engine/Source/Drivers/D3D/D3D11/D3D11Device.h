#include "D3D11Common.h"
#include "Core/Config.h"
#include "Graphics/RHI/IRenderDevice.h"
#include "D3D11Resource.h"

namespace Eggy
{
	class D3D11Device : public IRenderDevice
	{
		friend class D3D11ResourceFactory;

	public:
		D3D11Device();
		~D3D11Device();
	
		void* GetDevice() override;
		void* GetContext() override;
		IRenderTarget* GetBackBuffer() override { return mBackBuffers_[mCurrentFrameIndex_]; }
		D3D11ResourceFactory* GetResourceFactory() override { return mResourceFactory_; }

	public:
		void PrepareResource() override final;
		void DrawFrame() override final;

	protected:
		void EncodeRenderPass(class RenderPass* renderPass);
		void EncodeDrawCall(struct DrawCall* drawCall);

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
		D3D11ResourceFactory* mResourceFactory_;

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

		TComPtr<ID3D11DepthStencilView> mDepthStencilView_;
		D3D11_VIEWPORT mScreenViewport_;

		IRenderTarget** mBackBuffers_;
		IRenderTarget** mDepthStencilBuffers_;

		UINT mBufferCount_{ 1 };
		UINT mCurrentFrameIndex_{ 0 };
	};
}