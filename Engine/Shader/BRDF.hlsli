#ifndef __FILE_BRDF_HEADER__
#define __FILE_BRDF_HEADER__

#include "Common.hlsli"

float Chi(float v)
{
    return v > 0.0? 1.0 : 0.0;
}

// D-term
float D_GGX(float Roughness, float NoH)
{
    float r2 = Roughness * Roughness;
    float NoH2 = NoH * NoH;
    float den = NoH2 * r2 + (1 - NoH2);
    return (Chi(NoH) * r2) / (PI * den * den);
}

// F-term
float3 F_Schlick(float3 SpecularColor, float VoH)
{
	return SpecularColor + ( saturate( half(50.0) * SpecularColor.g ) - SpecularColor ) * exp2( (half(-5.55473) * VoH - half(6.98316)) * VoH );
}

float F_Schlick(float F0, float F90, float u)
{
    half x = half(1.0) - u;
    half x2 = x * x;
    half x5 = x * x2 * x2;
    return (F90 - F0) * x5 + F0;
}


// G-term
float G_Implicit()
{
    return 0.25;
}

float G_GGX(float Roughness, float VoH, float VoN)
{
    float VoH2 = VoH * VoH;
    return (Chi(VoH / VoN) * 2) / (1 + sqrt(1 + Roughness * Roughness * ((1 - VoH2) / VoH2)));
}


float G_Schlick(float Roughness, float VoH, float VoL)
{
    float k = (Roughness + 1);
    k = k * k / 8;
    float G_H = VoH / ((VoH) * (1 - k) + k);
    float G_L = VoL / ((VoL) * (1 - k) + k);
    return G_H * G_L;
}


float3 CalculateF0(float3 BaseColor, float IoR, float Metallic)
{
    float3 F0 = abs((1.0 - IoR) / (1.0 + IoR));
    F0 = F0 * F0;
    F0 = lerp(F0, BaseColor, Metallic);
    return F0;
}

float3 PbrBRDF(in float3 L, in float3 N, in float3 V, in float3 DiffuseColor, in float3 SpecularColor, in float Roughness)
{
    float3 H = normalize(V + L);
    float NoH = saturate(dot(N, H));
    float VoH = saturate(dot(V, H));
    float VoL = saturate(dot(V, L));

    float D = D_GGX(Roughness, NoH);
    float3 F = F_Schlick(SpecularColor, VoH);
    float G = G_Schlick(Roughness, VoH, VoL);

    return DiffuseColor * (1 - F) + SpecularColor * (D*F*G);
}



#endif /* __FILE_BRDF_HEADER__ */
