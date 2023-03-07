#include "Common.hlsli"


cbuffer Shader : register(b1)
{
    float Roughness;
}

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

float3 ShadingByDirectLight(VertexOut pIn, LightData light)
{
    float3 ambient = light.Ambient;
    
    float diff = max(dot(pIn.normal, light.Direction), 0.0);
    float3 diffuse = diff * light.Diffuse;

    float3 specular = float3(0, 0, 0);
    return ambient + diffuse + specular;
}


float4 PS(VertexOut pIn) : SV_Target
{
    float4 texColor = BaseMap.Sample(BaseMapSampler, pIn.st);
    float4 finalColor = pIn.color * texColor * Roughness;
    if(LightCount > 0)
    {
        int type = round(Lights[0].LightType * 255);
        if(type == EDirectionLight)
            finalColor += float4(ShadingByDirectLight(pIn, Lights[0]), 1);
    }
    return finalColor;
}
