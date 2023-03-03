#include "Common.hlsli"


cbuffer Batch : register(b2)
{
	row_major matrix cWorld;
}

Texture2D BaseMap : register(t0);
SamplerState BaseMapSampler : register(s0);

struct VertexIn
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 st : TEXCOORD;
};

struct VertexOut
{
    float4 posH : SV_POSITION;
    float3 normal : NORMAL;
    float4 color : COLOR;
    float2 st : TEXCOORD;
};

VertexOut VS(VertexIn vIn)
{
    VertexOut vOut;
    vOut.posH = mul(float4(vIn.pos, 1.0f), cWorld);
    vOut.posH = mul(vOut.posH, cView);
    vOut.posH = mul(vOut.posH, cProj);
    vOut.color = DebugColor;
    vOut.normal = vIn.normal;
    vOut.st = 1 - vIn.st;
    return vOut;
}


float4 PS(VertexOut pIn) : SV_Target
{
    float4 texColor = BaseMap.Sample(BaseMapSampler, pIn.st);
    return pIn.color * texColor;   
}
