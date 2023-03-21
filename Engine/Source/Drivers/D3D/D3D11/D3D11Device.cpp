#include "D3D11Device.h"
#include "Core/Engine/Engine.h"
#include "Graphics/Elements/RenderElement.h"
#include "Graphics/RHI/IRenderPass.h"
#include "Client/RenderScene.h"
#include "D3D11Shader.h"
#include "Core/System/RenderSystem.h"
#include "Core/System/ConfigSystem.h"


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
		mSwapchainFormat_ = Converter::Format(RenderSystem::Get()->ScreenFormat);
		mDepthStencilFormat_ = Converter::Format(RenderSystem::Get()->ScreenFormat);

		mResourceFactory_ = new D3D11ResourceFactory(this);
		mBufferCount_ = RenderSystem::Get()->GetBackBufferCount();
		mBackBuffers_ = new IRenderTarget* [mBufferCount_];
		mDepthStencilBuffers_ = new IRenderTarget * [mBufferCount_];
		for (UINT i = 0; i < mBufferCount_; ++i)
		{
			mBackBuffers_[i] = RenderSystem::Get()->GetBackBuffer(i);
			mDepthStencilBuffers_[i] = RenderSystem::Get()->GetDepthStencilBuffer(i);
		}

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

	void* D3D11Device::GetDevice()
	{
		return mDevice_.Get();
	}

	void* D3D11Device::GetContext()
	{
		return mImmediateContext_.Get();
	}

	void D3D11Device::PrepareResource()
	{
		auto pipeline = Engine::Get()->GetClientScene()->GetRenderScene()->GetPipeline();
		for (auto renderPass : pipeline->GetRenderPasses())
		{
			const size_t nRT = renderPass->GetOutputCount();
			for (size_t i = 0; i < nRT; ++i)
			{
				IRenderTarget* rt = renderPass->GetPipeline()->GetRenderTargetResource(renderPass->GetOutput(static_cast<uint8>(i)));
				rt->CreateDeviceResource(GetResourceFactory());

				float clearValue[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
				mImmediateContext_->ClearRenderTargetView(((D3D11RenderTarget*)(rt->DeviceResource))->ppRTV.Get(), clearValue);
			}

			IRenderTarget* rt = renderPass->GetPipeline()->GetRenderTargetResource(renderPass->GetDepthStencil());
			if (rt)
			{
				rt->CreateDeviceResource(GetResourceFactory());
				mImmediateContext_->ClearDepthStencilView(((D3D11DepthStencil*)(rt->DeviceResource))->ppDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
			}
			
		}
	}
	
	void D3D11Device::_UpdateConstantBuffer(ShadingParameterCollection* collection)
	{
		// update batch uniform
		if (collection)
		{
			IConstantBuffer* srcBuffer = collection->GetRenderResource();
			D3D11Buffer* buffer = (D3D11Buffer*)srcBuffer->DeviceResource;
			D3D11_MAPPED_SUBRESOURCE mappedData;
			mImmediateContext_->Map(buffer->ppBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
			memcpy_s(mappedData.pData, srcBuffer->ByteWidth, srcBuffer->Data, srcBuffer->ByteWidth);
			mImmediateContext_->Unmap(buffer->ppBuffer.Get(), 0);
		}
	}

	void D3D11Device::DrawFrame()
	{
		ClearScreen();
		auto pipeline = Engine::Get()->GetClientScene()->GetRenderScene()->GetPipeline();

		// Update Global Uniform
		pipeline->GetContext()->GetParameters()->CreateDeviceResource(GetResourceFactory());
		pipeline->GetContext()->GetLightParameters()->CreateDeviceResource(GetResourceFactory());

		_UpdateConstantBuffer(pipeline->GetContext()->GetParameters());
		_UpdateConstantBuffer(pipeline->GetContext()->GetLightParameters());
		for (auto renderPass : pipeline->GetRenderPasses())
		{
			EncodeRenderPass(renderPass);
		}
		Present();
	}

	void D3D11Device::EncodeRenderPass(RenderPass* renderPass)
	{
		auto& rts = renderPass->GetRenderTargetResources();
		auto ds = renderPass->GetDepthStencilResource();


		size_t nRTV = rts.size();
		List<ID3D11RenderTargetView*> RTVs(nRTV);
		ID3D11DepthStencilView* DSV = nullptr;
		for (size_t i = 0; i < nRTV; ++i)
		{
			IRenderTarget* rt = rts[i];
			D3D11RenderTarget* d3dRT = (D3D11RenderTarget*)rt->DeviceResource;
			RTVs[i] = d3dRT->ppRTV.Get();
		}


		if (ds)
		{
			HYBRID_CHECK(ds->IsDepthStencil());
			DSV = ((D3D11DepthStencil*)ds->DeviceResource)->ppDSV.Get();
		}

		UINT UAVInitialCount = 0;
		mImmediateContext_->OMSetRenderTargetsAndUnorderedAccessViews((UINT)nRTV, RTVs.data(), DSV, 0, 0, nullptr, &UAVInitialCount);

		DrawCall* drawCall = renderPass->GetDrawCallHead();
		while (drawCall)
		{
			EncodeDrawCall(drawCall);
			drawCall = drawCall->Next_;
		}
		renderPass->EncodeRenderPassEnd();
	}

	void D3D11Device::EncodeDrawCall(DrawCall* drawCall)
	{
		drawCall->CreateDeviceResource(GetResourceFactory());
		
		auto batch = drawCall->ShadingState_->GetBatch();

		auto shaderCollection = batch->GetConstantBuffer(EShaderConstant::Shader);
		if (shaderCollection)
			_UpdateConstantBuffer(shaderCollection);

		auto batchCollection = batch->GetConstantBuffer(EShaderConstant::Batch);
		if (batchCollection)
			_UpdateConstantBuffer(batchCollection);
		
		// VS
		{
			auto vsInstance = drawCall->ShaderTechnique_->GetStageInstance(EShaderStage::VS);

			// Constant	
			for (EShaderConstant esc : vsInstance->GetReletedBatch())
			{
				auto parameter = batch->GetConstantBuffer(esc);
				if (parameter)
				{
					IConstantBuffer* srcBuffer = parameter->GetRenderResource();
					D3D11Buffer* buffer = (D3D11Buffer*)srcBuffer->DeviceResource;
					UINT numBuffer = 1;
					uint8 slot = 0;
					bool isOK = vsInstance->GetBatchSlot(esc, slot);
					HYBRID_CHECK(isOK);
					mImmediateContext_->VSSetConstantBuffers(slot, numBuffer, buffer->ppBuffer.GetAddressOf());
				}
			}

			D3D11VertexShader* vertexShader = (D3D11VertexShader*)vsInstance->_ShaderRenderResource->DeviceResource;
			mImmediateContext_->VSSetShader(vertexShader->ppShader.Get(), nullptr, 0);

		}

		// RS
		{
			mImmediateContext_->RSSetState(((D3D11PipelineState*)drawCall->Pipeline_.DeviceResource)->ppRasterizerState.Get());
		}

		// OM
		{
			// mImmediateContext_->OMSetBlendState();
		}

		// PS
		{

			auto psInstance = drawCall->ShaderTechnique_->GetStageInstance(EShaderStage::PS);
			auto psShader = psInstance->_ShaderRenderResource;

			// Constant	
			for (EShaderConstant esc : psInstance->GetReletedBatch())
			{
				auto parameter = batch->GetConstantBuffer(esc);
				if (parameter)
				{
					IConstantBuffer* srcBuffer = parameter->GetRenderResource();
					D3D11Buffer* buffer = (D3D11Buffer*)srcBuffer->DeviceResource;
					UINT numBuffer = 1;
					uint8 slot = 0;
					bool isOK = psInstance->GetBatchSlot(esc, slot);
					HYBRID_CHECK(isOK);
					mImmediateContext_->PSSetConstantBuffers(slot, numBuffer, buffer->ppBuffer.GetAddressOf());
				}
			}

			// Texture
			List<ID3D11ShaderResourceView*> texViews;
			texViews.reserve(psInstance->GetMaxImageBinding());
			auto resourceBinding = batch->GetResourceBinding();
			for (uint8 i = 0; i < psInstance->GetMaxImageBinding(); ++i)
			{
				if (!psInstance->IsImageBinding(i))
				{
					texViews.push_back(nullptr);
					continue;
				}

				ITextureBuffer* tex = (ITextureBuffer*)resourceBinding->GetTexture(i);
				HYBRID_CHECK(tex->BindType & EBufferTypes(EBufferType::ShaderResource));
				if (tex)
				{
					if (tex->BindType & EBufferTypes(EBufferType::RenderTarget))
						texViews.push_back(((D3D11RenderTarget*)(tex->DeviceResource))->ppSRV.Get());
					else
						texViews.push_back(((D3D11Texture*)(tex->DeviceResource))->ppSRV.Get());
				}
				else
				{
					texViews.push_back(nullptr);
				}
			}
			mImmediateContext_->PSSetShaderResources(0, (UINT)texViews.size(), texViews.data());

			// Sampler
			List<ID3D11SamplerState*> samplerStates;
			HYBRID_CHECK(resourceBinding->Textures == psShader->Samplers.size());
			samplerStates.reserve(psShader->Samplers.size());
			for (size_t i = 0; i < psShader->Samplers.size(); ++i)
			{
				if (!psInstance->IsSamplerBinding(uint8(i)))
				{
					samplerStates.push_back(nullptr);
					continue;
				}

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
			swapChainDesc.Width = GetBackBuffer()->Width;
			swapChainDesc.Height = GetBackBuffer()->Height;
			swapChainDesc.Format = mSwapchainFormat_;
			swapChainDesc.SampleDesc.Count = Converter::SamplerCount(GetBackBuffer()->Quality);
			UINT quality = UINT(Converter::SamplerQualityFactor(GetBackBuffer()->Quality) * m4xMsaaQaulity_);
			swapChainDesc.SampleDesc.Quality = quality > 0 ? quality - 1 : 0;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.BufferCount = mBufferCount_;
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			swapChainDesc.Flags = 0;

			DXGI_SWAP_CHAIN_FULLSCREEN_DESC swapchainFullScreenDesc;
			swapchainFullScreenDesc.RefreshRate.Numerator = ConfigSystem::Get()->GetGlobalConfig("ScreenRefreshRate", 60);
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
			swapChainDesc.BufferDesc.Width = RenderSystem::Get()->ScreenWidth;
			swapChainDesc.BufferDesc.Height = RenderSystem::Get()->ScreenHeight;
			swapChainDesc.BufferDesc.Format = mSwapchainFormat_;
			swapChainDesc.SampleDesc.Count = Converter::SamplerCount(GetBackBuffer()->Quality);
			UINT quality = UINT(Converter::SamplerQualityFactor(GetBackBuffer()->Quality) * m4xMsaaQaulity_);
			swapChainDesc.SampleDesc.Quality = quality > 0 ? quality - 1 : 0;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.BufferCount = mBufferCount_;
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			swapChainDesc.Flags = 0;

			swapChainDesc.BufferDesc.RefreshRate.Numerator = ConfigSystem::Get()->GetGlobalConfig("ScreenRefreshRate", 60);
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

		const UINT width = GetBackBuffer()->Width;
		const UINT height = GetBackBuffer()->Height;

		HR(mSwapChain_->ResizeBuffers(1, width, height, mSwapchainFormat_, 0));

		for (UINT i = 0; i < mBufferCount_; ++i)
		{
			SafeDestroy(mBackBuffers_[i]->DeviceResource);
			TComPtr<ID3D11Texture2D> backBuffer;
			HR(mSwapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf())));
			GetResourceFactory()->CreateBackBuffer(mBackBuffers_[i], backBuffer.Get());
			backBuffer.Reset();

			SafeDestroy(mDepthStencilBuffers_[i]->DeviceResource);
			GetResourceFactory()->CreateDepthStencil(mDepthStencilBuffers_[i]);
		}

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
		mImmediateContext_->ClearRenderTargetView(((D3D11RenderTarget*)GetBackBuffer()->DeviceResource)->ppRTV.Get(), clearValue);
		mImmediateContext_->ClearDepthStencilView(mDepthStencilView_.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	void D3D11Device::Present()
	{
		mSwapChain_->Present(0, 0);
		mCurrentFrameIndex_ = (mCurrentFrameIndex_ + 1) % mBufferCount_;
	}

}

