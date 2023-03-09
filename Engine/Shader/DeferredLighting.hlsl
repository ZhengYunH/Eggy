#include "Common.hlsli"
#include "DeferredShadingCommon.hlsli"


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

Texture2D tGBufferA : register(t0);
SamplerState tGBufferASampler : register(s0);

Texture2D tGBufferB : register(t1);
SamplerState tGBufferBSampler : register(s1);

Texture2D tGBufferC : register(t2);
SamplerState tGBufferCSampler : register(s2);

Texture2D tGBufferD : register(t3);
SamplerState tGBufferDSampler : register(s3);

Texture2D<float> tGBufferE : register(t4);
SamplerState tGBufferESampler : register(s4);


GBufferData SampleGBuffer(float2 tc)
{
	GBufferData GBuffer = DecodeGBuffer(
        tGBufferA.Sample(tGBufferASampler, tc),
        tGBufferB.Sample(tGBufferBSampler, tc),
        tGBufferC.Sample(tGBufferCSampler, tc),
        tGBufferD.Sample(tGBufferDSampler, tc),
        tGBufferE.Sample(tGBufferESampler, tc)
    );
    return GBuffer;
}

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
	GBufferData GBuffer = SampleGBuffer(pIn.st);
	float3 baseColor = GBuffer.BaseColor;
	int shadingModel = GBuffer.ShadingModelID;
	float roughness = GBuffer.Roughness;
	float3 worldNormal = GBuffer.WorldNormal;
	float depth = GBuffer.LinearDepth;
	return float4(baseColor.xyz, 1) * roughness;
}
