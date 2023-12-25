#include "Engine_Shader_Defines.hpp"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D g_BlurTarget;

float2 g_WinSize = float2(1900.f, 900.f);
float  g_fWeight_01[3] = 
{ 
	0.5f, 
	1.f, 
	0.5f 
};
float  g_fWeight_02[7]  = 
{ 
	0.1f, 0.4f, 0.7f, 
	1.f, 
	0.7f, 0.4f, 0.1f
};
float  g_fWeight_03[11] = 
{ 
	0.05f, 0.2f, 0.4f, 0.6f, 0.8f, 
	1.f, 
	0.8f, 0.6f, 0.4f, 0.2f, 0.05f
};
float  g_fWeight_04[15] = 
{ 
	0.05f, 0.15f, 0.3f, 0.45f, 0.6f, 0.75f, 0.9f,
	1.f,
	0.9f, 0.75f, 0.6f, 0.45f, 0.3f, 0.15f, 0.05f
};
float  g_fWeight_05[19] = 
{ 
	0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f,
	1.f,
	0.9f, 0.8f, 0.7f, 0.6f, 0.5f, 0.4f, 0.3f, 0.2f, 0.1f
};

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

PS_OUT PS_BLUR_UPSCALE(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;
	Out.vColor = g_BlurTarget.Sample(LinearSampler, In.vTexcoord);

	return Out;
}


// Power 1
PS_OUT PS_BLUR_Horizontal_01(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	float4 vColor = float4(0.f, 0.f, 0.f, 1.f);
	float  fTotal = 0.f;

	for (int i = -1; 2 > i; i++)
	{
		vColor += g_fWeight_01[i + 1] * g_BlurTarget.Sample(LinearSampler, In.vTexcoord + float2(1.f / (g_WinSize.x / 2.f) * i, 0.f));
		fTotal += g_fWeight_01[i + 1];
	}

	Out.vColor = vColor / fTotal;

	return Out;
}

PS_OUT PS_BLUR_Vertical_01(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	float4 vColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float  fTotal = 0.f;

	for (int i = -1; 2 > i; i++)
	{
		vColor += g_fWeight_01[i + 1] * g_BlurTarget.Sample(LinearSampler, In.vTexcoord + float2(0, 1.f / (g_WinSize.y / 2.f) * i));
		fTotal += g_fWeight_01[i + 1];
	}

	Out.vColor = vColor / fTotal;

	return Out;
}


// Power 2
PS_OUT PS_BLUR_Horizontal_02(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	float4 vColor = float4(0.f, 0.f, 0.f, 1.f);
	float  fTotal = 0.f;

	for (int i = -3; 4 > i; i++)
	{
		vColor += g_fWeight_02[i + 3] * g_BlurTarget.Sample(LinearSampler, In.vTexcoord + float2(1.f / (g_WinSize.x / 2.f) * i, 0.f));
		fTotal += g_fWeight_02[i + 3];
	}

	Out.vColor = vColor / fTotal;

	return Out;
}

PS_OUT PS_BLUR_Vertical_02(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	float4 vColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float  fTotal = 0.f;

	for (int i = -3; 4 > i; i++)
	{
		vColor += g_fWeight_02[i + 3] * g_BlurTarget.Sample(LinearSampler, In.vTexcoord + float2(0, 1.f / (g_WinSize.y / 2.f) * i));
		fTotal += g_fWeight_02[i + 3];
	}

	Out.vColor = vColor / fTotal;

	return Out;
}


// Power 3
PS_OUT PS_BLUR_Horizontal_03(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	float4 vColor = float4(0.f, 0.f, 0.f, 1.f);
	float  fTotal = 0.f;

	for (int i = -5; 6 > i; i++)
	{
		vColor += g_fWeight_03[i + 5] * g_BlurTarget.Sample(LinearSampler, In.vTexcoord + float2(1.f / (g_WinSize.x / 2.f) * i, 0.f));
		fTotal += g_fWeight_03[i + 5];
	}

	Out.vColor = vColor / fTotal;

	return Out;
}

PS_OUT PS_BLUR_Vertical_03(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	float4 vColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float  fTotal = 0.f;

	for (int i = -5; 6 > i; i++)
	{
		vColor += g_fWeight_03[i + 5] * g_BlurTarget.Sample(LinearSampler, In.vTexcoord + float2(0, 1.f / (g_WinSize.y / 2.f) * i));
		fTotal += g_fWeight_03[i + 5];
	}

	Out.vColor = vColor / fTotal;

	return Out;
}


// Power 4
PS_OUT PS_BLUR_Horizontal_04(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	float4 vColor = float4(0.f, 0.f, 0.f, 1.f);
	float  fTotal = 0.f;

	for (int i = -7; 8 > i; i++)
	{
		vColor += g_fWeight_04[i + 7] * g_BlurTarget.Sample(LinearSampler, In.vTexcoord + float2(1.f / (g_WinSize.x / 2.f) * i, 0.f));
		fTotal += g_fWeight_04[i + 7];
	}

	Out.vColor = vColor / fTotal;

	return Out;
}

PS_OUT PS_BLUR_Vertical_04(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	float4 vColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float  fTotal = 0.f;

	for (int i = -7; 8 > i; i++)
	{
		vColor += g_fWeight_04[i + 7] * g_BlurTarget.Sample(LinearSampler, In.vTexcoord + float2(0, 1.f / (g_WinSize.y / 2.f) * i));
		fTotal += g_fWeight_04[i + 7];
	}

	Out.vColor = vColor / fTotal;

	return Out;
}


// Power 5
PS_OUT PS_BLUR_Horizontal_05(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	float4 vColor = float4(0.f, 0.f, 0.f, 1.f);
	float  fTotal = 0.f;

	for (int i = -9; 10 > i; i++)
	{
		vColor += g_fWeight_05[i + 9] * g_BlurTarget.Sample(LinearSampler, In.vTexcoord + float2(1.f / (g_WinSize.x / 2.f) * i, 0.f));
		fTotal += g_fWeight_05[i + 9];
	}

	Out.vColor = vColor / fTotal;

	return Out;
}

PS_OUT PS_BLUR_Vertical_05(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	float4 vColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float  fTotal = 0.f;

	for (int i = -9; 10 > i; i++)
	{
		vColor += g_fWeight_05[i + 9] * g_BlurTarget.Sample(LinearSampler, In.vTexcoord + float2(0, 1.f / (g_WinSize.y / 2.f) * i));
		fTotal += g_fWeight_05[i + 9];
	}

	Out.vColor = vColor / fTotal;

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


	// 2
	pass Blur_Horizontal_01
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_BLUR_Horizontal_01();
	}

	// 3
	pass Blur_Vertical_01
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_BLUR_Vertical_01();
	}

	// 4
	pass Blur_Horizontal_02
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_BLUR_Horizontal_02();
	}

	// 5
	pass Blur_Vertical_02
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_BLUR_Vertical_02();
	}

	// 6
	pass Blur_Horizontal_03
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_BLUR_Horizontal_03();
	}

	// 7
	pass Blur_Vertical_03
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_BLUR_Vertical_03();
	}

	// 8
	pass Blur_Horizontal_04
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_BLUR_Horizontal_04();
	}

	// 9
	pass Blur_Vertical_04
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_BLUR_Vertical_04();
	}

	// 10
	pass Blur_Horizontal_05
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_BLUR_Horizontal_05();
	}

	// 11
	pass Blur_Vertical_05
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_BLUR_Vertical_05();
	}
}
