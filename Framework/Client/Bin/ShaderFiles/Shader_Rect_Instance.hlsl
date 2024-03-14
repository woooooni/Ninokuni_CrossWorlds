#include "Engine_Shader_Defines.hpp"

matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D	g_DiffuseTexture;

SamplerState ClampSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Clamp;
    AddressV = Clamp;
};

matrix WorldViewProj;
cbuffer NoiseBuffer
{
    float frameTime;
    float3 scrollSpeeds;
    float3 scales;
};


struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexcoord : TEXCOORD0;

	float4		vRight : TEXCOORD1;
	float4		vUp : TEXCOORD2;
	float4		vLook : TEXCOORD3;
	float4		vTranslation : TEXCOORD4;
    
    uint        iInstanceID : SV_INSTANCEID;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexcoord : TEXCOORD0;
};

struct FlameInput
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float2 vTexcoord1 : TEXCOORD1;
    float2 vTexcoord2 : TEXCOORD2;
    float2 vTexcoord3 : TEXCOORD3;
    
    uint iInstanceID : SV_INSTANCEID;
};

Texture2D fireTexture;
Texture2D noiseTexture;
Texture2D alphaTexture;

cbuffer DistortionBuffer
{
    float2 distortion1;
    float2 distortion2;
    float2 distortion3;
    float distortionScale;
    float distortionBias;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	float4x4	TransformMatrix = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);

	/* 정점 여섯개를 내가 원하는 상태로 변형한다. In.LocalSpace */
	vector		vPosition = mul(float4(In.vPosition, 1.f), TransformMatrix);

	Out.vPosition = mul(vPosition, matWVP);
	Out.vTexcoord = In.vTexcoord;

	
	return Out;
}

FlameInput FireVertexShader(VS_IN input)
{
    FlameInput output = (FlameInput) 0;

    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    float4x4 TransformMatrix = float4x4(input.vRight, input.vUp, input.vLook, input.vTranslation);
    vector vPosition = mul(float4(input.vPosition, 1.f), TransformMatrix);
	
    output.vPosition = mul(vPosition, matWVP);
    output.vTexcoord = input.vTexcoord;
    
    output.vTexcoord1 = input.vTexcoord * scales.x;
    output.vTexcoord1.y = output.vTexcoord1.y + (frameTime * scrollSpeeds.x);
    
    output.vTexcoord2 = input.vTexcoord * scales.y;
    output.vTexcoord2.y = output.vTexcoord2.y + (frameTime * scrollSpeeds.y);
    
    output.vTexcoord3 = input.vTexcoord * scales.z;
    output.vTexcoord3.y = output.vTexcoord3.y + (frameTime * scrollSpeeds.z);
    
    output.iInstanceID = input.iInstanceID;
    
    return output;
}

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexcoord : TEXCOORD0;
};

struct PS_OUT
{
	float4		vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(PointSampler, In.vTexcoord);

	if (0 == Out.vColor.a)
		discard;

	return Out;	
}

PS_OUT FirePixelShader(FlameInput input) : SV_TARGET
{
    PS_OUT output = (PS_OUT) 0;
    
    float4 noise1;
    float4 noise2;
    float4 noise3;
    float4 finalNoise;
    float perturb;
    float2 noiseCoords;
    float4 fireColor;
    float4 alphaColor;
    
    noise1 = noiseTexture.Sample(LinearSampler, input.vTexcoord1);
    noise2 = noiseTexture.Sample(LinearSampler, input.vTexcoord2);
    noise3 = noiseTexture.Sample(LinearSampler, input.vTexcoord3);
    
    noise1 = (noise1 - 0.5f) * 2.0f;
    noise2 = (noise2 - 0.5f) * 2.0f;
    noise3 = (noise3 - 0.5f) * 2.0f;

    noise1.xy = noise1.xy * distortion1.xy;
    noise2.xy = noise2.xy * distortion2.xy;
    noise3.xy = noise3.xy * distortion3.xy;
    
    finalNoise = noise1 + noise2 + noise3;
    
    perturb = ((1.0f - input.vTexcoord.y) * distortionScale) + distortionBias;
    
    noiseCoords.xy = (finalNoise.xy * perturb) + input.vTexcoord.xy;
    
    fireColor = fireTexture.Sample(ClampSampler, noiseCoords.xy);

    alphaColor = alphaTexture.Sample(ClampSampler, noiseCoords.xy);
    
    fireColor.a = alphaColor;
    
    output.vColor = fireColor;
    //output.vBloom = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    return output;
}

technique11 DefaultTechnique
{
	pass DefaultPass
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

    pass PerlinFire3D // 1
    {
        SetRasterizerState(RS_NoneCull);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 FireVertexShader();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 FirePixelShader();
    }
	
}