#pragma once
#include "Core/Config.h"
#include "Graphics/RHI/IRenderDevice.h"
#include "Graphics/RHI/IRenderHeader.h"
#include "Graphics/RHI/IRenderResource.h"


namespace Eggy
{
	struct IRenderObject : public IRenderResource
	{
		virtual void Consolidate() {};
	};
}