#include "Core/Config.h"


namespace Eggy
{
	class IRenderDevice
	{
	public:
		IRenderDevice() = default;
	};
	extern IRenderDevice* GRenderDevice;
}
