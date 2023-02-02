#include "Basic.hlsli"

VertexOut VS(VertexIn vIn)
{
    VertexOut vOut;
    vOut.posH = mul(cWorld, float4(vIn.pos, 1.0f));
    vOut.posH = mul(cView, vOut.posH);
    vOut.posH = mul(cProj, vOut.posH);
    vOut.color = vIn.color;
    vOut.st = vIn.st;
    return vOut;
}
