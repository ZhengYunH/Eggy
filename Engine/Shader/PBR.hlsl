#include "Common.hlsli"

cbuffer Shader : register(b1)
{
    float Roughness;
}

cbuffer Batch : register(b2)
{
	row_major matrix cModel;
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

struct PixelOutput
{
	float4 GBufferA : SV_Target0;
	float4 GBufferB : SV_Target1;
	float4 GBufferC : SV_Target2;
	float4 GBufferD : SV_Target3;
	float GBufferE : SV_Target4;
};

VertexOut VS(VertexIn vIn)
{
    VertexOut vOut;
    vOut.posH = mul(float4(vIn.pos, 1.0f), cModel);
    vOut.posH = mul(vOut.posH, cView);
    vOut.posH = mul(vOut.posH, cProj);
    vOut.color = DebugColor;
    vOut.normal = vIn.normal;
    vOut.st = 1 - vIn.st;
    return vOut;
}

PixelOutput PS(VertexOut pIn)
{
    PixelOutput output;
    float4 baseColor = BaseMap.Sample(BaseMapSampler, pIn.st);
    float4 emissive = pIn.color;
    float3 normal = pIn.normal;
    float sheen = 0;
    float metallic = 0; 
    float roughness = Roughness;
    float anisotropic = 0;
    float clearCoat = 0;
    float depth = pIn.posH.z;

    output.GBufferA = baseColor;
    output.GBufferB = emissive;
    output.GBufferC = float4(normal, sheen);
    output.GBufferD = float4(metallic, roughness, anisotropic, clearCoat);
    output.GBufferE = depth;

    return output;
}
