#include "Common.hlsli"

struct VertexIn
{
	float3 pos : POSITION;
	float4 color : COLOR;
	float2 st: TEXCOORD;
};

struct VertexOut
{
	float4 posH : SV_POSITION;
	float2 st : TEXCOORD;
};

Texture2D BaseColorMap : register(t0);
SamplerState BaseColorSampler : register(s0);

Texture2D EmissiveMap : register(t1);
SamplerState EmissiveMapSampler : register(s1);

Texture2D NormalMap : register(t2);
SamplerState NormalMapSampler : register(s2);

Texture2D MiscMap : register(t3);
SamplerState MiscMapSampler : register(s3);

Texture2D<float> DepthMap : register(t4);
SamplerState DepthMapSampler : register(s4);


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
	float4 baseColor = BaseColorMap.Sample(BaseColorSampler, pIn.st);
	float shadingModel = baseColor.w;
	float4 emissive = EmissiveMap.Sample(EmissiveMapSampler, pIn.st);
	float4 normal = NormalMap.Sample(NormalMapSampler, pIn.st);
	float4 misc = MiscMap.Sample(MiscMapSampler, pIn.st);
	float metallic = misc.x;
	float roughness = misc.y;
	float anisotropic = misc.z;
	float clearCoat = misc.w;
	float depth = DepthMap.Sample(DepthMapSampler, pIn.st);
	return float4(baseColor.xyz * emissive.xyz, 1) * roughness;
}
