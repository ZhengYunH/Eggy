
struct GBufferData
{
    // GBufferA RGBA8f
	float3 WorldNormal;
    /* float __PlaceHolder__; */

    // GBufferB RGBA8f
    float Metallic;
    float Specular;
	float Roughness;
	int ShadingModelID;

    // GBufferC RGBA8f
	float3 BaseColor;
    float AO;

    // GBufferD RGBA8f
    float4 CustomData;

    // GBufferE R32f
    float LinearDepth; 
};

GBufferData GetInitialGBufferData(int ShadingModelID)
{
    GBufferData GBuffer;
    GBuffer.WorldNormal = float3(0, 1, 0);

    GBuffer.Metallic = 0.0;
    GBuffer.Specular = 0.0;
    GBuffer.Roughness = 0.0;
    GBuffer.ShadingModelID = ShadingModelID;

    GBuffer.BaseColor = float3(0, 0, 0);
    GBuffer.AO = 1;

    GBuffer.CustomData = float4(0, 0, 0, 0);
    GBuffer.LinearDepth = 1;

    return GBuffer;
}


float EncodeShadingModelID(float ShadingModelID)
{
	return (float)ShadingModelID / (float)0xFF;
}

int DecodeShadingModelID(float val)
{
	return (int)round(val * (float)0xFF);
}

void EncodeGBuffer(GBufferData GBuffer, out float4 GBufferA, out float4 GBufferB, out float4 GBufferC, out float4 GBufferD, out float GBufferE)
{
    GBufferA = float4(GBuffer.WorldNormal, 0.0);
    GBufferB = float4(GBuffer.Metallic, GBuffer.Specular, GBuffer.Roughness, EncodeShadingModelID(GBuffer.ShadingModelID));
    GBufferC = float4(GBuffer.BaseColor, GBuffer.AO);
    GBufferD = GBuffer.CustomData;
    GBufferE = GBuffer.LinearDepth;
}

GBufferData DecodeGBuffer(float4 GBufferA, float4 GBufferB, float4 GBufferC, float4 GBufferD, float GBufferE)
{
	GBufferData GBuffer;
    GBuffer.WorldNormal = GBufferA.xyz;
	
    GBuffer.Metallic = GBufferB.x;
    GBuffer.Specular = GBufferB.y;
    GBuffer.Roughness = GBufferB.z;
    GBuffer.ShadingModelID = DecodeShadingModelID(GBufferB.w);

    GBuffer.BaseColor = GBufferC.rgb;
    GBuffer.AO = GBufferC.a;

    GBuffer.CustomData = GBufferD;

    GBuffer.LinearDepth = GBufferE;
    return GBuffer;
}

