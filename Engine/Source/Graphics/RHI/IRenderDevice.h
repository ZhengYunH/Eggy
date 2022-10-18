#include "Core/Config.h"


namespace Eggy
{
	class IRenderDevice
	{
	public:
		IRenderDevice() = default;

	public:
		virtual void DrawFrame() = 0;

	protected:
		virtual void CreateSwapChain() {};
		virtual void OnResize() {};

	};
	extern void CreateRenderDevice();
	extern IRenderDevice* GRenderDevice;
}
