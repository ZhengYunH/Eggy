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
    float2 st : TEXCOORD0;
    float3 worldPosition : TEXCOORD1;
    float3 worldNormal: TEXCOORD2;
};

VertexOut VS(VertexIn vIn)
{
    VertexOut vOut;
    vOut.posH = mul(float4(vIn.pos, 1.0f), cModel);
    vOut.worldPosition = vOut.posH.xyz;
    vOut.worldNormal = normalize(mul(float4(vIn.normal, 0.0f), cModel).xyz);

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
    
    float diff = max(dot(-light.Direction, pIn.worldNormal), 0.0);
    float3 diffuse = diff * light.Diffuse;

    float3 viewDir = normalize(ViewPos - pIn.worldPosition);
    float3 reflectDir = reflect(light.Direction, pIn.worldNormal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
    float3 specular = spec * light.Specular;

    return ambient + diffuse + specular;
}

float3 ShadingByPointLight(VertexOut pIn, LightData light)
{
    float3 ambient = light.Ambient;

    float3 lightDir = normalize(light.Position - pIn.worldPosition);
    float diff = max(dot(lightDir, pIn.worldNormal), 0.0);
    float3 diffuse = diff * light.Diffuse;

    float3 viewDir = normalize(ViewPos - pIn.worldPosition);
    float3 reflectDir = reflect(light.Direction, pIn.worldNormal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
    float3 specular = spec * light.Specular;

    float distance = length(light.Position - pIn.worldPosition);
    float constantFactor = light.Misc0;
    float linearFactor = light.Misc1;
    float quadraticFactor = light.Misc2;
    float attenuation = 1.0 / max(constantFactor + linearFactor * distance + quadraticFactor * distance * distance, 0.01);
    return (ambient + diffuse + specular) * attenuation;
}

float3 ShadingBySpotLight(VertexOut pIn, LightData light)
{
    float3 ambient = light.Ambient;

    float3 lightDir = normalize(light.Position - pIn.worldPosition);
    float theta = dot(lightDir, normalize(light.Direction));
    float cutoff = light.Misc0;
    float outCutOff = light.Misc1;
    if(theta <= outCutOff) // outside spot light
        return ambient;
    float epsilon = cutoff - outCutOff;
    float intensity = clamp((theta - outCutOff) / epsilon, 0.0, 1.0);

    float diff = max(dot(pIn.worldNormal, lightDir), 0.0) * intensity;
    float3 diffuse = diff * light.Diffuse;

    float3 viewDir = normalize(ViewPos - pIn.worldPosition);
    float3 reflectDir = reflect(light.Direction, pIn.worldNormal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16) * intensity;
    float3 specular = spec * light.Specular;
    return ambient + diffuse + specular;
}

float4 PS(VertexOut pIn) : SV_Target
{
    float4 texColor = BaseMap.Sample(BaseMapSampler, pIn.st);
    float4 finalColor = pIn.color * texColor * Roughness;
    float3 irradiance = float3(0.0, 0.0, 0.0);
    for(int i = 0; i < LightCount; i = i + 1)
    {
        int type = Lights[i].LightType;
        if(type == EDirectionLight)
            irradiance += ShadingByDirectLight(pIn, Lights[i]);
        else if(type == ESpotLight)
            irradiance += ShadingBySpotLight(pIn, Lights[i]);
        else if(type == EPointLight)
            irradiance += ShadingByPointLight(pIn, Lights[i]);
    }
    return finalColor * float4(irradiance, 1.0);
}
