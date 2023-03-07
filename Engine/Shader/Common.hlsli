cbuffer Global : register(b0)
{
    row_major matrix cView;
    row_major matrix cProj;
    float3 ViewPos;
    float  __spare__;
    float4 DebugColor;
}


#define EDirectionLight 0
#define EPointLight 1
#define ESpotLight 2

struct LightData
{
    float LightType;
    float3 Position;
    float Misc0;
    float3 Direction;
    float Misc1;
    float3 Ambient;
    float Misc2;
    float3 Diffuse;
    float Misc3;
    float3 Specular;
};

cbuffer Light : register(b3)
{
    LightData Lights[4];
    int LightCount;
}

cbuffer Skeleton : register(b4)
{

}


