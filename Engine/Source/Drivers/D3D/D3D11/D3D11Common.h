#include <d3d11_1.h>
#include <d3dcommon.h>
#include <dxgiformat.h>
#include <wrl/client.h>

namespace Eggy
{
	template <class T>
	using TComPtr = Microsoft::WRL::ComPtr<T>;
}