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
float3 F_Schlick(float3 F0, float VoH)
{
    float cosTheta = max(VoH, 0);
	return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
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

float3 PbrBRDF(in float3 L, in float3 N, in float3 V, in float3 DiffuseColor, in float3 SpecularColor, in float Roughness, in float Metallic)
{
    float3 H = normalize(V + L);
    float NoH = saturate(dot(N, H));
    float VoH = saturate(dot(V, H));
    float VoL = saturate(dot(V, L));

    float D = D_GGX(Roughness, NoH);
    float IoR = Metallic < 0.01 ? 1.5 : 20;
    float3 F0 = CalculateF0(Roughness, IoR, Metallic);
    float3 F = F_Schlick(F0, VoH);
    float G = G_Schlick(Roughness, VoH, VoL);

    return DiffuseColor * (1 - F) + SpecularColor * (D*F*G);
}



#endif /* __FILE_BRDF_HEADER__ */
