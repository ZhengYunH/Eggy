#include "ShadingModel.hlsli"

struct VertexIn
{
	float3 pos : POSITION;
	float4 color : COLOR;
	float2 st: TEXCOORD;
};

struct VertexOut
{
	float4 posH : SV_POSITION;
	float2 st : TEXCOORD;
	float3 ScreenVector : TEXCOORD1;
};

Texture2D tGBufferA : register(t0);
SamplerState tGBufferASampler : register(s0);

Texture2D tGBufferB : register(t1);
SamplerState tGBufferBSampler : register(s1);

Texture2D tGBufferC : register(t2);
SamplerState tGBufferCSampler : register(s2);

Texture2D tGBufferD : register(t3);
SamplerState tGBufferDSampler : register(s3);

Texture2D<float> tGBufferE : register(t4);
SamplerState tGBufferESampler : register(s4);


GBufferData SampleGBuffer(float2 tc)
{
	GBufferData GBuffer = DecodeGBuffer(
        tGBufferA.Sample(tGBufferASampler, tc),
        tGBufferB.Sample(tGBufferBSampler, tc),
        tGBufferC.Sample(tGBufferCSampler, tc),
        tGBufferD.Sample(tGBufferDSampler, tc),
        tGBufferE.Sample(tGBufferESampler, tc)
    );
    return GBuffer;
}

float3 HomScreenVector(float2 st)
{
	return cWBasisZ.xyz + (cWBasisX.xyz * st.x) + (cWBasisY.xyz * st.y);
}

VertexOut VS(VertexIn vIn)
{
    VertexOut vOut;
	float3 vPos = vIn.pos;
	vPos.y = 1 - vPos.y;
    vOut.posH = float4(vPos.xy * 2 - 1, vPos.z, 1.0);
    vOut.st = vIn.st;
	vOut.ScreenVector = HomScreenVector(vIn.st);
    return vOut;
}

float4 PS(VertexOut pIn) : SV_Target
{
	LightingResult LResult;
	LResult.DiffuseLighting = float3(0, 0, 0);
	LResult.SpecularLighting = float3(0, 0, 0);

	GBufferData GBuffer = SampleGBuffer(pIn.st);
	GBuffer.ViewDir = normalize(pIn.ScreenVector);
	float3 WorldPosition = cViewPos + pIn.ScreenVector * GBuffer.LinearDepth;
	for(int i = 0; i < LightCount; i = i + 1)
	{
		LightingData LitData = GetLightDataByLight(Lights[i], WorldPosition);
		IntegrateBxDF(LResult, GBuffer, LitData);
	}

	float3 baseColor = LResult.DiffuseLighting + LResult.SpecularLighting;
	return float4(baseColor, 1);
}
