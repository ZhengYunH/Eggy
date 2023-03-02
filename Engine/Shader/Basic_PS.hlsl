#include "Basic.hlsli"

Texture2D BaseMap : register(t0);
SamplerState BaseMapSampler : register(s0);
float4 PS(VertexOut pIn) : SV_Target
{
    float4 texColor = BaseMap.Sample(BaseMapSampler, pIn.st);
    return pIn.color * texColor;   
}
