#include "Engine_Shader_Defines.hpp"

matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

float g_fAlpha = 1.f;
float4	g_vDiffuseColor = { 0.1f, 0.1f, 0.1f, 1.f };
float2	g_vMaskTexUV;
float g_Ratio;

Texture2D	g_DiffuseTexture;
Texture2D	g_AlphaTexture;
Texture2D	g_DepthTexture;

Texture2D	g_MaskTexture;


struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};


VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
	
	return Out;
}

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	float4		vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	if (0.f >= Out.vColor.a)
		discard;

	return Out;	
}

PS_OUT PS_OPACTITY(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_AlphaTexture.Sample(LinearSampler, In.vTexUV);
	if (0.f >= Out.vColor.a)
		discard;

	return Out;
}

PS_OUT PS_REVERSE_OPACTITY(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vAlphaColor = g_AlphaTexture.Sample(LinearSampler, In.vTexUV);

	if (vAlphaColor.r >= 0.2f)
		discard;

	Out.vColor = vector(vAlphaColor.rgb, 1.f - vAlphaColor.r);

	if (0.f >= Out.vColor.a)
		discard;

	return Out;
}

PS_OUT PS_USING_ALPHA_VALUE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vTextureColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	if (vTextureColor.a == 0.f || g_fAlpha == 0.f)
		discard;

	vTextureColor.a = g_fAlpha;
	Out.vColor = vTextureColor;

	if (0.f >= Out.vColor.a)
		discard;

	return Out;
	
}

PS_OUT PS_USING_MASK(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = saturate(g_DiffuseTexture.Sample(LinearSampler, In.vTexUV) * g_vDiffuseColor);

	In.vTexUV += g_vMaskTexUV;
	float4 vMask = g_MaskTexture.Sample(LinearSampler, In.vTexUV);

	Out.vColor.a *= g_fAlpha;
	Out.vColor.a *= vMask.a;

	if (0.0f >= Out.vColor.a)
		discard;

	return Out;
}

PS_OUT PS_CIRCLE_MASK(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0; // 초기화

	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV); // 텍스처에서 샘플링된 색상을 가져온다.

	if (g_Ratio <= 0.f) // 비율이 0인 경우
	{
		Out.vColor.rgb = float3(0.85f, 0.f, 0.f);
		return Out;
	}

	float2 vDir = In.vTexUV - float2(0.5f, 0.5f); // float2(0.5f, 0.5f)는 중점이다.
	vDir = normalize(vDir); // 방향벡터 Normalize
	float2 vUpDir = float2(0.0f, sign(vDir.x));
	vUpDir = normalize(vUpDir);

	float fDot = dot(vUpDir, vDir); // 두 벡터를 내적한다.
	float fDotRatio = g_Ratio;

	// 방향벡터가 음수인 경우, 비교할 기준 벡터의 방향은 위
	if (vDir.x < 0.f)
	{
		fDotRatio -= 0.5f;
	}


	fDotRatio = fDotRatio * 4.f - 1.f;

	if (fDotRatio < fDot) // 픽셀을 버린다.
		discard;

	// 특정 영역에서만 색상이 표시된다.
	return Out;
}

technique11 DefaultTechnique
{
	pass DefaultPass // 0
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}
	pass OpacityPass // 1
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_OPACTITY();
	}

	pass ReverseOpacityPass // 2
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_REVERSE_OPACTITY();
	}

	pass UsingAlphaValue // 3
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_USING_ALPHA_VALUE();
	}

	pass UsingMaskTexture // 4
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_USING_MASK();
	}

	pass CircleMaskTexture // 5
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_AlphaBlend, float4(1.f, 1.f, 1.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_CIRCLE_MASK();
	}
	
}