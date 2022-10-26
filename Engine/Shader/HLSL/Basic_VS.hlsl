#include "Basic.hlsli"

VertexOut VS(VertexIn vIn)
{
    VertexOut vOut;
    vOut.posH = mul(float4(vIn.pos, 1.0f), cWorld);
    vOut.posH = mul(vOut.posH, cView);
    vOut.posH = mul(vOut.posH, cProj);
    vOut.color = vIn.color;
    return vOut;
}
