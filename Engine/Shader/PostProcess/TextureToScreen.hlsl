#include "PostProcessBasic.hlsli"

cbuffer Shader : register(b1)
{
    float Brightness;
}

Texture2D Base : register(t0);
SamplerState BaseSampler : register(s0);


VertexOut VS(VertexIn vIn)
{
    VertexOut vOut;
	float3 vPos = vIn.pos;
	vPos.y = 1 - vPos.y;
    vOut.posH = float4(vPos.xy * 2 - 1, vPos.z, 1.0);
    vOut.st = vIn.st;
    return vOut;
}


float4 PS(VertexOut pIn) : SV_Target
{
	float4 baseColor = Base.Sample(BaseSampler, pIn.st) * Brightness;
	return baseColor;
}
