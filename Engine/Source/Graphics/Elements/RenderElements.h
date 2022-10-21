#pragma once
#include "Core/Config.h"
#include "Graphics/RHI/IRenderResource.h"
#include "Core/Interface/IRenderElements.h"

namespace Eggy
{
	class RenderElements : public IRenderElements, public IGeometry
	{

	};
}