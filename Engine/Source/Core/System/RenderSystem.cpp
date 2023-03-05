#include "RenderSystem.h"


namespace Eggy
{
	DefineSystem(RenderSystem);

	void RenderSystem::Initialize()
	{
		ShadingType = EShadingType::Deferred;

		mBackBufferCount_ = 2;

		mBackBuffers_ = new IRenderTarget[mBackBufferCount_];
		mDepthStencilsBuffer_ = new IRenderTarget[mBackBufferCount_];

		ScreenFormat = EFormat::R8G8B8A8;
		ScreenDSFormat = EFormat::D32_SFLOAT_S8X24_UINT;
		ScreenWidth = 800;
		ScreenHeight = 600;

		for (uint32 i = 0; i< mBackBufferCount_; ++i)
		{
			mBackBuffers_[i].Width = ScreenWidth;
			mBackBuffers_[i].Height = ScreenHeight;
			mBackBuffers_[i].Format = ScreenFormat;
			mBackBuffers_[i].IsBackBuffer = true;

			mDepthStencilsBuffer_[i].Width = ScreenWidth;
			mDepthStencilsBuffer_[i].Height = ScreenHeight;
			mDepthStencilsBuffer_[i].Format = ScreenDSFormat;
			mDepthStencilsBuffer_[i].BindType = EBufferTypes(EBufferType::DepthStencil);
			mDepthStencilsBuffer_[i].IsBackBuffer = true;
		}
	}

	void RenderSystem::Finalize()
	{
		SafeDestroyArray(mBackBuffers_);
	}

	IRenderTarget* RenderSystem::GetBackBuffer(uint32 i)
	{
		return &mBackBuffers_[i % mBackBufferCount_];
	}

	IRenderTarget* RenderSystem::GetDepthStencilBuffer(uint32 i)
	{
		return &mDepthStencilsBuffer_[i % mBackBufferCount_];
	}

}