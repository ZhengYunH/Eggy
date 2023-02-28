cbuffer Batch : register(b0)
{
	row_major matrix cWorld;
}

cbuffer Global : register(b2)
{
    row_major matrix cView;
    row_major matrix cProj;
    float4 DebugColor;
}