#include "Engine_Shader_Defines.hpp"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D g_BlurTarget;

float2 g_WinSize = float2(1900.f, 900.f);
float  g_fWeight_low[3] =
{ 
	0.5f, 
	1.f, 
	0.5f 
};

float  g_fWeight_middle[7] =
{ 
	0.2f, 0.5f, 0.8f,
	1.f, 
	0.8f, 0.5f, 0.2f
};

float  g_fWeight_high[11] =
{ 
	0.1f, 0.2f, 0.4f, 0.6f, 0.8f,
	1.f,
	0.8f, 0.6f, 0.4f, 0.2f, 0.1f
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
	PS_OUT Out = (PS_OUT)0;
	Out.vColor = g_BlurTarget.Sample(LinearSampler, In.vTexcoord);
	return Out;
}

PS_OUT PS_BLUR_UPSCALE(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;
	Out.vColor = g_BlurTarget.Sample(LinearSampler, In.vTexcoord);
	return Out;
}


// Power low
PS_OUT PS_BLUR_Horizontal_low(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	float4 vColor = float4(0.f, 0.f, 0.f, 1.f);
	float  fTotal = 0.f;

	for (int i = -1; 2 > i; i++)
	{
		vColor += g_fWeight_low[i + 1] * g_BlurTarget.Sample(LinearSampler, In.vTexcoord + float2(1.f / (g_WinSize.x / 2.f) * i, 0.f));
		fTotal += g_fWeight_low[i + 1];
	}

	Out.vColor = vColor / fTotal;

	return Out;
}

PS_OUT PS_BLUR_Vertical_low(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	float4 vColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float  fTotal = 0.f;

	for (int i = -1; 2 > i; i++)
	{
		vColor += g_fWeight_low[i + 1] * g_BlurTarget.Sample(LinearSampler, In.vTexcoord + float2(0, 1.f / (g_WinSize.y / 2.f) * i));
		fTotal += g_fWeight_low[i + 1];
	}

	Out.vColor = vColor / fTotal;

	return Out;
}


// Power middle
PS_OUT PS_BLUR_Horizontal_middle(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	float4 vColor = float4(0.f, 0.f, 0.f, 1.f);
	float  fTotal = 0.f;

	for (int i = -3; 4 > i; i++)
	{
		vColor += g_fWeight_middle[i + 3] * g_BlurTarget.Sample(LinearSampler, In.vTexcoord + float2(1.f / (g_WinSize.x / 2.f) * i, 0.f));
		fTotal += g_fWeight_middle[i + 3];
	}

	Out.vColor = vColor / fTotal;

	return Out;
}

PS_OUT PS_BLUR_Vertical_middle(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	float4 vColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float  fTotal = 0.f;

	for (int i = -3; 4 > i; i++)
	{
		vColor += g_fWeight_middle[i + 3] * g_BlurTarget.Sample(LinearSampler, In.vTexcoord + float2(0, 1.f / (g_WinSize.y / 2.f) * i));
		fTotal += g_fWeight_middle[i + 3];
	}

	Out.vColor = vColor / fTotal;

	return Out;
}



// Power high
PS_OUT PS_BLUR_Horizontal_high(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	float4 vColor = float4(0.f, 0.f, 0.f, 1.f);
	float  fTotal = 0.f;

	for (int i = -5; 6 > i; i++)
	{
		vColor += g_fWeight_high[i + 5] * g_BlurTarget.Sample(LinearSampler, In.vTexcoord + float2(1.f / (g_WinSize.x / 2.f) * i, 0.f));
		fTotal += g_fWeight_high[i + 5];
	}

	Out.vColor = vColor / fTotal;

	return Out;
}

PS_OUT PS_BLUR_Vertical_high(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	float4 vColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float  fTotal = 0.f;

	for (int i = -5; 6 > i; i++)
	{
		vColor += g_fWeight_high[i + 5] * g_BlurTarget.Sample(LinearSampler, In.vTexcoord + float2(0, 1.f / (g_WinSize.y / 2.f) * i));
		fTotal += g_fWeight_high[i + 5];
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
	pass Blur_Horizontal_low
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_BLUR_Horizontal_low();
	}

	// 3
	pass Blur_Vertical_low
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_BLUR_Vertical_low();
	}


	// 4
	pass Blur_Horizontal_middle
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_BLUR_Horizontal_middle();
	}

	// 5
	pass Blur_Vertical_middle
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_BLUR_Vertical_middle();
	}


	// 6
	pass Blur_Horizontal_high
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_BLUR_Horizontal_high();
	}

	// 7
	pass Blur_Vertical_high
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_BLUR_Vertical_high();
	}
}
