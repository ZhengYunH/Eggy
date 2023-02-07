cbuffer ConstantBuffer : register(b0)
{
	row_major matrix cWorld;
}

cbuffer GlobalBuffer : register(b2)
{
    row_major matrix cView;
    row_major matrix cProj;
}

struct VertexIn
{
    float3 pos : POSITION;
    float4 color : COLOR;
    float2 st : TEXCOORD;
};

struct VertexOut
{
    float4 posH : SV_POSITION;
    float4 color : COLOR;
    float2 st : TEXCOORD;
};
