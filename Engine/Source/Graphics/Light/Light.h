#pragma once
#include "Graphics/RHI/ILight.h"

namespace Eggy
{
	class DirectionLight : public ILight
	{
	public:
		DirectionLight(Vector3 Direction, Vector3 Color, float Intensity) : ILight(ELight::DirectionLight)
		{
			mData_.Direction = Direction.GetNormalized();
			mData_.Color = Color;
			mData_.Intensity = Intensity;
		}
	};

	class PointLight : public ILight
	{
	public:
		PointLight(Vector3 Position, Vector3 Color, float Intensity) : ILight(ELight::PointLight)
		{
			mData_.Position = Position;
			mData_.Color = Color;
			mData_.Intensity = Intensity;

			SetConstant(1.0f);
			SetLinear(0.5f);
			SetQuadratic(0.3f);
		}

		void SetConstant(float v)
		{
			mData_.Misc0 = v;
		}

		void SetLinear(float v)
		{
			mData_.Misc1 = v;
		}

		void SetQuadratic(float v)
		{
			mData_.Misc2 = v;
		}
	};

	class SpotLight : public ILight
	{
	public:
		SpotLight(Vector3 Position, Vector3 Direction, Vector3 Color, float Intensity) : ILight(ELight::SpotLight)
		{
			mData_.Position = Position;
			mData_.Direction = Direction.GetNormalized();
			mData_.Color = Color;
			mData_.Intensity = Intensity;

			SetCutOff(15.f);
			SetOutCutoff(20.f);
		}

		void SetCutOff(float degree)
		{
			mData_.Misc0 = Cos(DegreeToRadian(degree));
		}

		void SetOutCutoff(float degree)
		{
			mData_.Misc1 = Cos(DegreeToRadian(degree));
		}
	};

}


