#include "Common.hlsli"


cbuffer Shader : register(b1)
{
    float Roughness;
}

cbuffer Batch : register(b2)
{
	row_major matrix cModel;
}

struct VertexIn
{
    float3 pos : POSITION;
    float4 color : COLOR;
};

struct VertexOut
{
    float4 posH : SV_POSITION;
    float4 color : COLOR;
};

VertexOut VS(VertexIn vIn)
{
    VertexOut vOut;
    vOut.posH = mul(float4(vIn.pos, 1.0f), cModel);
    vOut.posH = mul(vOut.posH, cView);
    vOut.posH = mul(vOut.posH, cProj);
    vOut.color = vIn.color;
    return vOut;
}

float4 PS(VertexOut pIn) : SV_Target
{
    return pIn.color;
}
