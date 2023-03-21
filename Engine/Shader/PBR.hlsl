#include "Common.hlsli"
#include "DeferredShadingCommon.hlsli"


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
    float3 WorldPosition : TEXCOORD1;
    float3 WorldNormal: TEXCOORD2;
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
    vOut.WorldPosition = vOut.posH.xyz;
    vOut.WorldNormal = normalize(mul(float4(vIn.normal, 0.0f), cModel).xyz);

    vOut.posH = mul(vOut.posH, cView);
    vOut.posH = mul(vOut.posH, cProj);
    vOut.color = cDebugColor;
    vOut.normal = vIn.normal;
    vOut.st = 1 - vIn.st;
    return vOut;
}

PixelOutput PS(VertexOut pIn)
{
    PixelOutput output;
    int ShadingModelID = 1;

    GBufferData GBuffer = GetInitialGBufferData(ShadingModelID);
    GBuffer.BaseColor = BaseMap.Sample(BaseMapSampler, pIn.st).rgb;
    GBuffer.ShadingModelID = ShadingModelID;
    GBuffer.Roughness = Roughness;
    GBuffer.LinearDepth = length(pIn.WorldPosition - cViewPos) * cCameraInfo.w;
    GBuffer.WorldNormal = pIn.WorldNormal;

    EncodeGBuffer(GBuffer, output.GBufferA, output.GBufferB, output.GBufferC, output.GBufferD, output.GBufferE);
    return output;
}
