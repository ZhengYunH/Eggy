#include "Common.hlsli"

struct GBufferData
{
    // GBufferA RGBA8f
	float3 WorldNormal;
    /* float __PlaceHolder__; */

    // GBufferB RGBA8f
    float Metallic;
    float Specular;
	float Roughness;
	int ShadingModelID;

    // GBufferC RGBA8f
	float3 BaseColor;
    float AO;

    // GBufferD RGBA8f
    float4 CustomData;

    // GBufferE R32f
    float LinearDepth; 


    float3 SpecularColor;
    float3 ViewDir;
};

GBufferData GetInitialGBufferData(int ShadingModelID)
{
    GBufferData GBuffer;
    GBuffer.WorldNormal = float3(0, 1, 0);

    GBuffer.Metallic = 0.0;
    GBuffer.Specular = 0.0;
    GBuffer.Roughness = 0.0;
    GBuffer.ShadingModelID = ShadingModelID;

    GBuffer.BaseColor = float3(0, 0, 0);
    GBuffer.AO = 1;

    GBuffer.CustomData = float4(0, 0, 0, 0);
    GBuffer.LinearDepth = 1;

    GBuffer.SpecularColor = float3(0, 0, 0);

    return GBuffer;
}

float EncodeShadingModelID(float ShadingModelID)
{
	return (float)ShadingModelID / (float)0xFF;
}

int DecodeShadingModelID(float val)
{
	return (int)round(val * (float)0xFF);
}

void EncodeGBuffer(GBufferData GBuffer, out float4 GBufferA, out float4 GBufferB, out float4 GBufferC, out float4 GBufferD, out float GBufferE)
{
    GBufferA = float4((float3(1.0, 1.0, 1.0) + GBuffer.WorldNormal) * 0.5, 0.0);
    GBufferB = float4(GBuffer.Metallic, GBuffer.Specular, GBuffer.Roughness, EncodeShadingModelID(GBuffer.ShadingModelID));
    GBufferC = float4(GBuffer.BaseColor, GBuffer.AO);
    GBufferD = GBuffer.CustomData;
    GBufferE = GBuffer.LinearDepth;
}

GBufferData DecodeGBuffer(float4 GBufferA, float4 GBufferB, float4 GBufferC, float4 GBufferD, float GBufferE)
{
	GBufferData GBuffer;
    GBuffer.WorldNormal = GBufferA.xyz * 2 - 1;
	
    GBuffer.Metallic = GBufferB.x;
    GBuffer.Specular = GBufferB.y;
    GBuffer.Roughness = GBufferB.z;
    GBuffer.ShadingModelID = DecodeShadingModelID(GBufferB.w);

    GBuffer.BaseColor = GBufferC.rgb;
    GBuffer.AO = GBufferC.a;

    GBuffer.CustomData = GBufferD;

    GBuffer.LinearDepth = GBufferE;

    GBuffer.SpecularColor = float3(0, 0, 0);

    GBuffer.ViewDir = float3(0, 0, 0);

    if(GBuffer.ShadingModelID == SHADINGMODELID_UNLIT)
    {
        GBuffer.SpecularColor = float3(0, 0, 0);
    }
        
    if(GBuffer.ShadingModelID == SHADINGMODELID_DEFAULT_LIT)
    {
        GBuffer.SpecularColor = lerp(float3(0.4, 0.4, 0.4), GBuffer.BaseColor, GBuffer.Metallic);
    }
        
    if(GBuffer.ShadingModelID == SHADINGMODELID_CLEAR_COAT)
    {
        GBuffer.SpecularColor = lerp(float3(0.4, 0.4, 0.4), GBuffer.BaseColor, GBuffer.Metallic);
    }
    
    return GBuffer;
}

struct LightingResult
{
    float3 DiffuseLighting;
    float3 SpecularLighting;
};

struct LightingData
{
    /* Color * Atten * NoL always means irradiance, but Color and Atten defined in different ways  for different type of light
        1. point / spot lights
            Color = intensity(point light: Φ / 4PI , spot light: Φ / [4PI*(1-cos(phi))])
            Atten = 1 / (0.01 + SquareDistance)
        2. sun light
            Color = projected irradiance
            Attend = 1
    */ 
    float3 Color;
    float Atten;

    // normalized direction to light source
    float3 L;
};

LightingData GetLightDataByLight(LightData ld, float3 WorldPosition)
{
    LightingData data;
    data.Color = float3(0, 0, 0);
    data.Atten = 0.0;
    data.L = float3(0, 0, 0);

    if(ld.LightType == EDirectionLight)
    {
        data.Color = ld.Color * ld.Intensity;
        data.Atten = 1;
        data.L = -ld.Direction;
    }
    else if(ld.LightType == EPointLight)
    {
        data.Color = ld.Color * ld.Intensity / (4 * PI);
        float3 P2L = ld.Position - WorldPosition;
        float constantFactor = ld.Misc0;
        float linearFactor = ld.Misc1;
        float quadraticFactor = ld.Misc2;
        float distance = max(1.0, length(P2L));
        float attenuation = 1.0 / max(constantFactor + linearFactor * distance + quadraticFactor * distance * distance, 0.01);
        data.Atten = attenuation;
        data.L = normalize(P2L);
    }
    else if(ld.LightType == ESpotLight)
    {
        float3 P2L = ld.Position - WorldPosition;
        float phi = dot(normalize(P2L), -ld.Direction);
        data.Color = ld.Color * ld.Intensity * phi / (4 * PI);
        float distance = max(1.0, length(P2L));
        data.Atten = 1.0 / (distance * distance);
        data.L = normalize(P2L);
    }
    return data;
}

