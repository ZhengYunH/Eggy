#ifndef __FILE_COMMON_HEADER__
#define __FILE_COMMON_HEADER__


#ifndef PI
	#define PI 3.1415926535897932384626433832795
#endif

#define EDirectionLight 0
#define EPointLight 1
#define ESpotLight 2

#define SHADINGMODELID_UNLIT 		(0)
#define SHADINGMODELID_DEFAULT_LIT 	(1)

#define SHADINGMODELID_CLEAR_COAT	(4)
#define SHADINGMODELID_SSS			(8)

cbuffer Global : register(b0)
{
    row_major matrix cView;
    row_major matrix cProj;
    float3 cViewPos;
    float  __spare__;
    float4 cWBasisX;
    float4 cWBasisY;
    float4 cWBasisZ;
    float4 cCameraInfo; // near, far, nearInv, farInv
    float4 cDebugColor;
}



struct LightData
{
    int LightType;
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


#endif /* __FILE_COMMON_HEADER__ */
