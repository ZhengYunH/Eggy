cbuffer ConstantBuffer : register(b0)
{
    matrix cWorld;
    matrix cView;
    matrix cProj;
}


struct VertexIn
{
    float3 pos : POSITION;
    float4 color : COLOR;
};

struct VertexOut
{
    float4 posH : SV_POSITION;
    float4 color : COLOR;
};
