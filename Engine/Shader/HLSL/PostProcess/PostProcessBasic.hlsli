#include "../Common.hlsli"

struct VertexIn
{
	float3 pos : POSITION;
	float2 st: TEXCOORD;
};

struct VertexOut
{
	float4 posH : SV_POSITION;
	float2 st : TEXCOORD;
};