#pragma once
#include "Core/Config.h"


namespace Eggy
{
	struct IRenderObject
	{
		virtual void PrepareBatchData(class IShadingBatch* batch) {};
	};
}