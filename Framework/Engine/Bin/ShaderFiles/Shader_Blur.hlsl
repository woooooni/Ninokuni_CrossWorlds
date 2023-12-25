#include "Engine_Shader_Defines.hpp"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture2D g_BlurTarget;

float2 g_WinSize     = float2(1900.f, 900.f);
float  g_fWeight[13] = { 0.0561f , 0.1353f , 0.278f ,0.4868f , 0.7261f , 0.9231f, 1.f, 0.9231f , 0.7261f , 0.4868f , 0.278f , 0.1353f , 0.0561f };


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


struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexcoord : TEXCOORD0;
};

struct PS_OUT
{
	float4	vColor : SV_TARGET0;
};

PS_OUT PS_BLUR_DOWNSCALE(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vColor = g_BlurTarget.Sample(LinearSampler, In.vTexcoord);
	return Out;
}

PS_OUT PS_BLUR_Horizontal(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	float4 vColor = float4(0.f, 0.f, 0.f, 1.f);
	float  fTotal = 0.f;

	for (int i = -6; 7 > i; i++)
	{
		vColor += g_fWeight[i + 6] * g_BlurTarget.Sample(LinearSampler, In.vTexcoord + float2(1.f / (g_WinSize.x / 2.f) * i, 0.f));
		fTotal += g_fWeight[i + 6];
	}

	Out.vColor = vColor / fTotal;

	return Out;
}

PS_OUT PS_BLUR_Vertical(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	float4 vColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float  fTotal = 0.f;

	for (int i = -6; 7 > i; i++)
	{
		vColor += g_fWeight[i + 6] * g_BlurTarget.Sample(LinearSampler, In.vTexcoord + float2(0, 1.f / (g_WinSize.y / 2.f) * i));
		fTotal += g_fWeight[i + 6];
	}

	Out.vColor = vColor / fTotal;

	return Out;
}

PS_OUT PS_BLUR_UPSCALE(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;
	Out.vColor = g_BlurTarget.Sample(LinearSampler, In.vTexcoord);

	return Out;
}

technique11 DefaultTechnique
{
	// 0
	pass BlurDownScale
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_BLUR_DOWNSCALE();
	}

	// 1
	pass Blur_Horizontal
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_BLUR_Horizontal();
	}

	// 2
	pass Blur_Vertical
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_BLUR_Vertical();
	}

	// 3
	pass BlurUpScale
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_OneBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_BLUR_UPSCALE();
	}
}
