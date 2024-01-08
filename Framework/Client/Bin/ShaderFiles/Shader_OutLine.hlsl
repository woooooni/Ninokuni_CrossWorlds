#include "Engine_Shader_Defines.hpp"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
float4 g_LineColor = float4(0.631f, 0.353f, 0.094f, 1.f);

Texture2D g_DepthTarget;

float  g_fMask[9] = {
	-1, -1, -1,
	-1,  8, -1,
	-1, -1, -1
};
float  g_fCoord[3] = { -1, 0, +1 };
float  g_fDivier = 1;


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


PS_OUT PS_OUTLINE(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	float fCenterDepth = g_DepthTarget.Sample(PointSampler, In.vTexcoord).r;
	float4 fDepthDifference = 0;

	for (int i = 0; i < 9; ++i)
	{
		vector vDepthDesc = g_DepthTarget.Sample(PointSampler, float2(In.vTexcoord + float2(g_fCoord[i % 3] / 1600.f, g_fCoord[i / 3] / 900.f)));
		fDepthDifference += g_fMask[i] * (fCenterDepth - vDepthDesc.r) * 1000.f;
	}

	float fDepthScale = 0.01;
	float fOutlineThickness = fDepthScale * length(fDepthDifference);
	fOutlineThickness = saturate(fOutlineThickness * 10.f);

	if (fOutlineThickness < 0.1)
		Out.vColor = float4(1.f, 1.f, 1.f, 1.f);
	else
		Out.vColor = g_LineColor;

	return Out;
}

technique11 DefaultTechnique
{
	// 0
	pass OutLine
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_OUTLINE();
	}
}
