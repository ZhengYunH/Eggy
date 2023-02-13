#include "PostProcessBasic.hlsli"


VertexOut VS(VertexIn vIn)
{
    VertexOut vOut;
    vOut.posH = float4(vIn.pos, 1.0);
    vOut.st = vIn.st;
    return vOut;
}
