#ifndef __FILE_SHADING_MODEL_HEADER__
#define __FILE_SHADING_MODEL_HEADER__

#include "DeferredShadingCommon.hlsli"
#include "BRDF.hlsli"


void DefaultLitBxDF(in out LightingResult LitResult, in GBufferData GBuffer, in LightingData LitData)
{
    float NoL = saturate(dot(GBuffer.WorldNormal, LitData.L));

    float3 irradiance = LitData.Color * LitData.Atten * NoL;
    LitResult.DiffuseLighting += irradiance;

    LitResult.SpecularLighting += PbrBRDF(
        LitData.L, GBuffer.WorldNormal, GBuffer.ViewDir, 
        GBuffer.BaseColor, GBuffer.SpecularColor, GBuffer.Roughness, GBuffer.Metallic
    );
}

void IntegrateBxDF(in out LightingResult LitResult, in GBufferData GBuffer, in LightingData LitData)
{
    if(GBuffer.ShadingModelID == SHADINGMODELID_DEFAULT_LIT)
    {
        DefaultLitBxDF(LitResult, GBuffer, LitData);
    }
}

#endif /* __FILE_SHADING_MODEL_HEADER__ */
