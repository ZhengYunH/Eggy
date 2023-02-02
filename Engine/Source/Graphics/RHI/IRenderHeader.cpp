#include "IRenderHeader.h"


namespace Eggy
{
	const EFormatInfo& GetFormatInfo(EFormat format)
	{
		HYBRID_CHECK(uint32(format) < uint32(EFormat::END));
		return GFormatInfo[uint32(format)];
	}
}
