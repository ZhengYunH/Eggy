#pragma once
#include "Core/Config.h"
#include "Core/Interface/ISystem.h"
#include "Graphics/RHI/IRenderResource.h"


namespace Eggy
{
	class RenderSystem : public ISystem<RenderSystem>
	{
		DeclareSystem(RenderSystem);
	public:
		void Initialize() override;
		void Finalize() override;

		IRenderTarget* GetBackBuffer(uint32 i);
		IRenderTarget* GetDepthStencilBuffer(uint32 i);
		uint32 GetBackBufferCount() { return mBackBufferCount_; }

	public:
		uint32 ScreenWidth{ 0 };
		uint32 ScreenHeight{ 0 };
		EFormat ScreenFormat{ EFormat::R8G8B8A8 };
		EFormat ScreenDSFormat{ EFormat::D32_SFLOAT_S8X24_UINT };
		bool Enable4XMSAA{ false };

	protected:
		IRenderTarget* mBackBuffers_{ nullptr };
		IRenderTarget* mDepthStencilsBuffer_{ nullptr };
		uint32 mBackBufferCount_{ 1 };
	};

}


