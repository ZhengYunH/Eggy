#include "Basic.hlsli"

Texture2D MeshTexture : register(t0);
SamplerState MeshTextureSampler : register(s0)
{
    Filter = Linear;
    AddressU = Wrap;
    AddressV = Wrap;
};

float4 PS(VertexOut pIn) : SV_Target
{
    float4 texColor = MeshTexture.Sample(MeshTextureSampler, pIn.st);
    return pIn.color * texColor;   
}
