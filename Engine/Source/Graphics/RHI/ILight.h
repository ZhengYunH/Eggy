#pragma once
#include "IRenderHeader.h"
#include "Core/Math/Vector3.h"


namespace Eggy
{
	// keep order as declared in light shader
	enum class ELight : uint32
	{
		DirectionLight = 0,
		PointLight = 1,
		SpotLight = 2,
	};

	struct LightData
	{
		uint32 LightType;
		Vector3 Position;
		
		float Intensity;
		Vector3 Direction;
		
		float Misc0;
		Vector3 Color;

		float Misc1;
		float Misc2;
		float Misc3;
		float Misc4;
	};

	class ILight
	{
	public:
		ILight(ELight lightType)
		{
			mData_.LightType = (uint32)lightType;
		}

		const LightData& GetData()
		{
			return mData_;
		}

	protected:
		LightData mData_;
	};
}

