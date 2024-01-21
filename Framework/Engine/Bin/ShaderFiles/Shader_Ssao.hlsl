#include "Engine_Shader_Defines.hpp"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

Texture2D g_NormalTarget;
Texture2D g_DepthTarget;

float  g_fRadius = 0.005f;
float3 g_vRandom[16] =
{
	float3(0.2024537f, 0.841204f, -0.9060141f),
	float3(-0.2200423f, 0.6282339f, -0.8275437f),
	float3(0.3677659f, 0.1086345f, -0.4466777f),
	float3(0.8775856f, 0.4617546f, -0.6427765f),
	float3(0.7867433f, -0.141479f, -0.1567597f),
	float3(0.4839356f, -0.8253108f, -0.1563844f),
	float3(0.4401554f, -0.4228428f, -0.3300118f),
	float3(0.0019193f, -0.8048455f, 0.0726584f),
	float3(-0.7578573f, -0.5583301f, 0.2347527f),
	float3(-0.4540417f, -0.252365f, 0.0694318f),
	float3(-0.0483353f, -0.2527294f, 0.5924745f),
	float3(-0.4192392f, 0.2084218f, -0.3672943f),
	float3(-0.8433938f, 0.1451271f, 0.2202872f),
	float3(-0.4037157f, -0.8263387f, 0.4698132f),
	float3(-0.6657394f, 0.6298575f, 0.6342437f),
	float3(-0.0001783f, 0.2834622f, 0.8343929f),
};

float3 Get_RandomNormal(float2 fUV)
{
	
	float fNoiseX = frac(sin(dot(fUV, float2(15.8989f, 76.132f) * 1.0f)) * 46336.23745f);
	float fNoiseY = frac(sin(dot(fUV, float2(11.9899f, 62.223f) * 2.0f)) * 34748.34744f);
	float fNoiseZ = frac(sin(dot(fUV, float2(13.3238f, 63.122f) * 3.0f)) * 59998.47362f);

	return normalize(float3(fNoiseX, fNoiseY, fNoiseZ));
}

struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexcoord : TEXCOORD0;
	float		fFogFactor : FOG;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
	Out.vTexcoord = In.vTexcoord;

	return Out;
}

struct PS_IN_SSAO
{
	float2 fUV;
	float  fDepth;
	float3 vNormal;
};

struct PS_OUT_SSAO
{
	float4 vAmbient;
};

PS_OUT_SSAO Get_SSAO(PS_IN_SSAO In)
{
	PS_OUT_SSAO Out = (PS_OUT_SSAO)0.f;

	float3 vRay;
	float3 vReflect;
	float2 vRandomUV;
	float  fOccNormal;

	int iColor = 0;
	for (int i = 0; i < 16; ++i)
	{
		vRay = reflect(Get_RandomNormal(In.fUV), g_vRandom[i]);

		vReflect = normalize(reflect(vRay, In.vNormal)) * g_fRadius;
		vReflect.x *= -1.f;

		vRandomUV = In.fUV + vReflect.xy;
		fOccNormal = g_DepthTarget.Sample(LinearSampler, vRandomUV).g * 1000.f;

		if (fOccNormal < In.fDepth + 0.01f)
			++iColor;
	}

	Out.vAmbient = abs((iColor / 16.f) - 1.f);

	return Out;
}

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexcoord : TEXCOORD0;
};

struct PS_OUT
{
	float4	vColor : SV_TARGET0;
};

PS_OUT PS_SSAO(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	vector vDepth = g_DepthTarget.Sample(PointSampler, In.vTexcoord);
	float  fViewZ = vDepth.y * 1000.f;

	vector vNormal = g_NormalTarget.Sample(PointSampler, In.vTexcoord);
	vNormal = vector(vNormal.xyz * 2.f - 1.f, 0.f);

	PS_IN_SSAO SSAO_In = (PS_IN_SSAO)0;
	SSAO_In.fUV     = In.vTexcoord;
	SSAO_In.vNormal = vNormal.rgb;
	SSAO_In.fDepth  = fViewZ;

	PS_OUT_SSAO SSAOAmbient = Get_SSAO(SSAO_In);
	Out.vColor = (1.f - SSAOAmbient.vAmbient);

	return Out;
}

technique11 DefaultTechnique
{
	// 0
	pass Ssao 
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_SSAO();
	}
}