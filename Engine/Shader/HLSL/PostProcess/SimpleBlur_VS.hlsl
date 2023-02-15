#include "PostProcessBasic.hlsli"


VertexOut VS(VertexIn vIn)
{
    VertexOut vOut;
	float3 vPos = vIn.pos;
	vPos.y = 1 - vPos.y;
    vOut.posH = float4(vPos.xy * 2 - 1, vPos.z, 1.0);
    vOut.st = vIn.st;
    return vOut;
}
