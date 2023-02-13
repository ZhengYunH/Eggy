#include "PostProcessBasic.hlsli"

Texture2D Base : register(t0);
SamplerState BaseSampler : register(s0)
{
    Filter = Linear;
    AddressU = Wrap;
    AddressV = Wrap;
};

float4 PS(VertexOut pIn) : SV_Target
{
	float4 baseColor = Base.Sample(BaseSampler, pIn.st);
	return baseColor;
}