#include "D3D11Device.h"
#include "Core/Engine/Engine.h"
#include "Graphics/Elements/RenderElement.h"
#include "Graphics/RHI/IRenderPass.h"
#include "Client/RenderScene.h"
#include "D3D11Shader.h"


// just for render-test, will be deleted soon
#include "Client/ClientScene.h"


// reference: https://www.cnblogs.com/X-Jun/p/9028764.html
namespace Eggy
{
	IRenderDevice* GRenderDevice = nullptr;
	extern void CreateRenderDevice()
	{
		SafeDestroy(GRenderDevice);
		GRenderDevice = new D3D11Device();
	}

	D3D11Device::D3D11Device()
	{
		mResourceFactory_ = new D3D11ResourceFactory(this);

		// IDXGIFactory::EnumAdapters();
		IDXGIAdapter* pAdapter = nullptr;
		D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_HARDWARE;
		UINT Flags = 0;
		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
		};

		D3D_FEATURE_LEVEL featureLevel;

		HRESULT hr = S_OK;
		for (UINT i = 0; i < ARRAYSIZE(featureLevels); ++i)
		{
			hr = D3D11CreateDevice(pAdapter, driverType, nullptr, Flags, featureLevels + i, UINT(ARRAYSIZE(featureLevels) - i),
				D3D11_SDK_VERSION, mDevice_.GetAddressOf(), &featureLevel, mImmediateContext_.GetAddressOf());

			// if (SUCCEEDED(hr))
			if (SUCCEEDED(hr))
				break;
		}

		if (FAILED(hr))
		{
			MessageBox(0, L"D3D11CreateDevice Failed.", 0, 0);
			return;
		}

		if (featureLevel != D3D_FEATURE_LEVEL_11_1 && featureLevel != D3D_FEATURE_LEVEL_11_0)
		{
			MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
			return;
		}

		mDevice_->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8_UNORM, 4, &m4xMsaaQaulity_);
		HYBRID_CHECK(m4xMsaaQaulity_ > 0);

		mMainWnd_ = Engine::Get()->Platform->GetWindow();
		CreateSwapChain();
	}

	D3D11Device::~D3D11Device()
	{
		if (mImmediateContext_)
			mImmediateContext_->ClearState();
	}

	void D3D11Device::PrepareResource()
	{
	}
	
	void D3D11Device::DrawFrame()
	{
		ClearScreen();
		auto pipeline = Engine::Get()->GetClientScene()->GetRenderScene()->GetPipeline();
		for (auto renderPass : pipeline->GetRenderPasses())
			EncodeRenderPass(renderPass);
		Present();
	}

	void D3D11Device::EncodeRenderPass(RenderPass* renderPass)
	{
		DrawCall* drawCall = renderPass->GetDrawCallHead();
		while (drawCall)
		{
			EncodeDrawCall(drawCall);
			drawCall = drawCall->Next_;
		}
	}

	void D3D11Device::EncodeDrawCall(DrawCall* drawCall)
	{
		drawCall->CreateDeviceResource(GetResourceFactory());
		IRenderResource** pDataBase = drawCall->ResourceBinding_->Data;
		
		// Update Constant Buffer
		{
			for (uint16 i = 0; i < drawCall->ResourceBinding_->Buffers; ++i)
			{
				IBuffer* srcBuffer = (IBuffer*)pDataBase[i];
				if (!srcBuffer)
					continue;
				D3D11Buffer* buffer = (D3D11Buffer*)srcBuffer->DeviceResource;
				D3D11_MAPPED_SUBRESOURCE mappedData;
				mImmediateContext_->Map(buffer->ppBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
				memcpy_s(mappedData.pData, srcBuffer->ByteWidth, srcBuffer->Data, srcBuffer->ByteWidth);
				mImmediateContext_->Unmap(buffer->ppBuffer.Get(), 0);
			}
		}
		
		// VS
		{
			// Constant	
			for (uint16 i = 0; i < drawCall->ResourceBinding_->Buffers; ++i)
			{
				if (!pDataBase[i])
					continue;
				D3D11Buffer* buffer = (D3D11Buffer*)pDataBase[i]->DeviceResource;
				UINT numBuffer = 1;
				mImmediateContext_->VSSetConstantBuffers(i, numBuffer, buffer->ppBuffer.GetAddressOf());
			}

			D3D11VertexShader* vertexShader = (D3D11VertexShader*)drawCall->ShaderCollection_->GetShader(EShaderType::VS)->DeviceResource;
			mImmediateContext_->VSSetShader(vertexShader->ppShader.Get(), nullptr, 0);

		}

		// RS
		{
			mImmediateContext_->RSSetState(((D3D11PipelineState*)drawCall->Pipeline_.DeviceResource)->ppRasterizerState.Get());

		}

		// PS
		{
			auto psShader = drawCall->ShaderCollection_->GetShader(EShaderType::PS);

			// Constant
			for (uint16 i = 0; i < drawCall->ResourceBinding_->Buffers; ++i)
			{
				if (!pDataBase[i])
					continue;
				D3D11Buffer* buffer = (D3D11Buffer*)pDataBase[i]->DeviceResource;
				UINT numBuffer = 1;
				mImmediateContext_->VSSetConstantBuffers(i, numBuffer, buffer->ppBuffer.GetAddressOf());
			}

			// Texture
			IRenderResource** pTextureBase = drawCall->ResourceBinding_->Data + drawCall->ResourceBinding_->Buffers;
			List<ID3D11ShaderResourceView*> texViews;
			texViews.reserve(drawCall->ResourceBinding_->Textures);
			for (uint16 i = drawCall->ResourceBinding_->Buffers; i < drawCall->ResourceBinding_->Buffers + drawCall->ResourceBinding_->Textures; ++i)
			{
				IRenderResource* tex = pDataBase[i];
				texViews.push_back(((D3D11Texture*)(tex->DeviceResource))->ppSRV.Get());
			}
			mImmediateContext_->PSSetShaderResources(0, (UINT)texViews.size(), texViews.data());

			// Sampler
			List<ID3D11SamplerState*> samplerStates;
			HYBRID_CHECK(drawCall->ResourceBinding_->Textures == psShader->Samplers.size());
			samplerStates.reserve(psShader->Samplers.size());
			for (size_t i = 0; i < psShader->Samplers.size(); ++i)
			{
				SamplerState* sampler = psShader->Samplers[i];
				samplerStates.push_back(((D3D11SamplerState*)(sampler->DeviceResource))->ppSamplerState.Get());
			}
			mImmediateContext_->PSSetSamplers(0, (UINT)samplerStates.size(), samplerStates.data());
			
			D3D11PixelShader* pixelShader = (D3D11PixelShader*)psShader->DeviceResource;
			mImmediateContext_->PSSetShader(pixelShader->ppShader.Get(), nullptr, 0);

		}

		// Draw
		{
			// IA
			{
				// Vertex Buffer
				{
					auto& vertexBuffer = drawCall->GeometryBinding_->VertexBuffer;
					UINT startVertexLocaltion = 0;
					UINT numBuffer = 1;
					UINT stride = static_cast<UINT>(vertexBuffer.ByteWidth / vertexBuffer.Count);
					UINT offset = 0;
					mImmediateContext_->IASetVertexBuffers(startVertexLocaltion, numBuffer, ((D3D11Buffer*)vertexBuffer.DeviceResource)->ppBuffer.GetAddressOf(), &stride, &offset);

				}

				// Index Buffer
				{
					auto& indexBuffer = drawCall->GeometryBinding_->IndexBuffer;
					mImmediateContext_->IASetIndexBuffer(((D3D11Buffer*)indexBuffer.DeviceResource)->ppBuffer.Get(), Converter::IndexFormat(indexBuffer.ByteWidth / indexBuffer.Count), 0);
				}
				mImmediateContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
				mImmediateContext_->IASetInputLayout(((D3D11InputLayout*)drawCall->GeometryBinding_->Layout.DeviceResource)->ppAddress.Get());
			}

			mImmediateContext_->DrawIndexed(static_cast<UINT>(drawCall->GeometryBinding_->IndexBuffer.Count), 0, 0);
		}
	}

	void D3D11Device::CreateSwapChain()
	{
		TComPtr<IDXGIDevice> DXGIDevice = nullptr;
		TComPtr<IDXGIAdapter> DXGIAdapter = nullptr;
		TComPtr<IDXGIFactory1> DXGIFactory1 = nullptr;
		TComPtr<IDXGIFactory2> DXGIFactory2 = nullptr;

		HR(mDevice_->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(DXGIDevice.GetAddressOf())));
		HR(DXGIDevice->GetAdapter(DXGIAdapter.GetAddressOf()));
		HR(DXGIAdapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(DXGIFactory1.GetAddressOf())));

		HRESULT hr = DXGIAdapter->GetParent(__uuidof(DXGIFactory2), reinterpret_cast<void**>(DXGIFactory2.GetAddressOf()));
		if (SUCCEEDED(hr))
		{
			HR(mDevice_.As(&mDevice1_));
			HR(mImmediateContext_.As(&mImmediateContext1_));

			DXGI_SWAP_CHAIN_DESC1 swapChainDesc;
			ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
			swapChainDesc.Width = SCREEN_INIT_WIDTH;
			swapChainDesc.Height = SCREEN_INIT_HEIGHT;
			swapChainDesc.Format = mSwapchainFormat_;
			if (mEnable4xMsaa_)
			{
				swapChainDesc.SampleDesc.Count = 4;
				swapChainDesc.SampleDesc.Quality = m4xMsaaQaulity_ - 1;
			}
			else
			{
				swapChainDesc.SampleDesc.Count = 1;
				swapChainDesc.SampleDesc.Quality = 0;
			}
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.BufferCount = 1;
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			swapChainDesc.Flags = 0;

			DXGI_SWAP_CHAIN_FULLSCREEN_DESC swapchainFullScreenDesc;
			swapchainFullScreenDesc.RefreshRate.Numerator = SCREEN_REFRESH_RATE;
			swapchainFullScreenDesc.RefreshRate.Denominator = 1;
			swapchainFullScreenDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			swapchainFullScreenDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			swapchainFullScreenDesc.Windowed = TRUE;

			HR(DXGIFactory2->CreateSwapChainForHwnd(mDevice1_.Get(), mMainWnd_, &swapChainDesc, &swapchainFullScreenDesc, nullptr, mSwapChain1_.GetAddressOf()));
			HR(mSwapChain1_.As(&mSwapChain_));
		}
		else
		{
			DXGI_SWAP_CHAIN_DESC swapChainDesc;
			ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
			swapChainDesc.BufferDesc.Width = SCREEN_INIT_WIDTH;
			swapChainDesc.BufferDesc.Height = SCREEN_INIT_HEIGHT;
			swapChainDesc.BufferDesc.Format = mSwapchainFormat_;
			if (mEnable4xMsaa_)
			{
				swapChainDesc.SampleDesc.Count = 4;
				swapChainDesc.SampleDesc.Quality = m4xMsaaQaulity_ - 1;
			}
			else
			{
				swapChainDesc.SampleDesc.Count = 1;
				swapChainDesc.SampleDesc.Quality = 0;
			}
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.BufferCount = 1;
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			swapChainDesc.Flags = 0;

			swapChainDesc.BufferDesc.RefreshRate.Numerator = SCREEN_REFRESH_RATE;
			swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
			swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			swapChainDesc.Windowed = TRUE;

			swapChainDesc.OutputWindow = mMainWnd_;
			HR(DXGIFactory1->CreateSwapChain(mDevice_.Get(), &swapChainDesc, mSwapChain_.GetAddressOf()));
		}

		// Forbid enter alt + enter to set fullScreen
		DXGIFactory1->MakeWindowAssociation(mMainWnd_, DXGI_MWA_NO_ALT_ENTER | DXGI_MWA_NO_WINDOW_CHANGES);
		
		
		OnResize();
	}

	void D3D11Device::OnResize()
	{
		HYBRID_CHECK(mImmediateContext_);
		HYBRID_CHECK(mDevice_);
		HYBRID_CHECK(mSwapChain_);

		if (mDevice1_ != nullptr)
		{
			HYBRID_CHECK(mImmediateContext1_);
			HYBRID_CHECK(mSwapChain1_);
		}

		mRenderTargetView_.Reset();
		mDepthStencilView_.Reset();
		mDepthStencilBuffer_.Reset();

		TComPtr<ID3D11Texture2D> backBuffer;
		const UINT width = SCREEN_INIT_WIDTH;
		const UINT height = SCREEN_INIT_HEIGHT;

		HR(mSwapChain_->ResizeBuffers(1, width, height, mSwapchainFormat_, 0));
		HR(mSwapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf())));
		HR(mDevice_->CreateRenderTargetView(backBuffer.Get(), nullptr, mRenderTargetView_.GetAddressOf()));

		backBuffer.Reset();

		D3D11_TEXTURE2D_DESC depthStencilDesc;
		depthStencilDesc.Width = width;
		depthStencilDesc.Height = height;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = mDepthStencilFormat_;

		if (mEnable4xMsaa_)
		{
			depthStencilDesc.SampleDesc.Count = 4;
			depthStencilDesc.SampleDesc.Quality = m4xMsaaQaulity_ - 1;
		}
		else
		{
			depthStencilDesc.SampleDesc.Count = 1;
			depthStencilDesc.SampleDesc.Quality = 0;
		}

		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;

		mDevice_->CreateTexture2D(&depthStencilDesc, nullptr, mDepthStencilBuffer_.GetAddressOf());
		mDevice_->CreateDepthStencilView(mDepthStencilBuffer_.Get(), nullptr, mDepthStencilView_.GetAddressOf());

		mImmediateContext_->OMSetRenderTargets(1, mRenderTargetView_.GetAddressOf(), mDepthStencilView_.Get());

		mScreenViewport_.TopLeftX = 0;
		mScreenViewport_.TopLeftY = 0;
		mScreenViewport_.Width = static_cast<float>(width);
		mScreenViewport_.Height = static_cast<float>(height);
		mScreenViewport_.MinDepth = 0.0f;
		mScreenViewport_.MaxDepth = 1.0;

		mImmediateContext_->RSSetViewports(1, &mScreenViewport_);
	}

	void D3D11Device::ClearScreen()
	{
		float clearValue[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		mImmediateContext_->ClearRenderTargetView(mRenderTargetView_.Get(), clearValue);
		mImmediateContext_->ClearDepthStencilView(mDepthStencilView_.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	void D3D11Device::Present()
	{
		mSwapChain_->Present(0, 0);
	}

}

